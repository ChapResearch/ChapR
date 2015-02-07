#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 11); // RX, TX (swapped from the docs version)

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("Hi moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.write("VPF pirate.mp3");
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
