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

void ChapRName::setFromConsole()
{
  Serial.println("Hit space then enter if you want to rename your ChapR.");
  char buffer[100];
  String name = "";
  static int index = 0;
  bool enteringName = false;
  if (Serial.available() > 0){
    int input = Serial.read();
    buffer[index] = input;
    if (buffer[index - 1] == ' '){
      Serial.write("Enter your new name (must be 1 - 20 characters without spaces): ");
      index = 0;
      enteringName = true;
    }
    else if (buffer[index] == '\r'){
     for (int i = 0; i <= index; i++){
       name = name + buffer[i];
     }
    Serial.println(name);
    Serial.println("Congrats! Your ChapR has been named " + name);
    }
    if (!enteringName){
    index++;
    }
  }
}

bool ChapRName::setFromFlashDrive()
{
}
