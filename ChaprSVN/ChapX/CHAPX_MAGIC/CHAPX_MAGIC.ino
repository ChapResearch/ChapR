#include "mpr121.h"
#include <Wire.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(12, 11); // RX, TX (flipped from docs)

int irqpin = 2;  // Digital 2
boolean touchStates[12]; //to keep track of the previous touch states
int RTS = 0;

void setup(){
  pinMode(irqpin, INPUT);
  digitalWrite(irqpin, HIGH); //enable pullup resistor
  
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);
  
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();

  Serial.println("It's magic, you know...");

  mpr121_setup();
}

////////////////////////////////////////////////////////////////////////////////////////
//ALL FRUIT MUST BE PUT ON THE PINS WITH NUMBERS THE SAME AS THEIR SOUND FILE NUMMBERS//
////////////////////////////////////////////////////////////////////////////////////////
#define NUMFRUITS 2
int gameType = -1; //1-classic,2-Simon,3-team
boolean pressStart = true;


//team

//

// g0 presstoplay g1 goodsound g2 badsound g3 newhighscore g4 your score is  g5 wah wah wah waaaaaaahhh g6 silence
void loop(){
    if(pressStart){
        Serial.print("Press and hold any fruit to continue"); //play pressmesound
        playPressAndHold();
        delay(10000);
        gameType = readTouchInputs() + 1;
        if(gameType != 0){
          // if anything is touched by this time
            pressStart = false;
            Serial.print(gameType);
        }
    }else{
        if(gameType == 1){
          doClassic();
        }
        if(gameType == 2){
          doSimon();
        }
        if(gameType == 3){
          doTeam();
        }
    }
    if(gameType == 1){
      checkClassic();
    }
    if(gameType == 2){
      checkSimon();
    }
    if(gameType == 3){
      checkTeam();
    }
    if (mySerial.available())
      Serial.write(mySerial.read());
    if (Serial.available())
      mySerial.write(Serial.read());
}
// clssic
int hardness = 10000; //ten seconds to touch
long score = 0;
int strikes = 0;
boolean win = true; 
int pinTouched;
int highscore = 1;
//
void doClassic(){
  bool strikeDone = false; 
  int randNum = (int)random(0,NUMFRUITS);
  playPrompt(randNum); //play prompt sound
  long start = millis(); 
  int addScore = 0;
  long touchWindow = start + hardness; //reaction time
  while(millis() < touchWindow){
     pinTouched = readTouchInputs();
     if(pinTouched == randNum){
     playSound(randNum); //play fruit sound
     delay(2000);
     int timeBonus = ((touchWindow - millis())%100)*100 - (hardness/1000 +4) * 100 ;
          
              if(hardness > 1000){
                   hardness = hardness-1000;
                   addScore = 100 + timeBonus - 500;
                   Serial.print("Score: ");
                   Serial.println(score);
                   Serial.println(hardness);
              }else{
                  win = true;
                  addScore = 500;
              }
              
              break;
         } 
         else if(pinTouched != randNum && pinTouched != -1){
           strikes++;
           Serial.print("You now have ");
           Serial.print(strikes);
           playNoise('g',2);
           delay(2000);
           Serial.println(" strikes. :(");
           strikeDone = true;
           break;
         }
         
      }
      if(addScore==0 && strikeDone == false){
        strikes++;
        Serial.print("You now have ");
        Serial.print(strikes);
        Serial.println(" strikes. :(");
        playNoise('g',2);
        delay(2000);
        strikeDone = true;

      }else{
        score = score + addScore;
      }
}
//simon
int promptList[20];
int level = 1;
int strikes1 = 0;
bool right = true;
bool levelUp = false;
int highScore1 = 0;
//
void doSimon(){
  
  int randNum1 = (int)random(0,NUMFRUITS);
  Serial.print(randNum1);
  promptList[level-1] = randNum1;
  playPrompt(promptList[level-1]);
  Serial.print("play prompt");
  Serial.println(promptList[level-1]);
  letUserTry();
  while(!levelUp && strikes1 < 6){
    letUserTry();
    delay(1000);
  }
  delay(1000);
}
void letUserTry(){
  int checker = 0;
  Serial.println(promptList[checker]);
  while(right && checker < level){
    if(readTouchInputs() == promptList[checker]){
       checker = checker + 1;
       Serial.println("CORRECT");
       delay(1000);
    }  
    else if(readTouchInputs() != promptList[checker] && readTouchInputs() != -1){
       right =  false;
       Serial.println("WRONG");
       strikes1++;
       delay(1000);
    }   
  }
  if(right)
  {
    levelUp = true;
  }
}
void doTeam(){
  
}

void checkClassic(){
   if(strikes > 3){
      if(score > highscore){   
        playNoise('g', 5);//game over
        delay(6000);
        Serial.println("BOO! You SUCK at this game. But you got a highscore????"); //play meh
        Serial.print("New highscore: "); //play sound score
        playNoise('g',3);//your new high score is
        delay(5000);
        delay(6000);
        Serial.println(score);
        playScore(score);//score
        highscore = score;
        newGameSet();
      }else{
        playNoise('g', 5); //game over
        delay(6000);
        Serial.println("BOO! You SUCK at this game."); //play badsound
        Serial.print("Score: "); //play sound score
        playNoise('g', 4);
         delay(11000);
        Serial.println(score);
        playScore(score);  //play sound score
        newGameSet();
      }
    }
    if(win){
      if(score > highscore){
        Serial.println("WOOT!! You WIN at this game. AND you got a highscore!"); //play goodsound
        Serial.print("New highscore: ");
        playNoise('g',1); //you win
        delay(5000);
        Serial.println(score);
        playNoise('g',3); //your new high score is
        delay(2000);
        playScore(score); //play sound score
        highscore = score;
        newGameSet();
        delay(500);
      }else{
        Serial.println("WOOT!! You WIN at this game."); //play goodsound
        Serial.print("Score: ");
        playNoise('g',1); //you win
        delay(2000);
        playScore(score);
        Serial.println(score);
        newGameSet();
        delay(500);
      }
    }
    
    
}

void checkSimon(){
  if(strikes1 == 6){
    if(level > highScore1){  
      playNoise('g', 5);//game over
      Serial.println("You got too many wrong! Game Over. But you got a highscore?");
      Serial.print("Score: ");
      Serial.print(level);
      highScore1 = level;
      pressStart = true;
      delay(6000);
    }else{
      Serial.println("You got too many wrong! Game Over.");
      Serial.print("Score: ");
      Serial.print(level);
      pressStart = true;
      playNoise('g', 5);//game over
      delay(6000);
    }
  }
  if(level == 20){
    if(level > highScore1){  
      playNoise('g',1); //you win
      Serial.println("YAY you win. AND you got a highscore");
      Serial.print("Score: ");
      delay(5000);
      Serial.print(level);
      playNoise('g',3); //your new high score is
      delay(2000);
      highScore1 = level;
      pressStart = true;
    }else{
      Serial.println("YAY you win!");
      Serial.print("Score: ");
      playNoise('g',1); //you win
      delay(2000);
      playScore(score);
      Serial.println(level);
      delay(500);
      pressStart = true;
    }
  }
  if(levelUp){
    level++;
    right = true;
    levelUp = false;
  }
}

void checkTeam(){
  
}

void playScore(long score)
{
  while(mySerial.available()){
  Serial.print("Message from vmusic: ");
  Serial.println((char) mySerial.read());
  delay(20);
  }
  int scoreDigits[6];
  int i = 0;
  score = 569000;
  Serial.print(score);
  while(score >0 && i <6){
    int temp = (int)(score%10);
    Serial.print(temp);
    delay(500);
    score = score/10;
    scoreDigits[i] = temp;
    i++;
  }
  if(!(scoreDigits[5]>=0 && scoreDigits[5]<10)){
    scoreDigits[5] = 0;
  }
  
 
  //test
  for(int i =0; i < 6; i++)
  {
    Serial.println(i);
    
    
    mySerial.write("VPF ");
    Serial.print("VPF ");
    mySerial.write((char)((scoreDigits[i])+48));
    Serial.print(scoreDigits[i]+48);
    mySerial.write(".mp3\r");
    Serial.print(".mp3\r");
    delay(1000);
  }
  
  for(int i = 5; i > -1; i--){
    if(scoreDigits[i] == 0){
      playNoise('g', 6);
    }else{
          if(i == 5){ //fist digit
          //play number + hundred
          Serial.print(scoreDigits[i]);// it's getting mixed up here
          delay(2000);
          mySerial.write("VPF ");
          mySerial.write((char)(scoreDigits[i]+48));
          mySerial.write(".mp3\r");
          delay(500);
          mySerial.write("VPF 100.mp3\r");
          }
        
          if(i == 4) // second digit
          {
          //play number + 0 to play sound like "sixty"
          Serial.print(scoreDigits[i]);
          delay(2000);
          mySerial.write("VPF ");
          mySerial.write((char)(scoreDigits[i] + 48));
          mySerial.write("0.mp3\r");
          Serial.print("Yay");
          //mySerial.write(temp);
          //mySerial.write('0');
          //mySerial.write(".mp3\r");
          } 
        
         if(i == 3) // third digit
         {
          //play number + thousand
          Serial.print(scoreDigits[i]);
          delay(2000);
          mySerial.write("VPF ");
          mySerial.write((char)(scoreDigits[i]+48));
          mySerial.write(".mp3\r");
          delay(500);
          mySerial.write("VPF 1000.mp3\r ");
        }
        
         if(i == 2) // fourth digit
         {
          //play number + hundred
          Serial.print(scoreDigits[i]);
          delay(5000); 
          mySerial.write("VPF ");
          mySerial.write((char)(scoreDigits[i]+48));
          mySerial.write(".mp3\r");
          delay(500);
          mySerial.write("VPF 100.mp3\r ");
        }
    }
  // thats all since all scores are multiples of 100
  } 
  
}

void newGameSet(){
   hardness = 15000;
   strikes = 0;
   pressStart = true;
   win = false;
   score = 0;
}
void  playPressAndHold(){
  playNoise('g', 0);
}
void playPrompt(int fruit){
   playNoise('w', fruit); 
}
void playSound(int fruit){
   playNoise('s', fruit); 
}
void playNoise(char type, int fruit){
  while(mySerial.available()){
    Serial.print("Message from vmusic: ");
    Serial.println((char) mySerial.read());
    delay(20);
  }
  char filename[10];
  char therealrealFruit[3];
  int i =0;
  String realFruit = String(fruit);
  realFruit.toCharArray(therealrealFruit, 3);
  
  filename[i++] = type;
  filename[i++] = therealrealFruit[0];
  if(fruit >= 10){
    filename[i++] = therealrealFruit[1];
  }
  filename[i++] = '.';
  filename[i++] = 'm';
  filename[i++] = 'p';
  filename[i++] = '3';
  filename[i++] = '\0';
  Serial.print("fileName: ");  
  Serial.println(filename);
  mySerial.write("VPF ");
  mySerial.write(filename);
  mySerial.write("\r");
}
int readTouchInputs(){
  if(!checkInterrupt()){
    
    //read the touch state from the MPR121
    Wire.requestFrom(0x5A,2); 
    
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    
    uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

    
    for (int i=0; i < 12; i++){  // Check what electrodes were pressed
      if(touched & (1<<i)){
      
        if(touchStates[i] == 0){
          //pin i was just touched
          /*Serial.print("pin ");
          Serial.print(i);
          Serial.println(" was just touched");
          Serial.print("pin 8:");*/
          ////Serial.println(digitalRead(8));
          //while(digitalRead(8)){
          //}
          //mySerial.write("VPF s0.mp3\r");
          ////Serial.println(mySerial.read());
          return i;
        }else if(touchStates[i] == 1){
          //pin i is still being touched
        }  
      
        touchStates[i] = 1;      
      }else{
        if(touchStates[i] == 1){
          Serial.print("pin ");
          Serial.print(i);
          Serial.println(" is no longer being touched");
          
          //pin i is no longer being touched
       }
       
        touchStates[i] = 0;
      }
    
    }
    
  }
  return -1;
}




void mpr121_setup(void){

  set_register(0x5A, ELE_CFG, 0x00); 
  
  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);
  
  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, 0x44);
  set_register(0x5A, ELE0_R, 0x44);
 
  set_register(0x5A, ELE1_T, 0x33);
  set_register(0x5A, ELE1_R, 0x33);
  
  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);
  
  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);
  
  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);
  
  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);
  
  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);
  
  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);
  
  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);
  
  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);
  
  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);
  
  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);
  
  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);
  
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes
  
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
  
  set_register(0x5A, ELE_CFG, 0x0C);
  
}


boolean checkInterrupt(void){
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}
