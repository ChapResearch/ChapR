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

#include <SoftwareSerial.h>

SoftwareSerial mySerial(BT_RX, BT_TX); // RX, TX

void setup()  
{
  pinMode(BT_RESET, INPUT);
  digitalWrite(BT_RESET, LOW);
  pinMode(BT_RESET, OUTPUT);
  delay(500);
  pinMode(BT_RESET, INPUT);
  delay(1000);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}

