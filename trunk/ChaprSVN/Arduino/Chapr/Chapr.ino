#include "config.h"
#include "VDIPSPI.h"
#include "VDIP.h"
#include "BT.h"
#include "blinky.h"
#include "sound.h"
#include "nxt.h"
#include "ChapRName.h"
#include "ChapREEPROM.h"
#include <EEPROM.h>
#include "button.h"
#include <avr/sleep.h>

// it is really strange, but BT.h won't compile correctly unless this .ino
// file also includes SoftwareSerial.h...really weird...

#include <SoftwareSerial.h>

// LIGHTS
//	- Power Light	- slow flash during boot
//			- solid when operational
//			- fast flash means battery is low
//			- off means the ChapR is off or battery dead
//
//	- BT Light	- off during boot
//			- solid when BT connected
//			- fast flash when discoverable (NEW BRICK MODE)
//			- slow flash when trying to connect to a brick (AUTO CONNECTION MODE)
//

#ifdef V01

#define BT_9600BAUD 	A5
#define BT_CONNECTED	A4
#define BT_MODE		A3
#define BT_RX		12
#define BT_TX		13
#define BT_RESET	11

#define VDIP_CLOCK	6
#define VDIP_MOSI	7
#define VDIP_MISO	8
#define VDIP_CS		9
#define VDIP_RESET	10

#define LED_POWER	A2
#define LED_INDICATE	A1

#define BUTTON		A0
#define TONEPIN		5

#endif

#ifdef V02

#define BT_9600BAUD 	5
#define BT_CONNECTED	3
#define BT_MODE		4
#define BT_RX		12
#define BT_TX		13
#define BT_RESET	11

#define VDIP_CLOCK	6
#define VDIP_MOSI	7
#define VDIP_MISO	8
#define VDIP_CS		9
#define VDIP_RESET	10

#define LED_POWER	A2
#define LED_INDICATE	A1

#define BUTTON		A0
#define TONEPIN		A3

#define POWER_ON_HOLD	A4
#define POWER_BUTTON	A5

#endif

blinky	powerLED(LED_POWER);
blinky	indicateLED(LED_INDICATE);

VDIP	 vdip(VDIP_CLOCK, VDIP_MOSI, VDIP_MISO, VDIP_CS, VDIP_RESET);
BT	 bt(BT_RX, BT_TX, BT_RESET, BT_MODE, BT_9600BAUD, BT_CONNECTED);

button theButton(BUTTON);
button powerButton(POWER_BUTTON,true);

bool    inConfigMode;

ChapRName myName; //myName() doesn't work because it thinks it's declaring a function with return type ChapRName
ChapREEPROM myEEPROM;

sound  	beeper(TONEPIN);

#define LOCAL_SERIAL_BAUD	38400

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
     
     //Serial.print("If you want to rename \"");
     //Serial.print(myName.get());
     //Serial.println("\", type the name followed by a return");

     // standard init stuff happens here

     // NOTE, though, that the object init from the VDIP and BT objects has already occured
     // which is good, because the VDIP takes some time to get up and running

     // check the button to see if it was pressed upon boot, if so, enter config mode

     if (digitalRead(BUTTON) == HIGH) {		// the button has a pull-down, so normally LOW
          bt.configMode(myName.get());
          inConfigMode = true;
	  powerLED.slow();
     } else {
	  bt.opMode();
          inConfigMode = false;
	  powerLED.on();
     }

     Serial.write("Waiting on VDIP sync\n");

     while(!vdip.sync()) {		// while waiting, update the LED status
	  powerLED.update();
     }

     Serial.write("Out of VDIP sync\n");

     vdip.deviceUpdate();		// get initial device setup

     beeper.confirm();
}

byte emptyJSData[] = { 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04, 0x00 };
byte joy1data[] = { 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04,0x00 };
byte joy2data[] = { 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04,0x00 };

void jsprint(byte *js,char *label)
{
	  Serial.print(label);
	  for (int i=0; i < 8; i++) {
	       Serial.print(js[i],HEX);
	       Serial.print(" ");
	  }
	  Serial.println("");
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

long lastAnyAction = 0;
long lastJSAction = 0;
bool isLowPower = false;

//these two constants define the time before entering power saving mode and are in milliseconds
// (the second commented out versions are for debugging)

#define LOWPOWERTIMEOUT 300000
//#define LOWPOWERTIMEOUT 10000
#define ZMODETIMEOUT 600000
//#define ZMODETIMEOUT 20000

#define DEVICE_UPDATE_LOOP_COUNT	50

void loop()
{
     static bool	power_button_released = false;
     static int		loopCount = 0;
     static bool	wasConnected = false;
     bool		js1 = false;
     bool		js2 = false;
     bool               wfs = false;

     //monitor for return from IDE to indicate a wish to change name
   
    //myName.setFromConsole();
    
    if (Serial.available() > 0){
      myEEPROM.setFromConsole(myEEPROM.getName(), myEEPROM.getTimeout(), myEEPROM.getPersonality());
    }
    
     // when we first boot, the power button is pressed in, so ensure that it changes before
     // monitoring it for shutdown.

     if(powerButton.hasChanged()) {
	  if(!power_button_released) {
	       power_button_released = true;
	  } else {
	       digitalWrite(POWER_ON_HOLD,LOW);
	  }
     }

     // check each joystick that is connected, and grab a packet of information from it
     // the joysticks return 8 bytes of info

     if (vdip.getJoystick(1,(char *)joy2data) == 8) {
	  js2 = true;
//	  jsprint(joy2data,"(2):");
     }

     if (vdip.getJoystick(0,(char *)joy1data) == 8) {
	  js1 = true;
//	  jsprint(joy1data,"(1):");
     }

     if (theButton.hasChanged()){
       wfs = true;
     }

     if((loopCount % DEVICE_UPDATE_LOOP_COUNT) == 0) {
	  vdip.deviceUpdate();
     }

     // check to see if we're connected to the brick - turn on the light if so
     // if not connected, blink the thing

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

     // if we're connected, send out a joystick update

     if(bt.connected() && (!isLowPower || wfs)) {
	  byte	outbuff[25];
	  int	size;
	  int	UserMode = 0;
	  int	StopPgm = (theButton.isPressed())?0:1;

	  size = nxtMsgCompose(outbuff,
			       UserMode,
			       StopPgm,
//			       (js1)?joy1data:emptyJSData,
//			       (js2)?joy2data:emptyJSData);
			       joy1data,
			       joy2data);
	  (void)bt.btWrite(outbuff,size);
     }
     
     //checks to see if we should enter a power saving mode (if 5 min has passed)
     
     if (js1 || js2 || wfs){ //if something has happened, make note of the time since boot
        lastAnyAction = millis();
     }
     if (js1 || js2) { //if a joystick changed
        lastJSAction = millis();
        isLowPower = false;
     }
     
     if (millis() - lastJSAction >= LOWPOWERTIMEOUT){
         isLowPower = true;
         powerLED.slow();
      }
     if (millis() - lastAnyAction >= ZMODETIMEOUT){
	  digitalWrite(POWER_ON_HOLD,LOW);
	  enterZombieMode();
     }
     
     // update the state of the LEDs - this should always be done at the end of the loop

     powerLED.update();
     indicateLED.update();
     
     // allow only a certain number of updates - saves battery
     
     delay(5);

     loopCount++;

}