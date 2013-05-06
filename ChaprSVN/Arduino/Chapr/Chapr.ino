#include "VDIPSPI.h"
#include "VDIP.h"
#include "BT.h"
#include "blinky.h"
#include "sound.h"
#include "nxt.h"
#include "ChapRName.h"
#include <EEPROM.h>
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

VDIP	 vdip(VDIP_CLOCK, VDIP_MOSI, VDIP_MISO, VDIP_CS, VDIP_RESET);
BT	 bt(BT_RX, BT_TX, BT_RESET, BT_MODE, BT_9600BAUD, BT_CONNECTED);

blinky	powerLED(LED_POWER);
blinky	indicateLED(LED_INDICATE);

ChapRName myName; //myName() doesn't work because it thinks it's declaring a function with return type ChapRName

sound  	beeper(TONEPIN);

#define LOCAL_SERIAL_BAUD	38400

void setup()
{
     // standard init needs to occur here
     // NOTE though, that the init from the VDIP and BT objects has already occured

     Serial.begin(LOCAL_SERIAL_BAUD);	// the serial monitor operates at this BAUD
     Serial.write("ChapR v0.1 up!\n");
     Serial.println(myName.get());

     powerLED.slow();			// flash the power LED during boot
     
     // check the button to see if it was pressed upon boot, if so, enter config mode

     if (digitalRead(BUTTON) == HIGH) {		// the button has a pull-down, so normally LOW
          bt.configMode(myName.get());
	  powerLED.slow();
     } else {
	  bt.opMode();
	  powerLED.on();
     }
     vdip.reset(LED_POWER);		// reset and sync-up with the VDIP
     vdip.flush();			// consume incoming messages
     vdip.deviceUpdate();		// update device connections if necessary

     beeper.confirm();
}

byte emptyJSData[] = { 0x13, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04 };
byte joy1data[] = { 0x13, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04 };
byte joy2data[] = { 0x13, 0x80, 0x80, 0x80, 0x80, 0x08, 0x00, 0x04 };

void jsprint(byte *js,char *label)
{
	  Serial.print("(1):");
	  for (int i=0; i < 8; i++) {
	       Serial.print(joy1data[i],HEX);
	       Serial.print(" ");
	  }
	  Serial.println("");
}

void loop()
{
     static int		wasConnected = false;
//     unsigned char	joy1data[10];
//     unsigned char	joy2data[10];
     bool		js1 = false;
     bool		js2 = false;

     // check each joystick that is connected, and grab a packet of information from it
     // the joysticks return 8 bytes of info

     if (vdip.getJoystick(1,joy1data) == 8) {
	  js1 = true;
//	  jsprint(joy1data,"(1):");
     }

     delay(5);

     if (vdip.getJoystick(2,joy2data) == 8) {
	  js2 = true;
//	  jsprint(joy2data,"(2):");
     }

     delay(5);

     vdip.flush();
     vdip.deviceUpdate();	// update device connections if necessary

     // check to see if we're connected to the brick - turn on the light if so
     // if not connected, blink the thing

     if(bt.connected()) {
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

     

//     if (bt.connected() && (js1 || js2 || digitalRead(BUTTON) == HIGH)) {
     if(bt.connected()) {
	  byte	outbuff[25];
	  int	size;
	  int	UserMode = 0;
	  int	StopPgm = (digitalRead(BUTTON) == HIGH)?0:1;

	  size = nxtMsgCompose(outbuff,
			       UserMode,
			       StopPgm,
//			       (js1)?joy1data:emptyJSData,
//			       (js2)?joy2data:emptyJSData);
			       joy1data,
			       joy2data);
	  (void)bt.btWrite(outbuff,size);
     }

     // clear the joystick read data

     js1 = false;
     js2 = false;

     // update the state of the LEDs - this should always be here

     powerLED.update();
     indicateLED.update();

     // allow only a certain number of updates - saves battery

     delay(10);

}
     
