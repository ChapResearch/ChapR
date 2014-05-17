#include <EEPROM.h>;
#include "ChapRName.h";

ChapRName name;

void setup() {
  bool didItWork = name.setFromString("test");
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
