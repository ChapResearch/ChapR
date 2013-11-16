//
// personality_0.cpp
//
//   Implements the personality:  NXT-RobotC
//

#include <arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "personality.h"
#include "personality_0.h"
#include "robotc.h"
#include "sound.h"

#include "debug.h"

//
// Loop() - for the NXT-RobotC pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_0::Loop(BT *bt, int mode, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     if (bt->connected()) {

	  // first convert the gamepad data and button to the robotC structure
	  size = robotcTranslate(msgbuff,button,g1,g2, mode);
	  
	  // then compose a NXT mailbox message (for BT transport) with that data
	  // this routine operates within the given buffer.  Note that the
	  // mailbox used is #0.
	  size = nxtBTMailboxMsgCompose(0,msgbuff,size);

//	  dumpDataHex("out2",msgbuff,size);

	  // then send it over BT, again, operating on the message buffer
	  (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_0::Kill(BT *bt, int mode)
{
     byte	outbuff[64];
     int	size;

     if (bt->connected()) {
	  size = nxtBTKillCommand(outbuff);
	  (void)bt->btWrite(outbuff,size);
     }
}

void Personality_0::ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

void Personality_0::ChangeButton(BT *bt, int mode, bool button)
{ 
     extern sound beeper;
     // nothing happens here for this personality
     char  buf[NXT_PRGM_NAME_SIZE];
     
     if (button){ //only executes if the button is in the down position
       if (nxtGetProgramName(bt, buf)){
  
       } else {
         if (nxtGetChosenProgram(bt, buf)){
             nxtRunProgram(bt, buf);
         }
       }
     }
}
