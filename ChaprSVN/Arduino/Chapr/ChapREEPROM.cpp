#include <Arduino.h>
#include <EEPROM.h>
#include "ChapREEPROM.h"
#include "blinky.h"
#include "config.h"
#include "VDIP.h"
#include "BT.h"
#include "button.h"
#include "debug.h"
#include "sound.h"

button	theButton2(BUTTON);

blinky	powerLED2(LED_POWER);
blinky	indicateLED2(LED_INDICATE);

VDIP	vdip2(VDIP_CLOCK, VDIP_MOSI, VDIP_MISO, VDIP_CS, VDIP_RESET);

BT	bt2(BT_RX, BT_TX, BT_RESET, BT_MODE, BT_9600BAUD, BT_CONNECTED);

ChapREEPROM::ChapREEPROM()
{
}

bool ChapREEPROM::isInitialized()
{
  char *currentMagic = getString(EEPROM_MAGIC,EEPROM_MAGICLENGTH);
  return (strcmp(currentMagic, EEPROM_MAGICSTRING) == 0);
}

void ChapREEPROM::markInitialized()
{
  setString(EEPROM_MAGIC,EEPROM_MAGICLENGTH,EEPROM_MAGICSTRING);
}

void ChapREEPROM::setUSBPhase(byte phase)
{
  EEPROM.write(EEPROM_USBPHASE, phase);
}

byte ChapREEPROM::getUSBPhase()
{
  return EEPROM.read(EEPROM_USBPHASE);
}

void ChapREEPROM::flushSerial()
{
  while (Serial.available() > 0){
    if (Serial.read() == '\r'){
      break;
    }
  }
}

int ChapREEPROM::getStringFromMonitor(char *buffer, int size)
{
  int index = 0;
  char incoming;
  
  while (index < size){
    if (Serial.available() > 0){
      incoming = Serial.read();
    } else {
      incoming = 0;
    }
    if (incoming){
      if (incoming == '\r'){
        buffer[index] = '\0';
        return index;
      }
      buffer[index++] = incoming;
    }
  }
  buffer[index - 1] = '\0';
  flushSerial();
  return size;
}

void ChapREEPROM::boardBringUp()
{
  flushSerial();
  
  char buf[25];
  extern sound beeper;
  buf[0] = ' ';
  Serial.print("Running test program version ");
  Serial.println(BOARDBRINGUPVERSION);
  Serial.println("Don't forget to switch the top jumper on the VDIP");
  indicateLED2.off();
  Serial.println("Testing power LED...");
  powerLED2.on();
  Serial.println("Hit return to continue");
  getStringFromMonitor(buf, 2);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 2);
  }
  Serial.println("Testing BT LED...");
  powerLED2.off();
  indicateLED2.on();
  Serial.println("Hit return to continue");
  getStringFromMonitor(buf, 1);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 1);
  }
  indicateLED2.off();
  Serial.println("Hit return to squeep");
  getStringFromMonitor(buf, 2);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 2);
  }
  beeper.squeep();
  Serial.println("Press WFS button to continue");
  while (theButton2.isPressed() != true){
  }
  Serial.println("checking VDIP version...");
  for (int i = 0; i < sizeof(buf); i++){
    buf[i] ='\0';
  }
  vdip2.cmd(VDIP_FWV, buf, DEFAULTTIMEOUT, 15); //expects 15 bytes back see pg 23 of Viniculum Firmware reference
  Serial.print("VDIP version: ");
  Serial.println(buf);
  Serial.println("checking version of RN-42...");
  bt2.checkVersion();
  Serial.println("Everything looks good!");
}

void ChapREEPROM::setFromConsole(char *name, byte timeout, byte personality, byte speed, byte mode)
{
  if (Serial.read() == '!'){
    boardBringUp();
  }
  
  flushSerial();
  
  Serial.print("Enter ChapR name (1 thru 15 chars). Hit return for default: ");
  Serial.println(name);
  int index = 0;
  char buffer[EEPROM_MAXSTRINGLENGTH];
  getStringFromMonitor(buffer, sizeof(buffer));
  if (buffer[0] == '\0'){ //checks to see if only a return was pressed
    setName(name);
  } else {  
    setName(buffer);
    Serial.print("ChapR name is now: ");
    Serial.println(getName());
  }
  
  Serial.print("Enter ChapR timeout (0 (no timeout) - 120 min).  Hit return for default: ");
  Serial.println(timeout);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0'){
    setTimeout(timeout);
  } else {
    setTimeout(atoi(buffer));
    Serial.print("ChapR timeout is now: ");
    Serial.println(getTimeout());
  }

  Serial.print("Enter ChapR personality (numbers explained on www.thechapr.com). Hit return for default: ");
  Serial.println(personality);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_LASTPERSON){
    setPersonality(personality);
  } else {
    setPersonality(atoi(buffer));
    Serial.print("ChapR personality is now: ");
    Serial.println(getPersonality());
  }
  
  Serial.print("Enter ChapR lag (0-255, 0 being the least lag). Hit return for default: ");
  Serial.println(speed);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_MAXLAG || atoi(buffer) < 0){
    setSpeed(speed);
  } else {
    setSpeed(atoi(buffer));
    Serial.print("ChapR lag is now: ");
    Serial.println(getSpeed());
  }
  
  Serial.print("Enter ChapR mode. Hit return for default: ");
  Serial.println(mode);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_MAXMODE){
    setMode(mode);
  } else {
    setMode(atoi(buffer));
    Serial.print("ChapR mode is now: ");
    Serial.println(getMode());
  }
  
  Serial.println("Preferences saved!");
  markInitialized();
  setUSBPhase(0); //says the NXT has not been connected by USB yet
}

//
// setString() - given a string and a starting position, sets the string in EEPROM, up to
//               the maxLength, which is the size of the EEPROM area for that string - 1. This
//               means the maximum string that can be written is maxLength, because
//               maxLength does not include the null terminator.

void ChapREEPROM::setString(int start, int maxLength, char *thing)
{
  int i = 0;
  while(maxLength--){
    EEPROM.write(start, thing[i]);
    start++;
    if (thing[i] != '\0'){
      i++;
    }
  }
  EEPROM.write(start, '\0'); //null terminates the string
}

char* ChapREEPROM::getString(int start, int length)
{
  int size = length;
  static char buffer[EEPROM_MAXSTRINGLENGTH];
  
  for (int i = 0; i < length + 1; i++){
    buffer[i] = EEPROM.read(start);
    start++;
  }
  
  return (buffer);
}

void ChapREEPROM::setName(char *name)
{
  setString(EEPROM_NAME,EEPROM_NAMELENGTH,name);
  markInitialized();
}

char* ChapREEPROM::getName()
{
  return getString(EEPROM_NAME,EEPROM_NAMELENGTH);
}

void ChapREEPROM::setTimeout(byte time)
{
  EEPROM.write(EEPROM_TIMEOUT, time);
  markInitialized();
}

byte ChapREEPROM::getTimeout()
{
  EEPROM.read(EEPROM_TIMEOUT);
}

void ChapREEPROM::setPersonality(byte p)
{
  EEPROM.write(EEPROM_PERSONALITY, p);
  //markInitialized();
}

byte ChapREEPROM::getPersonality()
{
  EEPROM.read(EEPROM_PERSONALITY);
}

void ChapREEPROM::setSpeed(byte s)
{
  EEPROM.write(EEPROM_SPEED, s);
}

byte ChapREEPROM::getSpeed()
{
  EEPROM.read(EEPROM_SPEED);
}

void ChapREEPROM::setMode(byte m)
{
  EEPROM.write(EEPROM_MODE, m);
}

byte ChapREEPROM::getMode()
{
  EEPROM.read(EEPROM_MODE);
}
