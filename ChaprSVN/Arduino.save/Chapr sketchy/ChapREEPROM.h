#include "config.h"

#ifndef CHAPREEPROM_H
#define CHAPREEPROM_H

//Memory Map for Arduino EEPROM for ChapR
//
// bytes(inclusive)   description
// ------------------------------
// 0 - 15              null-terminated ChapR name
// 16                  minutes before timeout (0 means it never times out and the maximum is 255)
// 17                  personality (numbered based on information from the website)
// 18 - 23             null-terminated "ChapR" to verify EEPROM has been written

#define EEPROM_NAME             0
#define EEPROM_TIMEOUT         16
#define EEPROM_PERSONALITY     17
#define EEPROM_MAGIC           18
#define EEPROM_RSTATUS         24
#define EEPROM_SPEED           25 //23 - 33 is 0
#define EEPROM_MODE            26
#define EEPROM_DIGITALIN       27
#define EEPROM_ANALOGIN        28
#define EEPROM_AUTOLEN         36
#define EEPROM_TELELEN         37

//constants
#define EEPROM_NAMELENGTH      15 //without null terminator (15 - 0)
#define EEPROM_MAGICLENGTH      5 //without null terminator (23 - 18)
#define EEPROM_MAXSTRINGLENGTH (max(EEPROM_NAMELENGTH, EEPROM_MAGICLENGTH) + 1)
#define EEPROM_MAXTIMEOUT      120 //says timeout cannot be longer than two hours
#define EEPROM_LASTPERSON      PERSONALITYCOUNT //last personality that has been coded
#define EEPROM_MAXLAG          255
#define EEPROM_MAXMODE         1
#define EEPROM_MAGICSTRING   "Chap3" //number shows a new set of EEPROM settings

class ChapREEPROM
{
public:
     ChapREEPROM();

     void setName(char*);
     char *getName();
     void setTimeout(byte);
     byte getTimeout();
     void setPersonality(byte);
     byte getPersonality();
     void setSpeed(byte);
     byte getSpeed();
     void setMode(byte);
     byte getMode();
     bool isInitialized();
     void setFromConsole(char *name,byte timeout, byte personality, byte lag, byte mode, int auto, int tele, byte dgtlIn, float a1, float a2, float a3, float a4);
     void boardBringUp();
     void setResetStatus(byte);
     byte getResetStatus();
     void setDigitalInputs(byte);
     byte getDigitalInputs();
     void setAnalogInput(byte, float);
     float getAnalogInput(byte, bool = false);
     void setAutoLen(byte);
     void setTeleLen(byte);
     byte getTeleLen();
     byte getAutoLen();
     
private:
     int  getStringFromMonitor(char*, int);
     void setString(int, int, char*);
     char *getString(int, int);
     void markInitialized();
     void flushSerial();
     void setShort(int, short);
     short getShort(int);
};


#endif CHAPREEPROM_H
