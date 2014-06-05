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
//     '---' '---'   |   '._| |__| |_.'   |   '---' '---'
//     7---, 8---,   |        ,--,        |   7---, 8---,
//     | + | | + |   |        '--'        |   | + | | + |
//     '---' '---'   |      	          |   '---' '---'
//     	       	     '--------------------'
//
// HOWEVER, the current implementation only defines buttons
// Left 1-4,6 and Right 1-4,5.

#include "remote.h"

// The ButtonTranslation[] array defines what happens when
// each button is pressed.  The array MUST be named this
// way. See remote.h for definitions of the mapping values.
//
// The three NXT buttons Left, Right, and Orange can be used
// too.  The translation array looks like this:
//
//   [0] - NXT Orange Button
//   [1] - Right 1
//   [2] - Right 2 
//   [3] - Right 3
//   [4] - Right 4
//   [5] - Right 5
//   [6] - Right 6
//   [7] - Right 7
//   [8] - Right 8
//   [9] - Left 1
//   [10] - Left 2
//   [11] - Left 3
//   [12] - Left 4
//   [13] - Left 5
//   [14] - Left 6
//   [15] - Left 7
//   [16] - Left 8
//   [17] - NXT Left
//   [18] - NXT Right

BUTTONTRANSLATION ButtonTranslation[19] = {
	RELEASE,		// NXT Orange - release waitForStart();
	C2_Button2,		// Right 1 - Arm Out
	C2_Button5,		// Right 2 - Lift Up
	C2_Button4,		// Right 3 - Arm In
	C2_Button7,		// Right 4 - Lift Down
	C2_Button8,		// Right 5 - Balls out
	NOTHING,		// Right 6
	NOTHING,		// Right 7
	NOTHING,		// Right 8
	C1_Button5,		// Left 1 - Left Claw Open
	C1_Button6,		// Left 2 - Right Claw Open
	C1_Button7,		// Left 3 - Left Claw Close
	C1_Button8,		// Left 4 - Right Claw Close
	NOTHING,		// Left 5 - 
	C2_Button6,		// Left 6 - Balls in
	NOTHING,		// Left 7
	NOTHING,		// Left 8
	C1_Y1_North,		// NXT Left
	C1_Y2_North,		// NXT Right
	};

#include "remote.c"
