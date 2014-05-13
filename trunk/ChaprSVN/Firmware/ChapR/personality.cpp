//
// personality.cpp
//
//   General personality class, implementing matchMode etc.
//

#include <arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "personality.h"
#include "sound.h"
#include "settings.h"
#include "debug.h"

extern sound beeper;
extern settings myEEPROM;

Personality::Personality()
{
  pwrTarget, autoStart, teleStart, timePassed = 0;
  matchMode = NONE;
  inMatchMode = false;
}

bool Personality::updateMode()
{
  int oldMode = matchMode;
  
  Serial.print("endStart: ");
  Serial.println(endStart);

  Serial.print("teleStart: ");
  Serial.println(teleStart);

  Serial.print("autoStart: ");
  Serial.println(autoStart);

  if (endStart != 0 && millis() - endStart >= myEEPROM.getEndLen()*1000){
    matchMode = NONE;
  }
  else if (teleStart != 0 && millis() - teleStart >= myEEPROM.getTeleLen()*1000){
    if (matchMode != END){
      endStart = millis();
      matchMode = END;
    }
  }
  else if (autoStart != 0 && millis() - autoStart >= myEEPROM.getAutoLen()*1000){
    if (matchMode != TELE){
      teleStart = millis();
      matchMode = TELE;
    }
  }
  else {
    // matchMode = AUTO;
    // autoStart = millis();
  }

  return (oldMode != matchMode);
}

void Personality::beginMatchCycle()
{
  autoStart = millis();
}

void Personality::pauseMatchCycle()
{
  switch (matchMode){
  case AUTO : timePassed = millis() - autoStart; break;
  case TELE : timePassed = millis() - teleStart; break;
  case END  : timePassed = millis() - endStart; break;

  }
}

void Personality::playMatchCycle(){
  switch (matchMode){
    // case NONE : autoStart = millis(); break;
  case AUTO : autoStart = millis() - timePassed; break;
  case TELE : teleStart = millis() - timePassed; break;
  case END  : endStart = millis() - timePassed; break;
  }
}

void Personality::swapInMatchMode(){

  if (pwrTarget > 0 && millis() - pwrTarget <= 3000) // checks to see if the button was pressed 
    inMatchMode = !inMatchMode; // twice within 2 seconds
  if (inMatchMode){
    beeper.select();
    delay(150);
    beeper.select();
  } else {
    beeper.select();
  }

  pwrTarget = millis();
}

int Personality::getMatchMode()
{
  return (matchMode);
}
