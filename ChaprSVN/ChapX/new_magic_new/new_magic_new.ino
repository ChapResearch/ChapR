#include "mpr121.h"
#include <Wire.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(12, 11); // RX, TX (flipped from docs)

boolean touchStates[12]; //to keep track of the previous touch states
int RTS = 0;
volatile boolean touch_status_changed =false;
void touch_irq()
{
  touch_status_changed=true;
}
void setup(){

  attachInterrupt(0, touch_irq, FALLING);

  pinMode(8, INPUT);
  digitalWrite(8, HIGH);

  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();

  mpr121_setup();
}

////////////////////////////////////////////////////////////////////////////////////////
//ALL FRUIT MUST BE PUT ON THE PINS WITH NUMBERS THE SAME AS THEIR SOUND FILE NUMMBERS//
////////////////////////////////////////////////////////////////////////////////////////
#define NUMFRUITS 12
int gameType = -1; //1-classic,2-Simon,3-team
boolean pressStart = true;
#define NUMGAMES 3


//team

//

// g0 presstoplay g1 goodsound g2 badsound g3 newhighscore g4 your score is  g5 wah wah wah waaaaaaahhh g6 silence
void loop(){

  if(pressStart){
    Serial.print("Press and hold apple for classic or lemon for piano"); //play pressmesound
    playPressAndHold();
    delay(7000);
    gameType = readTouchInputs() + 1;
    Serial.print(gameType);
    if(gameType > 0 && gameType <= NUMGAMES){
      delay(1000);
      // if anything is touched by this time
      pressStart = false;
      Serial.print(gameType);
    }
  }
  else{
    if(gameType == 1){
      doSimon();
    }
    if(gameType == 2){
      doClassic();
    }
    if(gameType == 3){
      doPiano();
    }
  }
  if(gameType == 1){
    checkSimon();
  }
  if(gameType == 2){
    checkClassic();
  }
  if(gameType == 3){
    //checkTeam();
  }
  Serial.print("reading vmusic in main loop");
  readVMusic();
  while (Serial.available())
    mySerial.write(Serial.read());
  delay(10);

}
// clssic
int hardness = 10000; //ten seconds to touch
long score = 0;
int strikes = 0;
boolean win = false;
int pinTouched;
int highscore = 1;
//
void doClassic(){
  bool strikeDone = false;
  int randNum = (int)random(0,NUMFRUITS);
  playPrompt(randNum); //play prompt sound
  long start = millis();
  int addScore = 0;
  long touchWindow = start + hardness; //reaction timeF
  while(millis() < touchWindow){
    pinTouched = readTouchInputs();
    if(pinTouched == randNum){
      long touch_time=millis();
      playSound(randNum); //play fruit sound

      delay(2000);
      int timeBonus = ((touchWindow - touch_time)%100)*100 - (hardness/1000 +4) * 100 ;

      if(hardness > 2000){
        hardness = hardness-1000;
        addScore = 100 + timeBonus - 500;
        Serial.print("Score: ");
        Serial.println(score);
        Serial.println(hardness);
      }
      else{
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

  }
  else{
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
      delay(4000);
      Serial.println("BOO! You SUCK at this game. But you got a highscore????"); //play meh
      Serial.print("New highscore: "); //play sound score
      playNoise('g',3);//your new high score is
      delay(4000);
      Serial.println(score);
      playScore(score);//score
      highscore = score;
      Serial.println("setting ne game");
      newGameSet();
      Serial.println(pressStart? "true": "false");
      delay(1000);
      Serial.println(win? "true" : "false");
    }
    else{
      playNoise('g', 5); //game over
      delay(4000);
      Serial.println("BOO! You SUCK at this game."); //play badsound
      Serial.print("Score: "); //play sound score
      playNoise('g', 4);
      delay(4000);
      Serial.println(score);
      playScore(score);  //play sound score
      Serial.println("setting new game");
      newGameSet();
      Serial.println(pressStart? "true": "false");
      delay(1000);
      Serial.println(win? "true" : "false");
    }
  }
  if(win){
    if(score > highscore){
      Serial.println("WOOT!! You WIN at this game. AND you got a highscore!"); //play goodsound
      Serial.print("New highscore: ");
      playNoise('g',1); //you win
      delay(4000);
      Serial.println(score);
      playNoise('g',3); //your new high score is
      delay(4000);
      playScore(score); //play sound score
      highscore = score;
      Serial.println("setting new game");
      newGameSet();
      Serial.println(pressStart? "true": "false");
      delay(1000);
      Serial.println(win? "true" : "false");
      delay(500);
    }
    else{
      Serial.println("WOOT!! You WIN at this game."); //play goodsound
      Serial.print("Score: ");
      playNoise('g',1); //you win
      delay(2000);
      playNoise('g',4);
      delay(2000);
      playScore(score);
      Serial.println(score);
      Serial.println("setting new game");
      newGameSet();
      Serial.println(pressStart? "true": "false");
      delay(1000);
      Serial.println(win? "true" : "false");
      delay(500);
    }
  }


}
void doPiano(){
  Serial.print("Its in");
  int note = readTouchInputs();
  if(note == 10)
    pressStart = true;
  playNoise('n',note);
  delay(50);

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
    }
    else{
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
    }
    else{
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
void readVMusic()
{
  Serial.println("Message from vmusic: ");
  while(mySerial.available()){
    Serial.print((char) mySerial.read());
  }
  Serial.println(".");
}
void checkVPlayed()
{
  //waiting for response from VMusic
  while(!mySerial.available())
  {
    delay(500);
  }
  readVMusic();
}
void playDigit(char d)
{
  readVMusic();
  mySerial.write("VPF ");
  mySerial.write(d);
  mySerial.write(".mp3\r");
  checkVPlayed();
}
void playFile(const char* filename)
{
  readVMusic();
  mySerial.write(filename);
  mySerial.write("\r");
  checkVPlayed();
}
void playScore(long score)
{
  int scoreDigits[6];
  int i = 0;
  Serial.print(score);
  while(score >0 && i <6){
    int temp = (int)(score%10);
    Serial.print(temp);
    //delay(500);
    score = score/10;
    scoreDigits[i] = temp;
    i++;
  }
  if(!(scoreDigits[5]>=0 && scoreDigits[5]<10)){
    scoreDigits[5] = 0;
  }


  //test
  /*for(int i =0; i < 6; i++)
   {
   Serial.println(i);


   mySerial.write("VPF ");
   Serial.print("VPF ");
   mySerial.write((char)((scoreDigits[i])+48));
   Serial.print(scoreDigits[i]+48);
   mySerial.write(".mp3\r");
   Serial.print(".mp3\r");
   delay(1000);
   }//
   */
  int firstDigit=5;
  for(int i = 5; i > -1; i--){
    Serial.print("playing i=");
    Serial.println(i);
    if(scoreDigits[i] == 0&&i==firstDigit){
      Serial.println("playing empty noise");
      firstDigit--;//next digit is the first now.
      //      delay(500);
      //playNoise('g', 6);
    }
    else{
      if(i == 5){ //fist digit
        //play number + hundred
        Serial.print(scoreDigits[i]);// it's getting mixed up here
        playDigit((char)(scoreDigits[i]+48));
        playFile("VPF 100.mp3");
        delay(500);
      }

      if(i == 4 && scoreDigits[i]>1) // second digit not in teens of thousands
      {
        //play number + 0 to play sound like "sixty"
        Serial.print(scoreDigits[i]);
        readVMusic();
        char temp=(char)(scoreDigits[i] + 48);
        mySerial.write("VPF ");
        mySerial.write(temp);
        mySerial.write('0');
        mySerial.write(".mp3\r");
        checkVPlayed();
        delay(200);
        Serial.println("Yay");

      }

      if(i == 3) // third digit
      {
        //play number + thousand
        Serial.print(scoreDigits[i]);

        if(scoreDigits[4]==1) {
          if(scoreDigits[i]>2){
            playDigit((char)(scoreDigits[i]+48));
            playFile("VPF teen.mp3");
          }
          else{//for 10, 11 and 12
            //FIXME: need files...
            readVMusic();
            mySerial.write("VPF ");
            mySerial.write('1');
            mySerial.write((char)(scoreDigits[i]+48));
            mySerial.write(".mp3\r");
            checkVPlayed();
          }
        }
        else
        {
          playDigit((char)(scoreDigits[i]+48));
        }

        playFile("VPF 1000.mp3");
        delay(500);
      }
      if(i == 2) // fourth digit
      {
        //play number + hundred
        Serial.println("playing 4th digit");
        Serial.println(scoreDigits[i]);
        playDigit((char)(scoreDigits[i]+48));
        Serial.print("SOmething happened");
        playFile("VPF 100.mp3");
        delay(500);
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
  readVMusic();
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
int _i2caddr=0x5A;

int readTouchInputs(){
  int touched_pin=-1;
  while(touched_pin==-1){
    if(touch_status_changed){
      touch_status_changed=false;
      Serial.println("something happened!");
      //read the touch state from the MPR121
      while(Wire.requestFrom(0x5A,2)!=2);

      byte LSB = Wire.read();
      byte MSB = Wire.read();

      uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states

      for (int i=0; i < 12; i++){  // Check what electrodes were pressed
        if(touched & (1<<i)){

          if(touchStates[i] == 0){
            Serial.print("hi");
            //pin i was just touched
            //if(i == 0){
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was just touched");
            //}
            touched_pin=i;

          }
          else if(touchStates[i] == 1){
            //pin i is still being touched
          }

          touchStates[i] = 1;
        }
        else{
          if(touchStates[i] == 1){
            //if(i == 0){
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" is no longer being touched");
            //}
            //pin i is no longer being touched
          }
          touchStates[i] = 0;
        }
      }
    }
    delay(5);
  }
  return touched_pin;
}


void mpr121_setup(void){
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  writeRegister(ELE_CFG, 0x00);

  // Section A - Controls filtering when data is > baseline.
  writeRegister(MHD_R, 0x01);
  writeRegister(NHD_R, 0x01);
  writeRegister(NCL_R, 0x0E);
  writeRegister(FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  writeRegister(MHD_F, 0x01);
  writeRegister(NHD_F, 0x05);
  writeRegister(NCL_F, 0x01);
  writeRegister(FDL_F, 0x00);

  // Section C - Sets touch and release thresholds for each electrode
  writeRegister(ELE0_T, 7);
  writeRegister(ELE0_R, 2);

  writeRegister(ELE1_T, 6); //apple
  writeRegister(ELE1_R, 3);

  writeRegister(ELE2_T, TOU_THRESH); //2,5
  writeRegister(ELE2_R, REL_THRESH);

  writeRegister(ELE3_T, TOU_THRESH);//1,4
  writeRegister(ELE3_R, REL_THRESH);

  writeRegister(ELE4_T, TOU_THRESH);//5,7
  writeRegister(ELE4_R, REL_THRESH);

  writeRegister(ELE5_T, TOU_THRESH);
  writeRegister(ELE5_R, REL_THRESH);

  writeRegister(ELE6_T, TOU_THRESH);
  writeRegister(ELE6_R, REL_THRESH);

  writeRegister(ELE7_T, TOU_THRESH);//7,7
  writeRegister(ELE7_R, REL_THRESH);

  writeRegister(ELE8_T, TOU_THRESH);
  writeRegister(ELE8_R, REL_THRESH);

  writeRegister(ELE9_T, TOU_THRESH);
  writeRegister(ELE9_R, REL_THRESH);

  writeRegister(ELE10_T, 9);
  writeRegister(ELE10_R, 2);

  writeRegister(ELE11_T, TOU_THRESH);
  writeRegister(ELE11_R, REL_THRESH);

  // Section D
  // Set the Filter Configuration
  // Set ESI2
  //writeRegister(FIL_CFG, 16);
  writeRegister(MPR121_CONFIG1, 0x21); // default, 16uA charge current
  writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  writeRegister(ELE_CFG, 0x0C);  // Enables all 12 Electrodes

  read_register(0x5A);


  // Section F
  // Enable Auto Config and auto Reconfig
  /*writeRegister(ATO_CFG0, 0x0B);
   writeRegister(ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   writeRegister(ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
   writeRegister(ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V

  //writeRegister(ELE_CFG, 0x0C);

}


void read_register(int address){
  while(Wire.requestFrom(address, 6)!=6);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  {
    char c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}
uint16_t  filteredData(uint8_t t) {
  if (t > 12) return 0;
  return readRegister16(MPR121_FILTDATA_0L + t*2);
}

uint16_t  baselineData(uint8_t t) {
  if (t > 12) return 0;
  uint16_t bl = readRegister8(MPR121_BASELINE_0 + t);
  return (bl << 2);
}

uint16_t  touched(void) {
  uint16_t t = readRegister16(MPR121_TOUCHSTATUS_L);
  return t & 0x0FFF;
}

/*********************************************************************/


uint8_t readRegister8(uint8_t reg) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  while (Wire.requestFrom(_i2caddr, 1) != 1);
  return ( Wire.read());
}

uint16_t readRegister16(uint8_t reg) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(reg);
  Wire.endTransmission(false);
  while (Wire.requestFrom(_i2caddr, 2) != 2);
  uint16_t v = Wire.read();
  v |=  ((uint16_t) Wire.read()) << 8;
  return v;
}

/**************************************************************************/
/*!
 @brief  Writes 8-bits to the specified destination register
 */
/**************************************************************************/
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(_i2caddr);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(value));
  Wire.endTransmission();
}

