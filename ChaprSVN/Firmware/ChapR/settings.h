#include "config.h"

#ifndef SETTINGS_H
#define SETTINGS_H

//Memory Map for Arduino EEPROM for ChapR
//
// bytes(inclusive)   description
// ------------------------------
// 0 - 15              null-terminated ChapR name
// 16                  minutes before timeout (0 means it never times out and the maximum is 255)
// 17                  personality (numbered based on information from the website)
// 18 - 23             null-terminated "ChapR" to verify EEPROM has been written

#define EEPROM_NAME             0	// 15 characters + null termination
#define EEPROM_TIMEOUT         16	// byte
#define EEPROM_PERSONALITY     17	// byte
#define EEPROM_MAGIC           18	// magic string to identify version
#define EEPROM_RSTATUS         24	// reset status used for self-rebooting
#define EEPROM_SPEED           25	// byte (lag)
#define EEPROM_MODE            26	// byte (bool) - (NXT definition) true(1) is teleop, false(0) is auto
// there is a long story as to why there are 10 bytes of space here...
#define EEPROM_AUTOLEN         36	// byte
#define EEPROM_TELELEN         37	// byte
#define EEPROM_ENDLEN          38       // byte
#define EEPROM_MATCHMODE       39       // byte (determines whether matchMode is enabled)

//constants
#define EEPROM_NAMELENGTH      15 //without null terminator (15 - 0)
#define EEPROM_MAGICLENGTH      5 //without null terminator (23 - 18)
#define EEPROM_MAXSTRINGLENGTH (max(EEPROM_NAMELENGTH, EEPROM_MAGICLENGTH) + 1)
#define EEPROM_MAXTIMEOUT      120 //says timeout cannot be longer than two hours
#define EEPROM_LASTPERSON      PERSONALITYCOUNT //last personality that has been coded
#define EEPROM_MAXLAG          255
#define EEPROM_MAXMODE         1
#define EEPROM_MAGICSTRING   "Chap3" //number shows a new set of EEPROM settings

class settings
{
public:
     settings();

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
     void setFromConsole();
     void boardBringUp();
     void setResetStatus(byte);
     byte getResetStatus();
     void setAutoLen(byte);
     void setTeleLen(byte);
     void setEndLen(byte);
     byte getEndLen();
     byte getTeleLen();
     byte getAutoLen();
     void setMatchModeEnable(int);
     bool matchModeIsEnabled();
     //     void setDefaults(char *,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int);
     void setDefaults(char *,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int, unsigned int, double, double, double, double, unsigned int);
     void loadCache();
     
 private:
     byte mode;              // copies of EEPROM settings (initialized during setup, then
     byte speed;             // updated every time EEPROM changes)
     byte personality;
     byte timeout;
     long autoLen;             
     long teleLen; 
     long endLen;
     int matchModeEnable; // 0 is false, 1 is true

     void hitReturn();
     void doSetting(int,const __FlashStringHelper *, const __FlashStringHelper *,unsigned int,unsigned int,uint8_t);
     void printCurrentValue(int,unsigned int, uint8_t);
     int  getStringFromMonitor(char*, int);
     void setString(int, int, char*);
     char *getString(int, int);
     void markInitialized();
     void flushSerial();
     void setShort(int, short);
     short getShort(int);
     void hitReturnForDefault();
};


#endif SETTINGS_H
