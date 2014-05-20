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

Personality::Personality():
  pwrTarget(0), autoTarget(0), teleTarget(0), timePassed(0), 
  matchMode(NONE),
  inMatchMode(false)
{
}

// updateMode() - intended to be called from within a personality subclass
//                in the Loop method. Returns a boolean to indicate whether
//                a new mode has started, so that the caller can use it like so:
//
//                if (isInMatchMode()){
//		     if (updateMode()){
//                     switch (getMatchMode()){
//		       case AUTO: // auto started
//		          // personality-specific action when autonomous mode starts
//			  break;
//                     case TELE: // auto ended and tele started
//		          // personality-specific action when teleOp starts
//			  break;
//		       case END: // tele ended and endgame started
//		          // personality-specific action when the endgame starts
//			  break;
//		       case NONE: // not running a program
//		          // personality-specific action when nothing is running
//			  break;
//		       }
//                    }
//                  }
//
bool Personality::updateMode(bool buttonToggle)
{
  int oldMode = matchMode;
  
  Serial.print("endTarget: ");
  Serial.println(endTarget);

  Serial.print("teleTarget: ");
  Serial.println(teleTarget);

  Serial.print("autoTarget: ");
  Serial.println(autoTarget);

  switch(matchMode){
  case AUTO:
    if (millis() > autoTarget){
      matchMode = TELE;
    }
    break;
  case TELE:
    if (teleTarget != 0 && millis() > teleTarget){ //checks to see if teleTarget has a value (and therefore WFS has
     // been pressed
      endTarget = millis() + myEEPROM.getEndLen()*1000;  
      matchMode = END;
    }
    break;
  case END:
    if (millis() > endTarget){
      matchMode = NONE;
    }
    break;
  case NONE:
    autoTarget = 0;
    teleTarget = 0;
    endTarget = 0;
    break;
  }

  return (oldMode != matchMode);
}

void Personality::beginAuto()
{
  if (autoTarget == 0){
    autoTarget = millis() + myEEPROM.getAutoLen()*1000;
    matchMode = AUTO;
  }
}

void Personality::beginTele()
{
  if (teleTarget == 0){
    teleTarget = millis() + myEEPROM.getTeleLen()*1000;
    matchMode = TELE;
  }
}

void Personality::endCycle()
{
  autoTarget, teleTarget, endTarget = 0;
  matchMode = NONE;
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

bool Personality::isInMatchMode()
{
  return (inMatchMode);
}
