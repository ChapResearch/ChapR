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
	C1_TH_0,		// Right 1 - Linear Slide up (THup)
	C1_Button7,		// Right 2 - Basket Arm up
	C1_TH_4,		// Right 3 - Linear Slide down
	C1_Button8,		// Right 4 - Basket Arm down
	C1_Button5,		// Right 5 - Basket Spin CW
	NOTHING,		// Right 6
	NOTHING,		// Right 7
	NOTHING,		// Right 8
	C1_X1_West,		// Left 1 - Drive Left
	C1_Y1_North,		// Left 2 - Drive Forward
	C1_Y1_South,		// Left 3 - Drive Back
	C1_X1_East,		// Left 4 - Drive Right
	NOTHING,		// Left 5 - 
	C1_Button6,		// Left 6 - Basket Spin CCW
	NOTHING,		// Left 7
	NOTHING,		// Left 8
	C1_X2_West,		// NXT Left - Bot Spin CCW
	C1_X2_East,		// NXT Right - Bot Spin CW
	};

#include "remote.c"
