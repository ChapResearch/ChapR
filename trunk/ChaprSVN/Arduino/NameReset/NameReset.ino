#include <Arduino.h>
#include <EEPROM.h>
#include "ChapRName.h"

ChapRName myName;

void setup()
{
  Serial.begin(38400);
  delay(100);
  Serial.println("");
  Serial.println("The current name of your ChapR is: ");
  Serial.println(myName.get());
  Serial.println("Enter the name for your ChapR: ");
  int index = 0;
  char buffer[15];
  while (true){
    if (Serial.available() > 0){
      byte val = Serial.read();
      if (val != '\r'){
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
  myName.setFromString(buffer);
}

void loop()
{
}
