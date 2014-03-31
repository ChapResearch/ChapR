//
// personality_3.cpp
//
//   Implements the personality:  cRIO-LabView
//

#include <arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "cRIO.h"
#include "personality.h"
#include "personality_3.h"
#include "robotc.h"
#include "sound.h"

#include "debug.h"


cRIO   cRIO;         // the container for all of the cRIO calls (makes life pretty)

Personality_3::Personality_3() : startedProgram(false)
{
    
}

//
// Loop() - for the cRIO pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_3::Loop(BT *bt, int mode, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     if (bt->connected()) {

	  // first convert the gamepad data and button to the cRIO structure
	  size = cRIO.translate(msgbuff,g1,g2);

	  // compose the message to the cRIO with appropriate framing
	  size = cRIO.CMDCompose(msgbuff,0,size);

	  // then send it over BT, again, operating on the message buffer
	  (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_3::Kill(BT *bt, int mode)
{
     extern sound beeper;
}

void Personality_3::ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

void Personality_3::ChangeButton(BT *bt, int mode, bool button)
{ 

}
