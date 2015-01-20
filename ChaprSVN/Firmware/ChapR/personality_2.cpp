//
// personality_2.cpp
//
//   Implements the personality:  LabView
//

#include <Arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "personality.h"
#include "personality_0.h"
#include "personality_2.h"
#include "robotc.h"
#include "sound.h"
#include "settings.h"
#include "debug.h"

Personality_2::Personality_2()
{
     buttonToggle = true;	// can't be an initializer - in the super-scope
}
