#include <VDIPSPI.h>
#include <VDIP.h>
#include <BT.h>

// it is really strange, but BT.h won't compile correctly unless this .ino
// file also includes SoftwareSerial.h...really weird...

#include <SoftwareSerial.h>

//
// The VDIP is set-up to use 5 lines for control, 4 of which are for
//   the SPI interface and one for reset:
//
//     CLOCK - the SPI clock (output)
//     MOSI - master out slave in (output)
//     MISO - master in slave out (intput)
//     CS - chip select
//     RESET - VDIP reset pin
//
//   These pins are given to the VDIP constructor as follows in order:
//
//     vdip(CLOCK, MOSI, MISO, CS, RESET)
//  
//
// The bluetooth module uses a UART for communication along with a reset
// line.  They are given in the order:
//
//     bt(RX,TX, RESET)
//

VDIP	 vdip(6,7,8,9,10);
BT	 bt(12,13,11);

#define POWERLED	A2
#define INDICATELED	A1
#define SWITCH		A0
#define TONEPIN		5


void confirmTone(int pin)
{
  tone(pin,440,50);
  delay(50);
  tone(pin,880,50);
}

void squeepTone(int pin)
{
     noTone(pin);
     for (int i = 880; i < 3520; i += 20) {
	  tone(pin,i);
	  delay(1);
     }
     for (int i = 3520; i > 880; i -= 20) {
	  tone(pin,i);
	  delay(1);
     }
     noTone(pin);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("hola");
  noTone(11);			// turn off default tone

  pinMode(POWERLED,OUTPUT);	// flashing during reset
  pinMode(INDICATELED,OUTPUT);	// flashing during reset

  digitalWrite(POWERLED,HIGH);

  // first, reset the BT - it takes a little bit to settle so start it before
  //  the VDIP reset, then sync-up afterwards

  Serial.println("doing BT reset");
  bt.reset();

  vdip.reset(POWERLED);		// reset and sync-up with the VDIP
  vdip.flush();			// consume incoming messages
  vdip.deviceUpdate();		// update device connections if necessary

  Serial.println("doing BT sync");
  bt.sync();

  digitalWrite(POWERLED,HIGH);

//  squeepTone(TONEPIN);
  confirmTone(TONEPIN);

  digitalWrite(INDICATELED,HIGH);

  Serial.println("just reset");

  // bt.setup();      // reset and sync-up with the BT module

}

void loop()
{
     unsigned char	joy1data[100];
     unsigned char	joy2data[8];

     // check each joystick that is connected, and grab a packet of information from it
     // the joysticks return 8 bytes of info

     if (vdip.getJoystick(1,joy1data) == 8) {
	  Serial.print("(1):");
	  for (int i=0; i < 8; i++) {
	       Serial.print(joy1data[i],HEX);
	       Serial.print(" ");
	  }
	  Serial.println("");
     }

     delay(30);

     if (vdip.getJoystick(2,joy2data) == 8) {
	  Serial.print("(2):");
	  for (int i=0; i < 8; i++) {
	       Serial.print(joy2data[i],HEX);
	       Serial.print(" ");
	  }
	  Serial.println("");
     }

     delay(30);

     vdip.flush();
     vdip.deviceUpdate();	// update device connections if necessary

//     Serial.print(".");
}
     