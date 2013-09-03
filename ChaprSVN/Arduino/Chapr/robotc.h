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

int robotcTranslate(byte *, bool, Gamepad *, Gamepad *);

#endif ROBOTC_H

