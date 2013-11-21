//
// robotc.h
//
//   This file (and the associated robotc.cpp) supports communicating with an NXT that
//   is running robotc.  It is used mainly to translate the gamepad data to data that
//   the robotc library understands.
//
//   This is part of the "personality" of the NXT running robotc.
//

#ifndef ROBOTC_H
#define ROBOTC_H

//
// The user mode (defined in RobocC JoystickDriver.c) is either true (1) for teleop,
// or false(0) for autonomous.  It is used a boolean in the code.

#define USER_MODE_TELEOP	1
#define USER_MODE_AUTONOMOUS	0

int robotcTranslate(byte *, bool, Gamepad *, Gamepad *, int mode);

#endif ROBOTC_H

