#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"
#include "blinky.h"
#include "config.h"
#include "VDIP.h"
#include "BT.h"
#include "button.h"
#include "debug.h"
#include "sound.h"

extern button theButton;

extern blinky powerLED;
extern blinky indicateLED;

extern VDIP vdip;

extern BT bt;

settings::settings()
{
}

bool settings::isInitialized()
{
  char *currentMagic = getString(EEPROM_MAGIC,EEPROM_MAGICLENGTH);
  return (strcmp(currentMagic, EEPROM_MAGICSTRING) == 0);
}

void settings::markInitialized()
{
  setString(EEPROM_MAGIC,EEPROM_MAGICLENGTH,EEPROM_MAGICSTRING);
}

void settings::setResetStatus(byte stat)
{
  EEPROM.write(EEPROM_RSTATUS, stat);
}

byte settings::getResetStatus()
{
  return EEPROM.read(EEPROM_RSTATUS);
}

void settings::flushSerial()
{
  while (Serial.available() > 0){
    if (Serial.read() == '\r'){
      break;
    }
  }
}

int settings::getStringFromMonitor(char *buffer, int size)
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

void settings::boardBringUp()
{
     extern void software_Reset();

  flushSerial();
  
  char buf[25];
  extern sound beeper;
  buf[0] = ' ';
  Serial.print(F("Running test program version "));
  Serial.println(BOARDBRINGUPVERSION);
  Serial.println(F("Don't forget to switch the top jumper on the VDIP"));
  indicateLED.off();
  Serial.println(F("Testing power LED..."));
  powerLED.on();
  Serial.println(F("Hit return to continue"));
  getStringFromMonitor(buf, 2);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 2);
  }
  Serial.println(F("Testing BT LED..."));
  powerLED.off();
  indicateLED.on();
  Serial.println(F("Hit return to continue"));
  getStringFromMonitor(buf, 1);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 1);
  }
  indicateLED.off();
  Serial.println(F("Hit return to squeep"));
  getStringFromMonitor(buf, 2);
  while (buf[0] != '\0'){
    getStringFromMonitor(buf, 2);
  }
  beeper.squeep();
  Serial.println(F("Press WFS button to continue"));
  while (theButton.check() != true){
  }

  while(true) {
       Serial.println(F("checking VDIP version..."));
       for (int i = 0; i < sizeof(buf); i++){
	    buf[i] ='\0';
       }
       vdip.cmd(VDIP_FWV, buf, DEFAULTTIMEOUT, 15); //expects 15 bytes back see pg 23 of Viniculum Firmware reference
       Serial.print(F("VDIP version: "));
       Serial.println(buf);
       Serial.println(F("Should be 3.69 (enter \"!\" to flash it now - press return to cont)"));

       getStringFromMonitor(buf, 25);
       if(buf[0] == '\0' || buf[0] != '!') {
	    break;		// if return or something other than !, go on with life
       }

       Serial.println(F("Put the flash drive with ftrfb.ftd on it in USB 2, and press RETURN."));
       getStringFromMonitor(buf, 25);

       Serial.println(F("Resetting VDIP to allow flash, 15 seconds delay..."));
       Serial.println(F("Don't do anything until after that..."));
       vdip.reset();
       delay(5000);
       vdip.flush(10000);
       Serial.println(F("Remove flash drive, and hit RETURN to retest VDIP version..."));
       getStringFromMonitor(buf, 25);
  }

  Serial.println(F("checking version of RN-42..."));
  bt.checkVersion();
  Serial.println(F("Should be 6.15"));
  Serial.println(F("Everything looks good!"));
}

void settings::setFromConsole(char *name, 
				 byte timeout, 
				 byte personality, 
				 byte speed, 
				 byte mode,
				 int autoLen,
				 int teleLen,
				 byte dgtl, 
				 float analog1,
				 float analog2,
				 float analog3,
				 float analog4)
{
  if (Serial.read() == '!'){
    boardBringUp();
  }
  
  flushSerial();
  
  Serial.print(F("Enter ChapR name (1 thru 15 chars). Hit return for default: "));
  Serial.println(name);
  int index = 0;
  char buffer[EEPROM_MAXSTRINGLENGTH];
  getStringFromMonitor(buffer, sizeof(buffer));
  if (buffer[0] == '\0'){ //checks to see if only a return was pressed
    setName(name);
  } else {  
    setName(buffer);
    Serial.print(F("ChapR name is now: "));
    Serial.println(getName());
  }
  
  Serial.print(F("Enter ChapR timeout (0 (no timeout) - 120 min).  Hit return for default: "));
  Serial.println(timeout);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0'){
    setTimeout(timeout);
  } else {
    setTimeout(atoi(buffer));
    Serial.print(F("ChapR timeout is now: "));
    Serial.println(getTimeout());
  }

  Serial.print(F("Enter ChapR personality (numbers explained on www.thechapr.com). Hit return for default: "));
  Serial.println(personality);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_LASTPERSON){
    setPersonality(personality);
  } else {
    setPersonality(atoi(buffer));
    Serial.print(F("ChapR personality is now: "));
    Serial.println(getPersonality());
  }
  
  Serial.print(F("Enter ChapR lag (0-255, 0 being the least lag). Hit return for default: "));
  Serial.println(speed);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_MAXLAG || atoi(buffer) < 0){
    setSpeed(speed);
  } else {
    setSpeed(atoi(buffer));
    Serial.print(F("ChapR lag is now: "));
    Serial.println(getSpeed());
  }
  
  Serial.print(F("Enter ChapR mode. Hit return for default: "));
  Serial.println(mode);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0' || atoi(buffer) > EEPROM_MAXMODE){
    setMode(mode);
  } else {
    setMode(atoi(buffer));
    Serial.print(F("ChapR mode is now: "));
    Serial.println(getMode());
  }
  
  Serial.print(F("Enter digital inputs (FRC only). Hit return for default: "));
  Serial.println(dgtl, BIN);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] == '\0'){
    setDigitalInputs(dgtl);
  } else {
    byte newNum = 0;
    char *ptr = buffer;
    for (int i = 0; i < 8; i++){
      byte val = (buffer[i] == '1')?1:0;
      newNum |= val<<i;
    }
    setDigitalInputs(newNum);
    Serial.print(F("ChapR digital inputs are now: "));
    Serial.println(getDigitalInputs(), BIN);
  } 

  for (int i = 0; i < 4; i++){
    Serial.print(F("Enter analog input "));
    Serial.print(i);
    Serial.print(F(" (FRC only). Hit return for default: "));
    switch (i){
    case 0: Serial.println(analog1); break;
    case 1: Serial.println(analog2); break;
    case 2:  Serial.println(analog3); break;
    case 3:  Serial.println(analog4); break;
    }
    getStringFromMonitor(buffer,sizeof(buffer));
    if (buffer[0] == '\0'){
          switch (i){
	  case 0: setAnalogInput(i, analog1); break;
	  case 1: setAnalogInput(i, analog2); break;
	  case 2: setAnalogInput(i, analog3); break;
	  case 3: setAnalogInput(i, analog4); break;
	  }
    } else {
      setAnalogInput(i, atof(buffer));
      Serial.print(F("ChapR analog input "));
      Serial.print(i);
      Serial.print(" is now: ");
      Serial.println(getAnalogInput(i, true)); //indicates value should be translated to be between 0 and 5
    }
  }
  
  Serial.print(F("Enter length of autonomous (in seconds). Hit return for default: "));
  Serial.println(autoLen);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] != '\0'){
    Serial.print("Autonomous length is now: ");
    Serial.println(atoi(buffer));
    setAutoLen(atoi(buffer));
  }

  Serial.print(F("Enter length of teleOp (in seconds). Hit return for default: "));
  Serial.println(teleLen);
  getStringFromMonitor(buffer,sizeof(buffer));
  if (buffer[0] != '\0'){
    Serial.print("TeleOp length is now: ");
    Serial.println(atoi(buffer));
    setTeleLen(atoi(buffer));
  }
  
  Serial.println(F("Preferences saved!"));
  markInitialized();
  setResetStatus(0); //makes sure the ChapR knows it has not been (software) reset
}

//
// setString() - given a string and a starting position, sets the string in EEPROM, up to
//               the maxLength, which is the size of the EEPROM area for that string - 1. This
//               means the maximum string that can be written is maxLength, because
//               maxLength does not include the null terminator.

void settings::setString(int start, int maxLength, char *thing)
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

//
// setShort() - writes in little endian, starting from the
//              index given
void settings::setShort(int start, short value)
{
  EEPROM.write(start,(byte) (value&0x00FF));
  EEPROM.write(start + 1, (byte) ((value&0xFF00)>>8));
}

//
// getShort() - read in little endian, starting from the
//              index given
short settings::getShort(int start)
{
  short value = 0x00FF&((short) EEPROM.read(start));
  value |= 0xFF00&(((short) EEPROM.read(start+1))<<8);
  return value;
}

char* settings::getString(int start, int length)
{
  int size = length;
  static char buffer[EEPROM_MAXSTRINGLENGTH];
  
  for (int i = 0; i < length + 1; i++){
    buffer[i] = EEPROM.read(start);
    start++;
  }
  
  return (buffer);
}

void settings::setName(char *name)
{
  setString(EEPROM_NAME,EEPROM_NAMELENGTH,name);
}

char* settings::getName()
{
  return getString(EEPROM_NAME,EEPROM_NAMELENGTH);
}

void settings::setTimeout(byte time)
{
  EEPROM.write(EEPROM_TIMEOUT, time);
}

byte settings::getTimeout()
{
  return (EEPROM.read(EEPROM_TIMEOUT));
}

void settings::setPersonality(byte p)
{
  EEPROM.write(EEPROM_PERSONALITY, p);
}

byte settings::getPersonality()
{
  return (EEPROM.read(EEPROM_PERSONALITY));
}

void settings::setSpeed(byte s)
{
  EEPROM.write(EEPROM_SPEED, s);
}

byte settings::getSpeed()
{
  return (EEPROM.read(EEPROM_SPEED));
}

void settings::setMode(byte m)
{
  EEPROM.write(EEPROM_MODE, m);
}

byte settings::getMode()
{
  return (EEPROM.read(EEPROM_MODE));
}

void settings::setDigitalInputs(byte d)
{
  EEPROM.write(EEPROM_DIGITALIN, d);
}

byte settings::getDigitalInputs()
{
  return (EEPROM.read(EEPROM_DIGITALIN));
}

void settings::setAnalogInput(byte index, float a)
{
  a = (a*1023)/5; // scales the value to be from 0 to 1023 (the way LabView wants it)
  setShort(EEPROM_ANALOGIN + index*2, a);
}

float settings::getAnalogInput(byte index, bool trans)
{
  if (trans){
    return ((float)getShort(EEPROM_ANALOGIN + index*2))*5/1023;}
  return getShort(EEPROM_ANALOGIN + index*2);
}

void settings::setAutoLen(byte a)
{
  EEPROM.write(EEPROM_AUTOLEN, a);
}

void settings::setTeleLen(byte t)
{
  EEPROM.write(EEPROM_TELELEN, t);
}

byte settings::getTeleLen()
{
  EEPROM.read(EEPROM_TELELEN);
}


byte settings::getAutoLen()
{
  EEPROM.read(EEPROM_AUTOLEN);
}


