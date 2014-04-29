//
// gamepadXlate.h
//
//	Forward references for the Xlate routines.

#ifndef DRIVERS_H
#define DRIVERS_H

// sets the size of the data buffer for the maximum amount of gamepad data
// that any driver returns (currently max is 14 with xbox360 driver, but
// we use something a little bigger)

#define MAXTRANSLATE	20

class Gamepad;		// need this because this .h comes before gamepad.h
                        //   which is where Gamepad is actually defined

typedef bool (*xlateFn)(byte *,int, Gamepad &);

extern xlateFn driverLookup(int,int);
extern bool driverF310(byte *, int, Gamepad &);
extern bool driverXbox360(byte *, int, Gamepad &);
extern bool driverAttack3(byte *, int, Gamepad &);

#endif DRIVERS_H
