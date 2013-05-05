#include "../ChapRName.h"

ChapRName myName;

void setup()
{
  Serial.begin(38400);
  Serial.println("Enter the name for your ChapR: ");
  int index = 0;
  char buffer[15];
  while (index < 15){
    if (Serial.available() > 0){
      buffer[index] = Serial.read)();
      index++;
    }
  }
  index[15] = '\0';
  myName.setFromString(buffer);
}

void resetName(){
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
    
    /* 
    Serial1.println("$$$");
    while (Serial1.read() != "CMD"){
    Serial1.prinln("$$$");
    delay(10);
    }
    Serial1.println("SN, " + name);
    delay(100);
    Serial1.println("R, 1");
    */
    
    Serial.println("Congrats! Your ChapR has been named " + name);
    }
    if (!enteringName){
    index++;
    }
  } else {
    return;
  }
    
}

void loop()
{
}
