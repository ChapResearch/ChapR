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

void setup()
{
  if (!myEEPROM.isInitialized()){
    Serial.println("Beginning board bring-up");
    myEEPROM.boardBringUp();
    Serial.println("Please intialize your ChapR.");
    myEEPROM.setDefaults(DEF_NAME, DEF_TIMEOUT, DEF_PERSON, DEF_LAG, DEF_MODE,
			 DEF_AUTOLEN, DEF_TELELEN, DEF_DGTLIN, DEF_ANALOG1, DEF_ANALOG2, DEF_ANALOG3, DEF_ANALOG4);
  }
}

void loop()
{
  if (Serial.available() > 0){
    myEEPROM.setFromConsole();
  }
}
