//
// VDIP_ROM_ASSIST.ino
//
//	This file implements a simple sketch that puts the ChapR (and VDIP) into a mode
//	that can be help with the ROM load.  The basic features of this program are:
//
//	- "attach" the WFS button to the /RESET of the VDIP - if the button is pressed, the
//	  VDIP /RESET line goes low - the connect LED is wired to the button too
//	- ensure that the AD0-AD3 lines are "input", allowing them to be driven by an
//	  externally attached driver.
//	- turn on power LED when power comes on and things are cool
//

//
// These defines come from the ChapR config file.  Some are reused with diff names.
// but reset).
//
#define VDIP_CLOCK	6
#define VDIP_MOSI	7
#define VDIP_MISO	8
#define VDIP_CS		9
#define VDIP_RESET	10
#define LED_POWER	A2
#define LED_INDICATE	A1
#define BUTTON		A0
#define POWER_ON_HOLD	A4		// (==>board) keeps the power on, lower to turn off

//
// here's the renamed ones
//
#define VDIP_RX		VDIP_MOSI
#define VDIP_TX		VDIP_CLOCK
#define VDIP_CTS	VDIP_CS
#define VDIP_RTS	VDIP_MISO

#include <arduino.h>

void setup()
{
     // turn on the power circuit

     pinMode(POWER_ON_HOLD,OUTPUT);
     digitalWrite(POWER_ON_HOLD,HIGH);

     // ensure that the UART pins for the VDIP are tri-state (input) so that they
     // can be driven by outside lines

     pinMode(VDIP_RX,INPUT);
     pinMode(VDIP_TX,INPUT);
     pinMode(VDIP_CTS,INPUT);
     pinMode(VDIP_RTS,INPUT);

     // the reset pin needs to be set as output, and high at first

     digitalWrite(VDIP_RESET,HIGH);
     pinMode(VDIP_RESET,OUTPUT);

     // turn on the power light so we know we're running

     pinMode(LED_POWER,OUTPUT);
     digitalWrite(LED_POWER,HIGH);

     // prepare the indicate light
     pinMode(LED_INDICATE,OUTPUT);
     digitalWrite(LED_INDICATE,LOW);

     // set-up to monitor the WFS button
     pinMode(BUTTON,INPUT);
}

void loop()
{
     // simply monitor the button, if pressed, issue a VDIP reset

     if(digitalRead(BUTTON) != LOW) {
	  digitalWrite(VDIP_RESET,LOW);
	  digitalWrite(LED_INDICATE,HIGH);
     } else {
	  digitalWrite(VDIP_RESET,HIGH);
	  digitalWrite(LED_INDICATE,LOW);
     }
}