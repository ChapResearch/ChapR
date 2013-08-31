
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
#define EEPROM_NAMELENGTH      15 //without null terminator (15 - 0)
#define EEPROM_TIMEOUT         16
#define EEPROM_PERSONALITY     17
#define EEPROM_MAGIC           18
#define EEPROM_MAGICLENGTH      5 //without null terminator (23 - 18)
#define EEPROM_MAXSTRINGLENGTH (max(EEPROM_NAMELENGTH, EEPROM_MAGICLENGTH) + 1)
#define EEPROM_MAGICSTRING   "ChapR"

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
     bool isInitialized();
     void setFromConsole(char *,byte, byte);
     
private:
     int  getStringFromMonitor(char*, int);
     void setString(int, int, char*);
     char *getString(int, int);
     void markInitialized();
     void flushSerial();
};


#endif CHAPREEPROM_H
