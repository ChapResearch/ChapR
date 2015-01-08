//
// gamepad.h
//
//   This file (and the associated .cpp) implement the data management for gamepads
//   that are plugged into the ChapR.
//
//   These routines know how to translate from the gamepad input coming in from the
//   USB, to a structure allowing a bit easier usage of the gamepad data.
//

#include "drivers.h"

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

//
// Gamepads take a canonical form in the ChapR.  The following diagrams
// attempt to illustrate the canonical form for various controllers.
// The general philosophy, however, is to map the controls for a game
// pad geographically instead of numerically according to the controller
// itself.
//
// For example, a "playstation" compatible controller may number the
// left buttons from 1 to 4 starting at 12 o'clock proceeding clockwise,
// while the Logitech F310 (default FTC controller) starts numbering at
// 9 o'clock proceeding anti-clockwise.  Note that while each controller
// has different labeling schemes for the buttons (like X, Y, A, B) they
// still all have an internal numbering scheme.
//
// The general canonical form is as follows:
//
//	BUTTONS
//	   Location		#	gamepad bitmap
//	---------------		--	--------------------
//	Right Pad
//		West		B1	0000 0000 0000 0001
//		South		B2	0000 0000 0000 0010
//		East		B3	0000 0000 0000 0100
//		North		B4	0000 0000 0000 1000
//
//	Shoulder Buttons	("near" means closest to main controls)
//		left near	B5	0000 0000 0001 0000
//		right near	B6	0000 0000 0010 0000
//		left far	B7	0000 0000 0100 0000
//		right far	B8	0000 0000 1000 0000
//
//	Face Buttons			(the "start" and/or "select" buttons)
//		left		B9	0000 0001 0000 0000
//		right		B10	0000 0010 0000 0000

//	Jostick Buttons
//		joy left	B11	0000 0100 0000 0000
//		joy right	B12	0000 1000 0000 0000
//
//
//	JOYSTICKS
//	   Location		#
//	---------------		--
//	Left Analog		analog #1
//		up/down		-128 up, 127 down
//		left/right	-128 left, 127 right
//	Right Analog		analog #2
//		up/down		-128 up, 127 down
//		left/right	-128 left, 127 right
//
//	TOPHAT
//	---------------
//	zero - nothing pressed		5 - S
//	1 - North			6 - SW
//	2 - NE				7 - W
//	3 - E				8 - NW
//	4 - SE
//
//	The MODE switch isn't necessarily defined.

class Gamepad 
{
private:
     xlateFn		translator;	// translator for incoming USB data
     initFn		init;		// initialize function for the gamepad

public:
     Gamepad(int);			// constructor simply loads default data

     int		id;		// which gamepad am I?  1 or 2
     int		x1;		// left stick X reading (-128 to 127, -128 is to the left)
     int		y1;		// left stick Y reading (-128 to 127, -128 is to the top)
     int		x2;		// right stick X reading (-128 to 127, -128 is to the left)
     int		y2;		// right stick Y reading (-128 to 127, -128 is to the top)
     int		x3;		// misc axis reading (-128 to 127, -128 is to the left)
     int		y3;		// misc axis reading (-128 to 127, -128 is to the top)
     unsigned int	tophat;		// 0 if nothing pressed, 1 thru 8 for N, NE, E, etc... (clockwise)
     unsigned int	buttons;	// buttons & 0x01 is button 1, 0x02 is 2, 0x04 is 3, 0x08 is 4, etc.

     void		load(byte *usbdata);
     void		debugPrint(char *);
     bool		update(VDIP *);
     void		deviceUpdate(VDIP *);
     void		clear();

     bool		initialized;	// true if this gamepad has been initialized, false otherwise
};

#endif GAMEPAD_H

