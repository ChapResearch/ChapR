#include <Arduino.h>
#include <EEPROM.h>
#include "ChapRName.h"

ChapRName::ChapRName()
{
}

char *ChapRName::read()
{
  static char buffer[15];
  
  for (int i = 0; i < 15; i++){
    buffer[i] = EEPROM.read(i);
  }
  
  return (buffer);
}

void ChapRName::write(char *name)
{
  int i = 0;
  do {
    EEPROM.write(i, name[i]);
    i++;
  } while (name[i] != '\0');
  
  while (i < 15){
    EEPROM.write(i, '\0');
    i++;
  }
}

char *ChapRName::get()
{
	return (read());
}

void ChapRName::setFromString(char *name)
{
        write(name);
}

bool ChapRName::setFromConsole()
{
  if (Serial.available() > 0){
      Serial.println("The current name of your ChapR is: ");
      char *currentName = get();
      Serial.println(currentName);
      int index = 0;
      char buffer[15];
      while (true){
        if (Serial.available() > 0){
          byte val = Serial.read();
          if (val != '\r' && index != 14){
            buffer[index] = val;
          } else {
            buffer[index] = '\0';
            break;
          }
          index++;
        }
      }
      Serial.print("Your ChapR is now named: ");
      Serial.println(buffer);
      setFromString(buffer);
      Serial.println("Hit return to accept, or type \"cancel\" (return) to cancel");
    
      index = 0;
      buffer[15];
        while (true){
          if (Serial.available() > 0){
            byte val = Serial.read();
          if   (val != '\r' && index != 14){
              buffer[index] = val;
            } else {
              buffer[index] = '\0';
              break;
            }
            index++;
          }
        }
      
      if (strcmp(buffer, "cancel") == 0) {
        setFromString(currentName);
        Serial.print("Your ChapR is now named: ");
        Serial.println(currentName);
      }
    }
}

bool ChapRName::setFromFlashDrive()
{
}
