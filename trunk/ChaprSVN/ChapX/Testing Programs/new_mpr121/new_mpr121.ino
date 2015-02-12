#include "new_mpr121.h"
#include <Wire.h>

int irqpin = 2;  // Digital 2
boolean touchStates[12]; //to keep track of the previous touch states
volatile boolean touch_status_changed =false;
void touch_irq()
{
  touch_status_changed=true;
}
void setup(){
  //pinMode(irqpin, INPUT);
  //digitalWrite(irqpin, HIGH); //enable pullup resistor
  attachInterrupt(0, touch_irq, FALLING);
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Hi Moon?");

  mpr121_setup();
}

void loop(){
  readTouchInputs();
  /*Serial.print("Baseline for pin 1: ");
  Serial.println(baselineData(0));
  Serial.print("Filt Data for pin 1: ");
  Serial.println(filteredData(0));
  Serial.print("touch status for pin 1: ");

  Serial.println(touched()&0x02);
  delay(1000);
*/
}
int _i2caddr=0x5A;

void readTouchInputs(){
  
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
}




void mpr121_setup(void){
  writeRegister(MPR121_SOFTRESET, 0x63);
  delay(1);
  set_register(0x5A, ELE_CFG, 0x00);

  // Section A - Controls filtering when data is > baseline.
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x0E);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x05);
  set_register(0x5A, NCL_F, 0x01);
  set_register(0x5A, FDL_F, 0x00);

  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, 7);
  set_register(0x5A, ELE0_R, 2);

  set_register(0x5A, ELE1_T, TOU_THRESH); //apple
  set_register(0x5A, ELE1_R, REL_THRESH);

  set_register(0x5A, ELE2_T, TOU_THRESH); //2,5
  set_register(0x5A, ELE2_R, REL_THRESH);

  set_register(0x5A, ELE3_T, 9);//1,4
  set_register(0x5A, ELE3_R, REL_THRESH);

  set_register(0x5A, ELE4_T, TOU_THRESH);//5,7
  set_register(0x5A, ELE4_R, REL_THRESH);

  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);

  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);

  set_register(0x5A, ELE7_T, TOU_THRESH);//7,7
  set_register(0x5A, ELE7_R, REL_THRESH);

  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);

  set_register(0x5A, ELE9_T, 7);
  set_register(0x5A, ELE9_R, 2);

  set_register(0x5A, ELE10_T, 7);
  set_register(0x5A, ELE10_R, 2);

  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);

  // Section D
  // Set the Filter Configuration
  // Set ESI2
  //set_register(0x5A, FIL_CFG, 16);
   writeRegister(MPR121_CONFIG1, 0x1f); // default, 16uA charge current
   writeRegister(MPR121_CONFIG2, 0x20); // 0.5uS encoding, 1ms period

  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes

  read_register(0x5A);


  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
   set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
   set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V

  //set_register(0x5A, ELE_CFG, 0x0C);

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


void read_register(int address){
  Wire.requestFrom(address, 6);    // request 6 bytes from slave device #2

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
