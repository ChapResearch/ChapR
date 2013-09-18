#include <Arduino.h>
#include <EEPROM.h>
#include "ChapREEPROM.h"

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

void ChapREEPROM::setFromConsole(char *name, byte timeout, byte personality)
{
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
  Serial.println("Preferences saved!");
  markInitialized();
  setUSBPhase(0); //says the NXT has not been connected by USB yet
}

void ChapREEPROM::setString(int start, int length, char *thing)
{
  int i = 0;
  do {
    EEPROM.write(start, thing[i]);
    start++;
    i++;
  } while (thing[i] != '\0' && i < length + 1);
  
  while (i < EEPROM_MAXSTRINGLENGTH){
    EEPROM.write(start, '\0');
    start++;
    i++;
  }
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

