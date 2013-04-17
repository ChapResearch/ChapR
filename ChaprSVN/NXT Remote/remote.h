//
// REMOTE - this file, along with remote.c, implement the PC-less remote control.
//

//
//  The remote control is layout like this:
//
//	  LEFT 	     ,--------------------,      RIGHT
//     1---, 2---,   | ,----------------, |   1---, 2---,
//     | + | | + |   | |()()        ()()| |   | + | | + |
//     '---' '---'   | |()() 	    ()()| |   '---' '---'
//     3---, 4---,   | |()()   	    ()()| |   3---, 4---,
//     | + | | + |   | |  () 	    ()  | |   | + | | + |
//     '---' '---'   | |     	        | |   '---' '---'
//     5---, 6---,   | '----------------' |   5---, 6---,
//     | + | | + |   |    .-, ,--, ,-.    |   | + | | + |
//     '---' '---'   |   < L| |Y | |R >   |   '---' '---'
//     7---, 8---,   |    '-' '--' '-'    |   7---, 8---,
//     | + | | + |   |        ,--,        |   | + | | + |
//     '---' '---'   |        '--'        |   '---' '---'
//     	       	     '--------------------'
//
//  The remote check routine returns a short (16 bits) with
//  with the 16 buttons encoded within them.  Note that there
//  may NOT be 16 buttons in any implementations (including
//  the first that this code supports).  The state of the
//  buttons is return in two bytes:
//
//            LEFT (MSB)         RIGHT (LSB)
//          _ _ _ _ _ _ _ _    _ _ _ _ _ _ _ _
//  ushort |8|7|6|5|4|3|2|1|  |8|7|6|5|4|3|2|1|
//
//  This encoding is what's passed around.  When the buttons'
//  hook-up configuration changes, this remains the same.
//
//  The three control buttons, L, Y, and R can also be programmed.
//

//
// The following enum represents the values for the "Button Map" for translating
// remote buttons to joystick buttons/values.
//
typedef enum {
	NOTHING,

	RELEASE,		// this causes the waitForStart() to be released

	// buttons for the controller 1

	C1_Button1, C1_Button2, C1_Button3, C1_Button4,
	C1_Button5, C1_Button6, C1_Button7, C1_Button8,
	C1_Button9, C1_Button10, C1_Button11, C1_Button12,

	// joysticks on the controllers are mapped to their extremes when
	// buttons are pressed, if none then the joystick is considered to
	// be in the center (0)

	C1_X1_East, C1_X1_West,
	C1_Y1_North, C1_Y1_South,
	C1_X2_East, C1_X2_West,
	C1_Y2_North, C1_Y2_South,

	// The TopHat is divided into quadrants

	C1_TH_0, C1_TH_1, C1_TH_2, C1_TH_3, C1_TH_4, C1_TH_5, C1_TH_6, C1_TH_7, 

	// buttons for controller 2

	C2_Button1, C2_Button2, C2_Button3, C2_Button4,
	C2_Button5, C2_Button6, C2_Button7, C2_Button8,
	C2_Button9, C2_Button10, C2_Button11, C2_Button12,

	C2_X1_East, C2_X1_West,
	C2_Y1_North, C2_Y1_South,
	C2_X2_East, C2_X2_West,
	C2_Y2_North, C2_Y2_South,

	C2_TH_0, C2_TH_1, C2_TH_2, C2_TH_3, C2_TH_4, C2_TH_5, C2_TH_6, C2_TH_7

} BUTTONTRANSLATION;

// now include the joystick structure and such - remember, though,
// that a joystick read bluetooth task gets started.  This is
// stopped in the remote.c main program.

#include "JoystickDriver.c"
