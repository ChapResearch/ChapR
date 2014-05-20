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
#include "gamepad.h"
#include "robotc.h"
#include "nxt.h"
#include "settings.h"
#include "button.h"
#include "gamepad.h"
#include "personality.h"
#include "personality_0.h"		// NXT-RobotC
#include "personality_1.h"		// NXT-G
#include "personality_2.h"              // NXT-LabView
#include "personality_3.h"		// cRIO-LabView
#include "power.h"

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

settings myEEPROM;

Personality_0	p0;
Personality_1	p1;
Personality_2   p2;
Personality_3	p3;
Personality	*personalities[] = { &p0, &p1, &p2, &p3 };
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

bool    inConfigMode; // whether or not the ChapR is in pairing mode
long    powerTimeout; // how long until the ChapR turns itself off (configured by user)
int     lag; // changes the delay between loops

// set to true when the power button is pressed for the first time, which makes sure kill codes
// aren't sent on power up (and the ChapR isn't turned off by having the power button pressed for
// too long).
bool	power_button_released = false;


//
// setup() - this routine is run ONCE by the Arduino upon start-up.
//
void setup()
{
     pinMode(POWER_ON_HOLD,OUTPUT);	// the power-on-hold needs to be turned on as soon as possible
     digitalWrite(POWER_ON_HOLD,HIGH);
     
     powerLED.fast();			// flash the power LED during boot
     
     Serial.begin(LOCAL_SERIAL_BAUD);	// the serial monitor operates at this BAUD
     Serial.print("ChapR ");
     Serial.print(CODEVERSION);
     Serial.println(" up!");

     // EEPROM starts off unitialized - if this is the case, set the basic defaults
     //	and cause the user to have to go through basic settings - normally this is
     // only used when bringing up a board.  See settings.h for the order of the
     // arguments in setDefaults().  See config.h for the defaults.

     if (!myEEPROM.isInitialized()){
	  myEEPROM.setDefaults(DEF_NAME, DEF_TIMEOUT, DEF_PERSON, DEF_LAG, DEF_MODE,
			       DEF_AUTOLEN, DEF_TELELEN, DEF_ENDLEN,
			       DEF_DGTLIN, DEF_ANALOG1, DEF_ANALOG2, DEF_ANALOG3, DEF_ANALOG4);
	  myEEPROM.setFromConsole();
     }		

     myEEPROM.loadCache();

     // checks to see if the ChapR has undergone a software reset, making sure it remembers that
     // the power button had already been pressed (this makes sure the kill switch works the first
     // time).
     if (myEEPROM.getResetStatus() > 0){
       myEEPROM.setResetStatus(myEEPROM.getResetStatus()-1);
       power_button_released = true;
     }

     // check the WFS button to see if it was pressed upon boot, if so, enter config mode

     if (digitalRead(BUTTON) == HIGH) {		// the button has a pull-down, so normally LOW
          inConfigMode = true;			// in pairing/config mode
	  powerLED.slow();
     } else {
          inConfigMode = false;			// in normal mode
	  powerLED.on();
     }

     powerTimeout = 60000 * (long) myEEPROM.getTimeout(); //sets the timeout from EEPROM
     lag = myEEPROM.getSpeed(); //sets the lag from EEPROM
     
     while(!vdip.sync()) {		// while waiting, update the LED status
	  powerLED.update();
     }

     vdip.deviceUpdate();		// get initial device setup
     g1.deviceUpdate(&vdip);
     g2.deviceUpdate(&vdip);

     current_personality = myEEPROM.getPersonality();
     
     g1.clear();
     g2.clear();

     if (inConfigMode) {
          bt.configMode(myEEPROM.getName());
     } else {
	  bt.opMode();
	  // lowPowerOperation() is turned off to allow the ADC and Serial port to function
	  // we "may" want to turn it back on eventually - but the savings has gotten quite
	  // small with the turn-on of the ADC
	  //
	  // lowPowerOperation();		// turn off all unused portions of the ARDUINO - including serial port
     }

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
    myEEPROM.setResetStatus(myEEPROM.getResetStatus()+1); // tell the ChapR it has undergone a software reset
    asm volatile ("  jmp 0");  
}

#define DEVICE_UPDATE_LOOP_COUNT	50

void loop()
{
     static int	        loopCount = 0;
     static bool	wasConnected = false;
     static long         timeButtonPressed; //how long the power button has been pressed (makes sure the ChapR isn't accidentally turned off)
     static long        lastAnyAction = millis();
     bool		js1 = false;
     bool		js2 = false;
     bool               wfs = false;
     bool               pb = false;
     bool		lowBattery = false;

    if (Serial.available() > 0){
	 myEEPROM.setFromConsole();
	 current_personality = myEEPROM.getPersonality();	// in case the personality changed
	 powerTimeout = 60000 * (long) myEEPROM.getTimeout();
	 lag = myEEPROM.getSpeed();
    }
    
     // when we first boot, the power button is pressed in, so ensure that it changes before monitoring it for shutdown
     if(powerButton.hasChanged()) {
       timeButtonPressed = millis();
       if(!power_button_released) {
	    power_button_released = true;
       } else {
	    if (powerButton.isPressed()) {
		 // only call kill on the downstroke of the button
		 personalities[current_personality-1]->Kill(&bt);
		 pb = true; 
	    }
       }
     }
     
     if (power_button_released && powerButton.isPressed()){
       if (millis() - timeButtonPressed > POWEROFFHOLDDOWN){
           powerLED.off();
           indicateLED.off();
           beeper.yawn();
           digitalWrite(POWER_ON_HOLD,LOW);
           enterZombieMode();
       }
     }

     // check the battery - it is "low" if less than this amount

     lowBattery = (BATTERY_VOLTAGE < 65);		// 6.5 volts

     // check each joystick that is connected, and grab a packet of information from it if there is any
     if (g2.update(&vdip)) {
	  js2 = true;
	  personalities[current_personality-1]->ChangeInput(&bt, 2,&g2_prev,&g2);
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
	  if(vdip.deviceUpdate()) {
	       g1.deviceUpdate(&vdip);
	       g2.deviceUpdate(&vdip);
	  }
     }

     // check to see if we're connected to the robot - turn on the light if so if not connected, blink the thing
     if(bt.connected()) {
          if(inConfigMode){
            inConfigMode = false;
            software_Reset();          //does not initialize the IO lines, just resets master/slave pairing
          }

	  lowBattery?powerLED.fast():powerLED.on();
	  indicateLED.on();

	  if (!wasConnected) {
	       wasConnected = true;
	       beeper.squeep();
	  }
     } else {
	  if(!inConfigMode && lowBattery) {
	       powerLED.fast();
	  }
	  if (wasConnected) {
	      wasConnected = false;
	  }
	  indicateLED.slow();
     }

     personalities[current_personality-1]->Loop(&bt,theButton.isPressed(),&g1,&g2);
     
     //checks to see if we should enter a power saving mode (if 5 min has passed)
     if (js1 || js2 || wfs || pb){ //if something has happened, make note of the time since boot
        lastAnyAction = millis();
     }
     
     if (powerTimeout != 0 && millis() - lastAnyAction >= powerTimeout){
          beeper.yawn();
	  digitalWrite(POWER_ON_HOLD,LOW);
     }
     
     // update the state of the LEDs - this should always be done at the end of the loop
     powerLED.update();
     indicateLED.update();
     
     // the "5" allow only a certain number of updates - saves battery
     delay(5 + lag);

     loopCount++;
}
