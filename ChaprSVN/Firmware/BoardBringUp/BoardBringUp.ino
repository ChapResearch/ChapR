#include <Arduino.h>
#include <EEPROM.h>
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


void flushSerial()
{
  while (Serial.available() > 0){
    if (Serial.read() == '\r'){
      break;
    }
  }
}

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

     myEEPROM.setDefaults(DEF_NAME, DEF_TIMEOUT, DEF_PERSON, DEF_LAG, DEF_MODE,
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


//
// getStringFromMonitor() - read a string from the serial monitor.  The data
//			    is put into the given buffer.  The given size is
//			    the size of the BUFFER not of the string.  The
//			    string will have a '\0' termination so the maximum
//			    string length will be size - 1.
//
//			    The neat thing about this routine is that you can
//			    use a real small buffer and it will only read that
//			    number of characters from the serial monitor and
//			    just flush the rest...very efficient!
//
int getStringFromMonitor(char *buffer, int size)
{
  int index = 0;
  char incoming;
  
  while (index < size){			// only loop through for "size" loops
       incoming = '\0';
       if (Serial.available() > 0){
	    incoming = Serial.read();
	    if (incoming == '\r'){	// we stop when the user hits return
		 buffer[index] = '\0';
		 return index;
	    }
	    buffer[index++] = incoming;
       }
  }
  buffer[index - 1] = '\0';		// or we stop when we've run out of buffer
  flushSerial();			// need to flush until we get the return in this case
  return size;
}


void hitReturn()
{
     char buf[2];
     Serial.println(F("Hit RET to continue"));
     getStringFromMonitor(buf,2);
}


void boardBringUp()
{
     char buf[30];		// needs enough space for the return from VDIP version

     extern void software_Reset();

     Serial.begin(LOCAL_SERIAL_BAUD);	// the serial monitor operates at this BAUD

     flushSerial();

     buf[0] = ' ';

     Serial.println(F("------ Begin Board Bring-up --------"));
     Serial.print(F("Test prog v"));
     Serial.println(BOARDBRINGUPVERSION);

     // testing the power LED

     indicateLED.off();
     powerLED.on();
     Serial.print(F("Power LED is on..."));
     hitReturn();

     // testing the indicator (blue) LED

     indicateLED.on();
     powerLED.off();
     Serial.print(F("BT LED is on..."));
     hitReturn();

     // testing the speaker
     
     indicateLED.off();
     Serial.print(F("Ready to SQUEEP! "));
     hitReturn();
     beeper.squeep();

     // testing the Action button

     Serial.println(F("Press the ACTION button to continue..."));
     while (theButton.check() != true){
     }

     // Check communication to the VDIP and its version

     while(true) {
	  Serial.print(F("VDIP version(3.69): "));
	  for (int i = 0; i < sizeof(buf); i++){
	       buf[i] ='\0';
	  }
	  vdip.cmd(VDIP_FWV, buf, DEFAULTTIMEOUT, 15); //expects 15 bytes back see pg 23 of Viniculum Firmware reference
	  Serial.println(buf);
	  Serial.print(F("Enter \"!\" to flash now or RET to continue"));
	  getStringFromMonitor(buf,2);

	  if(buf[0] == '\0' || buf[0] != '!') {
	       break;		// if return or something other than !, go on with life
	  }

	  Serial.println("");
	  Serial.println(F("Put flash in USB 2;"));
	  hitReturn();

	  Serial.println(F("15 sec delay (don't do stuff)..."));
	  vdip.reset();
	  delay(5000);
	  vdip.flush(10000);
	  Serial.print(F("Remove flash; "));
	  hitReturn();
     }

     // check communication to the RN-42 and its version

     Serial.println("");
     Serial.print(F("RN-42 version (should be 6.15): "));
     if(!bt.checkVersion()) {
	  Serial.println("Connection to RN-42 failed.");
     }

     Serial.println(F("--------------- Done ----------------"));
}
