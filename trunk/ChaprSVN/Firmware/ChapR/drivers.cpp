//
// drivers
//
//	These are the "drivers" for the differing USB devices the
//	ChapR supports.  The main part of this file is the translator
//	functions for converting a specific USB gamepad's data to the
//	canonical form required by the gamepad object.
//	New drivers are added here.
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

#include <Arduino.h>
#include "VDIP.h"
#include "drivers.h"
#include "gamepad.h"

//
// The following table maps USB VID/PID entries to translator functions.
// It is very specific.  Only those matching VID/PID exactly can be
// translated.
//
// NOTES:
//	- the PS3 driver doesn't work and I haven't taken the time to figure it out
//	  so it is commented out in the driver database and the code is also commented
//	  out below.
//	- Only a few drivers will ever need the "init" function.  For the Xbox it
//	  turns on the right LED to identify the controller - it will do the same for PS3
//

struct {
     short	vid;	// vendor ID
     short	pid;	// product ID
     xlateFn	xlate;	// function to xlate to gamepad canonical form
     initFn	init;	// function to init gamepad the first time
} usbIDTable[] = {
     { 0x0E6F, 0x0401, driverXbox360, initXbox360 },	// the Gamestop Xbox 360 controller
     { 0x0E6F, 0x0213, driverXbox360, initXbox360 },	// the Afterglow Xbox 360 controller
     { 0x045E, 0x028E, driverXbox360, initXbox360 },	// the Microsoft Xbox 360 controller
     { 0x046D, 0xC216, driverF310, (initFn) NULL },	// "dual action" Logitech 310 (3 face buttons) (colored & #'d buttons)
     { 0x046D, 0xC218, driverF310, (initFn) NULL },	// "rumblepad" Logitech 310 (4 face buttons)
     { 0x046D, 0xC214, driverAttack3, (initFn) NULL },	// "attack 3" Logitech
//     { 0x054C, 0x0268, driverPS3, initPS3 },		// generic PS3 controller

     { 0x0000, 0x0000, (xlateFn) NULL, (initFn) NULL }	// last one in table must be this
};

void driverLookup(int vid, int pid, xlateFn *xlate, initFn *init)
{
     int	i = 0;

     while(usbIDTable[i].xlate != NULL) {
	  if(usbIDTable[i].vid == vid && usbIDTable[i].pid == pid) {
	       break;
	  }
	  i++;
     } 
     *xlate = usbIDTable[i].xlate;
     *init = usbIDTable[i].init;
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
     target.x2 = ((int)(data[2]))-128;	// analog adjustment on bottom

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
// [3] upper nibble 0x10 A (south)
//		    0x20 B (east)
//		    0x40 X (west)
//		    0x80 Y (north)
// [3] lower nibble 0x01 5 (upper left shoulder)
//		    0x02 6 (upper right shoulder)
//		    0x04 center X button
// [2] lower nibble 0x01 TH N
//		    0x09 TH NE
//		    0x08 TH E
//		    0x0A TH SE
//		    0x02 TH S
//		    0x06 TH SW
//		    0x04 TH W
//		    0x05 TH NW
// [2] upper nibble 0x10 Start button
//		    0x20 Back button
// 		    0x40 Left Joystick press
//		    0x80 Right Joystick press
// [4] lower left shoulder 0 - FF
// [5] lower right shoulder 0 - FF
// [6] left stick sub-x
// [7] left stick x - 0x00 center 0x80 right, 0x7f left (looks simply signed)
// [8] left stick sub-y
// [9] left stick y - top is 0x7f
// [10] right stick sub-x
// [11] right stick x - left is 0x7f
// [12] right stick sub-y
// [13] right stick y - top is 0x7f
//

//
// initXbox360() - just turns on the LED - port is 1 or 2
//
//	0x00	 All off
//	0x01	 All blinking
//	0x02	 1 flashes, then on
//	0x03	 2 flashes, then on
//	0x04	 3 flashes, then on
//	0x05	 4 flashes, then on
//	0x06	 1 on
//	0x07	 2 on
//	0x08	 3 on
//	0x09	 4 on
//	0x0A	 Rotating (e.g. 1-2-4-3)
//	0x0B	 Blinking*
//	0x0C	 Slow blinking*
//	0x0D	 Alternating (e.g. 1+4-2+3), then back to previous*
//
bool initXbox360(int port, VDIP *vdip)
{
     char	cbuf[3];

    // all xbox init does is turn on the correct LED

     cbuf[0] = 0x01;
     cbuf[1] = 0x03;
     cbuf[2] = (port==1)?0x02:0x03;

     vdip->portCmd(port,VDIP_DSD,cbuf,100,3);		// send the command

     return(true);
}

bool driverXbox360(byte *data, int count, Gamepad &target)
{
     extern void dumpDataHex(char *,byte *,int);

     if(count != 20) {
	  return(false);
     }

     // the joysticks are a bit funny - they have 2 bytes of resolution
     // but they are also already in negative format (0x80 - 0x00 - 0x7F)
     // so we let the sign extension occur below.  Note that the Y axis
     // needs to be flipped.

     target.x1 = data[7];		// data[6] has more resolution for x1 - ignored
     target.y1 = data[9] ^ 0xff;	// data[8] has more resolution for y1 - ignored
     target.x3 = data[11];		// data[10] has more resolution for x2 - ignored
     target.y3 = data[13] ^ 0xff;	// data[12] has more resolution for y2 - ignored

     // now to translate the buttons appropriately

     target.buttons = data[3]>>4;
    
     /*target.buttons = 0;
     target.buttons |= (data[3] & 0x10)>>3;	// xbox S --> B2
     target.buttons |= (data[3] & 0x20)>>3;	// xbox E --> B3
     target.buttons |= (data[3] & 0x40)>>6;	// xbox W --> B1
     target.buttons |= (data[3] & 0x80)>>4;	// xbox N --> B4
*/
     target.buttons |= (data[3] & 0x03)<<4;	// grab the two near shoulder buttons

     // the two far shoulder buttons on the xbox 360 are analog, so interpret them

     target.x2 = data[4];
     Serial.print("x2: ");
     Serial.println(target.x2);
     target.y2 = data[5]; //^ 0xff
     Serial.print("y2: ");
     Serial.println(target.y2);

     // old way of translating triggers to buttons    

     /*if(data[4] > 0x7f) {
	  target.buttons |= 0x40;
     }
     if(data[5] > 0x7f) {
	  target.buttons |= 0x80;
     }*/

     target.buttons |= ((int)(data[2] & 0x20)) << 1;	// get back button
     target.buttons |= ((int)(data[2] & 0x10)) << 3;	// get start button
    
     target.buttons |= ((int)(data[2] & 0xc0)) << 2;	// get two joystick buttons

     //target.buttons |= ((int)(data[3] &0x04)) << 10;	// get the big X

     // tophat is really different.  the four major compass points are the major
     // LSB bits, and when combined they just AND together for the minor points.
     //
     //               0x01 (N)
     //			 ^
     //                  |
     //      0x04(W)  <--|-->  0x08 (E)
     //			 |
     //			 v
     //               0x02 (S)
     
     Serial.print("final: ");
     Serial.println(target.buttons);

     switch(data[2]) {
     case 0x01:	target.tophat = 1; break;
     case 0x09:	target.tophat = 2; break;
     case 0x08:	target.tophat = 3; break;
     case 0x0A:	target.tophat = 4; break;
     case 0x02:	target.tophat = 5; break;
     case 0x06:	target.tophat = 6; break;
     case 0x04:	target.tophat = 7; break;
     case 0x05:	target.tophat = 8; break;
     default:	target.tophat = 0; break;
     }

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

#ifdef NOTDEF		// PS3 not working yet

bool initPS3(int port, VDIP *vdip)
{
     int	i=0;
     char	cbuf[15];

     Serial.println("PS3 Init");

     // the PS3 needs a special command before it will start sending data

     cbuf[i++] = 0x21;
     cbuf[i++] = 0x09;
     cbuf[i++] = 0x03;  //
     cbuf[i++] = 0xF4;	// flip of not little endian
     cbuf[i++] = 0x00;
     cbuf[i++] = 0x00;
     cbuf[i++] = 0x00;	//
     cbuf[i++] = 0x04;	// flip of not little endian

     vdip->portCmd(port,VDIP_DSD,cbuf,100,i);		// send the command
     delay(1000);
     i = 0;

     cbuf[i++] = 0x42;
     cbuf[i++] = 0x0c;
     cbuf[i++] = 0x00;
     cbuf[i++] = 0x00;
     
     vdip->portCmd(port,VDIP_DSD,cbuf,100,i);		// send the command

     return(true);
}

bool driverPS3(byte *data, int count, Gamepad &target)
{
     Serial.println(count);

     if(count != 20) {
	  return(false);
     }
}
#endif
