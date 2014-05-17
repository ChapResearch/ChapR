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
//                the mode has changed, so that the caller can use it like so:
/*                if (isInMatchMode()){
		     if (updateMode()){
                       switch (getMatchMode()){
		       case AUTO:
		          // personality-specific action when autonomous mode starts
			  break;
                       case TELE:
		          // personality-specific action when teleOp starts
			  break;
		       case END:
		          // personality-specific action when the endgame starts
			  break;
		       case NONE:
		          // personality-specific action when nothing is running
			  break;
		       }
                    }
                  }
*/
bool Personality::updateMode(bool buttonToggle)
{
  int oldMode = matchMode;
  
  Serial.print("endTarget: ");
  Serial.println(endTarget);

  Serial.print("teleTarget: ");
  Serial.println(teleTarget);

  Serial.print("autoTarget: ");
  Serial.println(autoTarget);

  if (endTarget != 0 && millis() > endTarget){
    matchMode = NONE;
    teleTarget = 0;
    endTarget = 0;
  }
  else if (teleTarget != 0 && millis() > teleTarget){
    if (matchMode != END){
      endTarget = millis() + myEEPROM.getEndLen()*1000;
      matchMode = END;
    }
  }
  else if (autoTarget != 0 && millis() > autoTarget){
    // starts tele, but only if the WFS has been pressed and the 
    // caller program has already been notified

    if (matchMode == TELE && buttonToggle){
      Serial.println("in if");
      teleTarget = millis() + myEEPROM.getTeleLen()*1000;
      autoTarget = 0;
    }
    Serial.println("in autoTarget");
    matchMode = TELE;
  }

  return (oldMode != matchMode);
}

// beginMatchCycle() - intended to be called when autonomous mode
//                     is enabled, which begins the cycling between
//                     autonomous, then tele, then endgame.
void Personality::beginMatchCycle()
{
  autoTarget = millis() + myEEPROM.getAutoLen()*1000;
  matchMode = AUTO;
}

void Personality::pauseMatchCycle()
{
  switch (matchMode){
  case AUTO : timePassed = millis() - autoTarget; autoTarget = 0; break;
  case TELE : timePassed = millis() - teleTarget; teleTarget = 0; break;
  case END  : timePassed = millis() - endTarget;  endTarget = 0;  break;
  }
  //  bool isPaused = true;
}

void Personality::playMatchCycle(){
  // if (isPaused){
    switch (matchMode){
    case AUTO : autoTarget = millis() - timePassed; break;
    case TELE : teleTarget = millis() - timePassed; break;
    case END  : endTarget = millis() - timePassed; break;
    }
    //} else {
    //  }
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
