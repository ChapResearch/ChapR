//
// gamepad.h
//
//   This file (and the associated .cpp) implement the data management for gamepads
//   that are plugged into the ChapR.
//
//   These routines know how to translate from the gamepad input coming in from the
//   USB, to a structure allowing a bit easier usage of the gamepad data.
//

#ifndef GAMEPAD_H
#define GAMEPAD_H

#define GAMEPAD_B1(g)	(g->buttons&0x0001)
#define GAMEPAD_B2(g)	(g->buttons&0x0002)
#define GAMEPAD_B3(g)	(g->buttons&0x0004)
#define GAMEPAD_B4(g)	(g->buttons&0x0008)
#define GAMEPAD_B5(g)	(g->buttons&0x0010)
#define GAMEPAD_B6(g)	(g->buttons&0x0020)
#define GAMEPAD_B7(g)	(g->buttons&0x0040)
#define GAMEPAD_B8(g)	(g->buttons&0x0080)
#define GAMEPAD_B9(g)	(g->buttons&0x0100)
#define GAMEPAD_B10(g)	(g->buttons&0x0200)
#define GAMEPAD_B11(g)	(g->buttons&0x0400)
#define GAMEPAD_B12(g)	(g->buttons&0x0800)

class Gamepad 
{
public:
     Gamepad(int);			// constructor simply loads default data

     int		id;		// which gamepad am I?  1 or 2
     int		x1;		// left stick X reading (-128 to 127, -128 is to the left)
     int		y1;		// left stick Y reading (-128 to 127, -128 is to the top)
     int		x2;		// right stick X reading (-128 to 127, -128 is to the left)
     int		y2;		// right stick Y reading (-128 to 127, -128 is to the top)
     unsigned int	tophat;		// 0 if nothing pressed, 1 thru 8 for N, NE, E, etc... (clockwise)
     unsigned int	buttons;	// buttons & 0x01 is button 1, 0x02 is 2, 0x04 is 3, 0x08 is 4, etc.

     void		load(byte *usbdata);
     void		debugPrint(char *);
     bool		update(VDIP *);
     
};

#endif GAMEPAD_H

