//
// personality.cpp
//
//   General personality class, implementing matchMode etc.
//

#include <Arduino.h>
#include "BT.h"
#include "VDIP.h"
#include "gamepad.h"
#include "personality.h"

Personality::Personality():
  pwrTarget(0), autoTarget(0), teleTarget(0), timePassed(0), 
  inMatchMode(false),
  enabled(false)
{
}
