#include <Arduino.h>
#include <SoftwareSerial.h>
#include "settings.h"
#include "button.h"
#include "blinky.h"
#include "VDIP.h"
#include "BT.h"
#include "sound.h"

blinky	powerLED(LED_POWER);
blinky	indicateLED(LED_INDICATE);

sound	beeper(TONEPIN);

VDIP	vdip(VDIP_CLOCK, VDIP_MOSI, VDIP_MISO, VDIP_CS, VDIP_RESET);
BT	bt(BT_RX, BT_TX, BT_RESET, BT_MODE, BT_9600BAUD, BT_CONNECTED);

button	theButton(BUTTON);

settings myEEPROM;

extern void boardBringUp();	// actually defined below setup down there...

void setup()
{
     // bring up the Board!  This basically runs the user through a checking
     // process to see if the functions on the board are working.

     boardBringUp();

     // initialize EEPROM, not worrying about whether it's been initialized or not
     //
     // NOTE - ChapR.ino does this too, but that code "should" never be run because
     //   every board should go through THIS bring-up process first.

     Serial.println("Please intialize your ChapR.");

     myEEPROM.setDefaults(DEF_NAME, DEF_TIMEOUT, DEF_PERSON, DEF_LAG,
			  DEF_AUTOLEN, DEF_TELELEN, DEF_ENDLEN,
			  DEF_DGTLIN, DEF_ANALOG1, DEF_ANALOG2, DEF_ANALOG3, DEF_ANALOG4);
     myEEPROM.setFromConsole();
}

void loop()
{
  if (Serial.available() > 0){
    myEEPROM.setFromConsole();
  }
}


void hitReturn()
{
     Serial.println(F("Hit RET to continue"));
}


void boardBringUp()
{
     char buf[25];

     extern void software_Reset();

     Serial.println("Beginning board bring-up");

     flushSerial();

     buf[0] = ' ';
     Serial.print(F("Test prog v"));
     Serial.println(BOARDBRINGUPVERSION);

     // testing the power LED

     indicateLED.off();
     powerLED.on();
     Serial.println(F("Power LED is on..."));
     hitReturn();

     // testing the indicator (blue) LED

     indicateLED.on();
     powerLED.off();
     Serial.println(F("BT LED is on..."));
     hitReturn();

     // testing the speaker
     
     indicateLED.off();
     Serial.println(F("Ready to SQUEEP!"));
     hitReturn();
     beeper.squeep();

     // testing the Action button

     Serial.println(F("Press the ACTION button to continue..."));
     while (theButton.check() != true){
     }

     // Check communication to the VDIP and its version

     while(true) {
	  Serial.println(F("VDIP version(3.69)...?"));
	  for (int i = 0; i < sizeof(buf); i++){
	       buf[i] ='\0';
	  }
	  vdip.cmd(VDIP_FWV, buf, DEFAULTTIMEOUT, 15); //expects 15 bytes back see pg 23 of Viniculum Firmware reference
	  Serial.print(F("v"));
	  Serial.println(buf);
	  Serial.print(F("Enter \"!\" to flash now"));
	  hitReturn();

	  getStringFromMonitor(buf, 25);
	  if(buf[0] == '\0' || buf[0] != '!') {
	       break;		// if return or something other than !, go on with life
	  }

	  Serial.println(F("Put flash in USB 2; press RETURN."));
	  getStringFromMonitor(buf, 25);

	  Serial.println(F("15 sec delay (don't do stuff)..."));
	  vdip.reset();
	  delay(5000);
	  vdip.flush(10000);
	  Serial.print(F("Remove flash; "));
	  hitReturn();
	  getStringFromMonitor(buf, 25);
     }

     // check communication to the RN-42 and its version

     Serial.println(F("RN-42 version (should be 6.15)...?"));
     bt.checkVersion();

     Serial.println(F("Done."));
}
