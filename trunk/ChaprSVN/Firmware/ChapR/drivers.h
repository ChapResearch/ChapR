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

// the translate function takes a byte array and reads it into the gamepad
// object.  There are different translate functions for different types of pads.

typedef bool (*xlateFn)(byte *,int, Gamepad &);

// the initialize function takes a port number, and usb control device number,
// and initializes the controller attached to that usb device for that port

typedef bool (*initFn)(int,VDIP*);

extern void driverLookup(unsigned int,unsigned int,xlateFn *,initFn *);
extern bool driverF310(byte *, int, Gamepad &);
extern bool driverXbox360(byte *, int, Gamepad &);
extern bool driverAttack3(byte *, int, Gamepad &);
extern bool initXbox360(int,VDIP*);
extern bool driverPS3(byte *, int, Gamepad &);
extern bool initPS3(int,VDIP*);

#endif DRIVERS_H
