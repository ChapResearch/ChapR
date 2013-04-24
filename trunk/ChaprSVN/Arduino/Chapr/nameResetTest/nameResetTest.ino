void setup()
{
  Serial.begin(115200);
  //Serial1.begin(115200);
  Serial.println("Hit space then enter if you want to rename your ChapR.");
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
 resetName();
}
