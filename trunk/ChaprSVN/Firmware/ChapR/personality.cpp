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
  pwrTarget = 0;
  autoStart = 0;
  teleStart = 0;
  timePassed = 0;
  matchMode = AUTO;
}

bool Personality::updateMode()
{
  int oldMode = matchMode;

  if (millis() - endStart >= myEEPROM.getEndLen()){
    matchMode = NONE;
  }
  else if (millis() - teleStart >= myEEPROM.getTeleLen()){
    matchMode = END;
    endStart = millis();
  }
  else if (millis() - autoStart >= myEEPROM.getAutoLen()){
    matchMode = TELE;
    teleStart = millis();
  }
  else {
    matchMode = AUTO;
    autoStart = millis();
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
  case 'a' : autoStart = millis() - timePassed; break;
  case 't' : teleStart = millis() - timePassed; break;
  case 'e' : endStart = millis() - timePassed; break;
  }
}

void Personality::swapInMatchMode(){
  if (pwrTarget > 0 && millis() - pwrTarget <= 3000) // checks to see if the button was pressed 
    matchMode = !matchMode; // twice within 2 seconds
  if (matchMode){
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
