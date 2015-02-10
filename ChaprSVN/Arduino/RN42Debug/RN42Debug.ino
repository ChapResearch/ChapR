#include <Arduino.h>
#include <SoftwareSerial.h>

//
// RN42debug - this program is meant to be loaded on to an ChapR
//		that has been fully populated.  Assuming that the
//		RN42 is soldered down correctly, and that the VDIP
//		is supplying the 3.3v like it should, this routine
//		will simply talk to the RN42 and ask it for its
//		version.  While it can't be read back by the Arduino,
//		it can be seen by a logic analyzer.
//
//	TO USE - to use this:
//		1. load it onto the ChapR
//		2. put the logic analyzer trigger on pin A7
//			(the pin above/between pins 13 and A0)
//		3. put a probe on pin D12 to listen to the RN42
//		4. This program will cycle the trigger, and
//		   generate the version every 5 seconds.
//

//
// Arduino pins defined
//
#define TRIGGER_PIN	A3
#define RN42_RX		13		// out to RN42
#define RN42_TX		12		// in from RN42 not used by this program
#define RN42_RESET	11		// there's a pull-up on this pin so it can stay as INPUT (floating)
#define RN42_9600	5		// the voltage divider will pull this to GND if this pin floating

SoftwareSerial	RN42Serial(RN42_TX,RN42_RX);


void RN42serialFlush()
{
     while(RN42Serial.available()) {
	  RN42Serial.read();
     }
}

void RN42reset()
{
     digitalWrite(RN42_RESET,LOW);
     pinMode(RN42_RESET,OUTPUT);
     delay(200);
     pinMode(RN42_RESET,INPUT);
     digitalWrite(RN42_RESET,HIGH);
     delay(1000);			// normally enough time for it to recover
}

void setup()
{
     pinMode(TRIGGER_PIN,OUTPUT);
     pinMode(RN42_RESET,INPUT);		// leaves it floating - therefore NOT in reset
     digitalWrite(RN42_RESET,HIGH);	// turns on pull-up
     pinMode(RN42_9600,OUTPUT);		// leaves it floating - therefore low, so NOT 9600 baud

     digitalWrite(RN42_9600,LOW);
     digitalWrite(TRIGGER_PIN,LOW);

     Serial.begin(38400);

//
// pick your favorite here
//

//     RN42Serial.begin(38400);
     RN42Serial.begin(115200);
//     RN42Serial.begin(9600);

     Serial.println(",--------------------------------,");
     Serial.println("|    RN42 Version Debugger       |");
     Serial.println("|                                |");
     Serial.println("| 100 ohm West - trigger (A3)    |");
     Serial.println("| D12 - 115k comm from RN42      |");
     Serial.println("| D13 - 115k comm to RN42        |");
     Serial.println("| LED North - GND                |");
     Serial.println("'--------------------------------'");
     Serial.println("");
     delay(400);

     Serial.print("Trying to reset RN42...");
     RN42reset();
     Serial.println("done (don't know if it reset, though).");

     Serial.print("Starting in 5...");
     delay(1000);
     Serial.print("4...");
     delay(1000);
     Serial.print("3...");
     delay(1000);
     Serial.print("2...");
     delay(1000);
     Serial.print("1...");
     delay(1000);
     Serial.println("GO!");

     digitalWrite(TRIGGER_PIN,HIGH);
     digitalWrite(TRIGGER_PIN,LOW);
     Serial.print("Initializing RN42...");
     delay(100);

     RN42Serial.write("$$$");
     delay(100);
     RN42Serial.write("+\r");
     delay(100);
     RN42serialFlush();
     delay(100);
     RN42Serial.write("+");
     delay(100);

     if(Serial.available() && Serial.read() == '+') {
	  Serial.println("in sync.");
     } else {
	  Serial.println("not necessarily sync'd.");
     }
}

void loop()
{
     Serial.println("-------------------------------");
     Serial.println("delay for 5 seconds...");
     delay(5000);

     
     Serial.print("Asking for version (and triggering)...");
     digitalWrite(TRIGGER_PIN,HIGH);
     digitalWrite(TRIGGER_PIN,LOW);

     RN42Serial.write("ver\r");

     Serial.println("done.");
}
