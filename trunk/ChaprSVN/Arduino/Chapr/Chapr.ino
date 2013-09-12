#include <arduino.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "VDIPSPI.h"
#include "VDIP.h"
#include "BT.h"
#include "blinky.h"
#include "sound.h"
#include "nxt.h"
#include "ChapRName.h"
#include "ChapREEPROM.h"
#include "button.h"
#include "gamepad.h"
#include "personality.h"
#include "personality_0.h"		// NXT-RobotC
#include "personality_1.h"		// NXT-G

#include "debug.h"

/****************************************************************************************/
/* OBJECTS										*/
/*   	First, instantiate all of our objects we are using - note that these things are	*/
/*	created BEFORE any other code is run, so they can't do silly things in their	*/
/*	constructors, like write to Serial!						*/
/*											*/
/*	It's pretty cool, though, that the constructors for the VDIP and BT module are	*/
/*	called early because it takes awhile for them to come up, so the sooner they	*/
/*	start, the better.								*/
/*											*/
/*	Some "special" objects are the "personalities".  they govern how the ChapR	*/
/*	interats with the rest of the world.  they are polymorphic so that the given	*/
/*	personality can be chosen, and used.						*/
/****************************************************************************************/

blinky	powerLED(LED_POWER);
blinky	indicateLED(LED_INDICATE);

sound	beeper(TONEPIN);

VDIP	vdip(VDIP_CLOCK, VDIP_MOSI, VDIP_MISO, VDIP_CS, VDIP_RESET);
BT	bt(BT_RX, BT_TX, BT_RESET, BT_MODE, BT_9600BAUD, BT_CONNECTED);

button	theButton(BUTTON);
button	powerButton(POWER_BUTTON,true);

ChapREEPROM myEEPROM;

Personality_0	p0;
Personality_1	p1;
Personality	*personalities[] = { &p0, &p1 };
int		current_personality;

Gamepad		g1(1);		// I'm gamepad #1!
Gamepad		g2(2);		// I'm gamepad #2!

Gamepad		g1_prev(1);	// the buffer for the gamepad data before current changes
Gamepad		g2_prev(2);	// the buffer for the gamepad data before current changes

/****************************************************************************************/
/* VARIABLES										*/
/*   	In addition to the objects, we have a few variables that we use here.		*/
/*											*/
/****************************************************************************************/

byte emptyJSData[] = { 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04, 0x00 };

bool    inConfigMode; //whether or not the ChapR is in pairing mode
long     powerTimeout; //how long until the ChapR turns itself off (configured by user)

//
// setup() - this routine is run ONCE by the Arduino upon start-up.
//
void setup()
{
     pinMode(POWER_ON_HOLD,OUTPUT);	// the power-on-hold needs to be turned on as soon as possible
     digitalWrite(POWER_ON_HOLD,HIGH);
     
     powerLED.fast();			// flash the power LED during boot
     
     Serial.begin(LOCAL_SERIAL_BAUD);	// the serial monitor operates at this BAUD
     Serial.println("ChapR v0.3 up!");
     
     if (!myEEPROM.isInitialized()){
       Serial.println("Please intialize your ChapR.");
       myEEPROM.setFromConsole("ChapRX", (byte) 10, (byte) 1);
     }
     
     powerTimeout = 60000 * (long) myEEPROM.getTimeout(); //sets the timeout from EEPROM
     
     // check the WFS button to see if it was pressed upon boot, if so, enter config mode
     if (digitalRead(BUTTON) == HIGH) {		// the button has a pull-down, so normally LOW
          bt.configMode(myEEPROM.getName());
          inConfigMode = true;
	  powerLED.slow();
     } else {
	  bt.opMode();
          inConfigMode = false;
	  powerLED.on();
     }

     while(!vdip.sync()) {		// while waiting, update the LED status
	  powerLED.update();
     }

     vdip.deviceUpdate();		// get initial device setup
     current_personality = myEEPROM.getPersonality();

     g1.load(emptyJSData);
     g2.load(emptyJSData);

     beeper.confirm();
}

void enterZombieMode()
{
   powerLED.off();
   indicateLED.off();
   
   bt.zombieMode();
   vdip.zombieMode();

   // turn off the Pro Mini green LED - it is directly connected to pin 13
   // so turn it off at the very last minute so we don't interfere with whatever
   // else happens to be connected to pin 13

   digitalWrite(13,LOW);		// turns off the green LED

   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
   sleep_enable();
   sleep_cpu();
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
    asm volatile ("  jmp 0");  
}

#define DEVICE_UPDATE_LOOP_COUNT	50

void loop()
{
     static bool	power_button_released = false;
     static int	        loopCount = 0;
     static bool	wasConnected = false;
     static int         timeButtonPressed; //how long the power button has been pressed (makes sure the ChapR isn't accidentally turned off)
     static long        lastAnyAction = millis();
     bool		js1 = false;
     bool		js2 = false;
     bool               wfs = false;
    
    if (Serial.available() > 0){
      myEEPROM.setFromConsole(myEEPROM.getName(), myEEPROM.getTimeout(), myEEPROM.getPersonality());
      current_personality = myEEPROM.getPersonality();	// in case the personality changed
      powerTimeout = 60000 * (long) myEEPROM.getTimeout();
    }
    
     // when we first boot, the power button is pressed in, so ensure that it changes before monitoring it for shutdown
     if(powerButton.hasChanged()) {
       timeButtonPressed = 0;
	  if(!power_button_released) {
	       power_button_released = true;
	  } else {
               personalities[current_personality-1]->Kill(&bt); 
          }
     }
     
     if (power_button_released && powerButton.isPressed()){
       timeButtonPressed++;
       if (timeButtonPressed > POWEROFFHOLDDOWN){
           powerLED.off();
           indicateLED.off();
           digitalWrite(POWER_ON_HOLD,LOW);
           enterZombieMode();
       }
     }

     // check each joystick that is connected, and grab a packet of information from it if there is any
     if (g2.update(&vdip)) {
	  js2 = true;
	  personalities[current_personality-1]->ChangeInput(&bt,2,&g2_prev,&g2);
	  g2_prev = g2;
     }

     if (g1.update(&vdip)) {
	  js1 = true;
	  personalities[current_personality-1]->ChangeInput(&bt,1,&g1_prev,&g1);
	  g1_prev = g1;
     }

     if (theButton.hasChanged()){
	  wfs = true;
	  personalities[current_personality-1]->ChangeButton(&bt,theButton.isPressed());
     }

     if((loopCount % DEVICE_UPDATE_LOOP_COUNT) == 0) {
	  vdip.deviceUpdate();
     }

     // check to see if we're connected to the brick - turn on the light if so if not connected, blink the thing
     if(bt.connected()) {
          if(inConfigMode){
            inConfigMode = false;
            software_Reset();          //does not initialize the IO lines, just resets master/slave pairing
          }
	  indicateLED.on();
	  powerLED.on();
	  if (!wasConnected) {
	       wasConnected = true;
	       beeper.squeep();
	  }
     } else {
	  if (wasConnected) {
	       wasConnected = false;
	  }
	  indicateLED.slow();
     }

     personalities[current_personality-1]->Loop(&bt,theButton.isPressed(),&g1,&g2);
     
     //checks to see if we should enter a power saving mode (if 5 min has passed)
     if (js1 || js2 || wfs){ //if something has happened, make note of the time since boot
        lastAnyAction = millis();
     }
     
     if (powerTimeout != 0 && millis() - lastAnyAction >= powerTimeout){
	  digitalWrite(POWER_ON_HOLD,LOW);
     }
     
     // update the state of the LEDs - this should always be done at the end of the loop
     powerLED.update();
     indicateLED.update();
     
     // allow only a certain number of updates - saves battery
     delay(5);

     loopCount++;
}
