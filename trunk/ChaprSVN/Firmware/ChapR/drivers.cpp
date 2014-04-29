//
// drivers
//
//	These are the "drivers" for the differing USB devices the
//	ChapR supports.  The main part of this file is the translator
//	functions for converting a specific USB gamepad's data to the
//	canonical form required by the gamepad object.  These are
//	really the "drivers" for any USB HID device.  New drivers are
//	added here.
//
//	Note that all of these functions take a reference to the gamepad object
//	and load it up appropriately.  It could simply be a pointer, but I'm
//	trying to be more C++'ish.

//
// As the ChapR starts to control more things, it also opens up the need to allow
// more and different types of controllers that plug into the USB.  From simple
// game pads, through complex gamepads, and even "thrustmaster" controllers, the
// ChapR needs a mechanism to allow these devices to connect and control.  On an
// OS like Windows, there are device drivers that are loaded for each different
// type of controller that is plugged into the USB port.  That's essentially what
// we do here. Though our device drivers are somewhat minimal because the
// ChapR only has a certain (small) number of personalities, so there are only
// a small number of things that a driver must do.
//
// While each of the controllers responds like an "HID" device, they use different
// encodings for the movements of buttons and sticks.  While the original USB
// specification attempted to normalize this, and make a self-descriptive mechanism
// for these types of devices, it appears that the "normal" thing to do these days
// is to support specific devices through their USB IDs.
//
// Our "device driver" capability will accomplish two things:
//
//	- Allow for read of different size of packets by the VDIP.
//
//	- Map the different controls on the connected controller to a standard
//	  controller mapping that the ChapR supports.
//
// All of the device drivers for gamepads live in this file.
//
// In the future, it would be nice to be able to augment this information by using
// a flash drive, and storing the controller database in EEPROM (or part of it anyway).
// There would undoubtedly be a mechanism for clearing the controller data too.

#include <arduino.h>
#include "drivers.h"
#include "VDIP.h"
#include "gamepad.h"

//
// The following table maps USB VID/PID entries to translator functions.
// It is very specific.  Only those matching VID/PID exactly can be
// translated.

struct {
     short	vid;	// vendor ID
     short	pid;	// product ID
     xlateFn	xlate;	// function to xlate to gamepad canonical form
} usbIDTable[] = {
     { 0x0E6F, 0x0401, driverXbox360 },	// the Gamestop Xbox 360 controller
     { 0x046D, 0xC216, driverF310 },	// "dual action" Logitech 310 (3 face buttons) (colored & #'d buttons)
     { 0x046D, 0xC218, driverF310 },	// "rumblepad" Logitech 310 (4 face buttons)
     { 0x046D, 0xC214, driverAttack3 },	// "attack 3" Logitech

     { 0x0000, 0x0000, (xlateFn) NULL }	// last one in table must be this
};

xlateFn driverLookup(int vid, int pid)
{
     int	i = 0;

     while(usbIDTable[i].xlate != NULL) {
	  if(usbIDTable[i].vid == vid && usbIDTable[i].pid == pid) {
	       break;
	  }
	  i++;
     } 
     return(usbIDTable[i].xlate);
}

//
// driverAttack3() - translation of the Logitech attack 3 controller data.
//
bool driverAttack3(byte *data, int count, Gamepad &target)
{
     // since the buttons are simply numbered, and really don't correspond to
     // cononcial form, they are just used verbatim

     if(count != 5) {
	  return(false);
     }

     target.x1 = ((int)(data[0]))-128;	// joystick 1 (left) X axis 
     target.y1 = ((int)(data[1]))-128;	// joystick 1 (left) Y axis 
     target.y2 = ((int)(data[2]))-128;	// analog adjustment on bottom

     target.buttons = data[3];			// start by just copying first 8 buttons
     target.buttons |= ((int)data[4]) << 8;	// then put in others

}

//
// driverF310() - translation of Logitech F310 type controller data.
//		 Note that this controller was what canonical form
//		 was build upon.
//
bool driverF310(byte *data, int count, Gamepad &target)
{
     if(count != 8) {
	  return(false);
     }

     target.x1 = ((int)(data[0]))-128;			// joystick 1 (left) X axis 
     target.y1 = ((int)(data[1]))-128;			// joystick 1 (left) Y axis 
     target.x2 = ((int)(data[2]))-128;			// joystick 2 (right) X axis 
     target.y2 = ((int)(data[3]))-128;			// joystick 2 (right) Y axis 

     // data[4] has buttons 4-1 in top 4 bits
     // data[5] has buttons 12-5

     target.buttons = ((unsigned int)((data[4]&0xf0)>>4)) | (((unsigned int)data[5])<<4);
     
     target.tophat = data[4] & 0x0f;
     if (target.tophat & 0x08) {	// 0x08 means that no tophat pressed
         target.tophat = 0;
     } else{
	 target.tophat++; 		//we have to map north to 1 to match the canonical form
     }

     // data[6] has the mode value = 08 if mode is toggled "on" - zero  otherwise
     // data[7] is always FF for this controller

     return(true);
}

// XBOX 360 Controller Map
// ------------------------
//
//   XBOX Button	Map
//   -----------	---
//	1(A)		B2
//	2(B)		B3
//	3(X)		B1
//	4(Y)		B4
//
//	5(LB)		B5
//	6(RB)		B6
//
//   The 360 controller has analog sticks for B7/B8, so they are
//   mapped to two things - B7/B8 and analog 3.  They are really weird
//   BTW in that there is no difference in the report when both are pressed
//   and when both aren't pressed.  I imagine creative programming can deal
//   with that issue.
//
//	7/left(back)	B9
//	8/right(start)	B10
//                        _                            _
//                   _.-'` `-._                    _,-' `'-._
//                ,-'          `-.,____________,.-'    .-.   `-.
//               /   .---.             ___            ( Y )     \
//              /  ,' ,-. `.     __   / X \   __   .-. `-` .-.   \
//             /   | |   | |    (__) | / \ | (__) ( X )   ( B )   \
//            /    `. `-' ,'    __    \___/        `-` ,-. `-`     \
//            |      `---`   ,-`  `-.       .---.     ( A )        |
//            |             / -'  `- \    ,'  .  `.    `-`         |
//            |            |          |   | -   - |                |
//            !             \ -.  ,- /    `.  '  ,'                |
//            |              `-.__,-'       `---`                  |
//            |                  ________________                  |
//            |             _,-'`                ``-._             |
//            |          ,-'                          `-.          |
//             \       ,'                                `.       /
//              `.__,-'                                    `-.__,'

//
// driverXbox360() - translation of Logitech F310 type controller data.
//		    Assumes 14 bytes data.
//
bool driverXbox360(byte *data, int count, Gamepad &target)
{
     Serial.println(count);

     if(count != 14) {
	  return(false);
     }

     // the joysticks are a bit funny - they have 2 bytes of resolution

     target.x1 = ((int)(data[1]))-128;		// data[0] has more resolution for x1
     target.y1 = ((int)(data[3]))-128;		// data[2] has more resolution for y1
     target.x2 = ((int)(data[5]))-128;		// data[4] has more resolution for x1
     target.y2 = ((int)(data[7]))-128;		// data[5] has more resolution for y1

     // now to translate the buttons appropriately

     target.buttons = 0;
     target.buttons |= (data[10] & 0x01)<<1;	// xbox S --> B2
     target.buttons |= (data[10] & 0x02)<<1;	// xbox E --> B3
     target.buttons |= (data[10] & 0x04)>>2;	// xbox W --> B1
     target.buttons |= (data[10] & 0x08);	// xbox N --> B4

     target.buttons |= (data[10] & 0x30);	// grab the two near shoulder buttons

     target.buttons |= ((int)(data[10] & 0xc0)) << 2;	// get two face buttons

     target.buttons |= ((int)(data[11] & 0x03)) << 10;	// get two joystick buttons

     // now, grab the far shoulder buttons with a bit of a heuristic
     // these two analog'ish buttons cancel either other out with a center
     // at 0x80.  If I press them down exactly together, data[9] will stay
     // at 0x80. Essentially data[9] is the difference between the two with
     // numbers < 0x80 meaning the right button is "more" pressed and numbers
     // above 0x80 means the left button is "more" pressed.

     if (((int)(data[9])) > 0xa0) {
	  target.buttons |= 0x40;
     }
     if (((int)(data[9])) < 0x60) {
	  target.buttons |= 0x80;
     }

     // the tophat is pretty much the same as canonical form, just needs shifting
     //
     // none = 0000 0000   (if shifted right by 2)
     //   N  = 0000 0100 =	1
     //   NE = 0000 1000 =	2
     //   E  = 0000 1100 =	3
     //   SE = 0001 0000 =	4
     //   S  = 0001 0100 =	5
     //   SW = 0001 1000 =	6
     //   W  = 0001 1100 =	7
     //   NW = 0010 0000 =	8

     target.tophat = data[11] >> 2;

     return(true);
}


//
//
//
//               _=====_                               _=====_
//              / _____ \                             / _____	\
//            +.-'_____'-.---------------------------.-'_____'-.+
//           /   |     |  '.        S O N Y        .'  |  _  | 	  \
//          / ___| /|\ |___ \                     / ___| /_\ |___  \
//         / |      |      | ;  __           _   ; | _         _ | ;
//         | | <---   ---> | | |__|         |_:> | ||_|       (_)| |
//         | |___   |   ___| ;SELECT       START ; |___       ___| ;
//         |\    | \|/ |    /  _     ___      _   \    | (X) |    /|
//         | \   |_____|  .','" "', |___|  ,'" "', '.  |_____|  .' |
//         |  '-.______.-' /       \ANALOG/       \  '-._____.-'   |
//         |               |       |------|       |                |
//         |              /\       /      \       /\               |
//         |             /  '.___.'        '.___.'  \              |
//         |            /                            \             |
//          \          /                              \           /
//           \________/                                \_________/
//
//
//
