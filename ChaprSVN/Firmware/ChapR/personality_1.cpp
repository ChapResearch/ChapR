//
// personality_1.cpp
//
//   Implements the personality:  NXT-G
//

#include <Arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "nxtg.h"
#include "personality.h"
#include "personality_1.h"
#include "sound.h"

#include "debug.h"

extern sound beeper;

//
// ChangeInput() - called when one of the input devices change.  This routine
//		   gets the whole state of the gamepad, so it needs to figure
//		   out what has changed, and send the appropriate message
//		   for that change.  Fortunately, the called is kind enough
//		   to send the old reading along with the new one to make
//		   it relatively easy to figure out.
//		   (see personality_1.h for a description of the messages)
//
void Personality_1::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     if (device != 1) {
	  return;
     }

     if (!bt->connected()) {
	  return;
     }

     if (gnu->x1 != old->x1) {		// if change, scale to 100, send on 1
	  mySendMessageInt(bt,1,gnu->x1);
     }
     if (gnu->y1 != old->y1) {		// if change, scale to 100, send on 2
	  mySendMessageInt(bt,2,gnu->y1);
     }
     if (gnu->x2 != old->x2) {		// if change, scale to 100, send on 3
	  mySendMessageInt(bt,3,gnu->x2);
     }
     if (gnu->y2 != old->y2) {		// if change, scale to 100, send on 4
	  mySendMessageInt(bt,4,gnu->y2);
     }

     if (GAMEPAD_B1(gnu) != GAMEPAD_B1(old)) {		// if change, send on 5
	  mySendMessageBool(bt,5,GAMEPAD_B1(gnu));
     }
     if (GAMEPAD_B2(gnu) != GAMEPAD_B2(old)) {		// if change, send on 6
	  mySendMessageBool(bt,6,GAMEPAD_B2(gnu));
     }
     if (GAMEPAD_B3(gnu) != GAMEPAD_B3(old)) {		// if change, send on 7
	  mySendMessageBool(bt,7,GAMEPAD_B3(gnu));
     }
     if (GAMEPAD_B4(gnu) != GAMEPAD_B4(old)) {		// if change, send on 8
	  mySendMessageBool(bt,8,GAMEPAD_B4(gnu));
     }
}

// this little define scales the joystick value from -100 to 100

#define GPADSCALE(x)	(x+((x<0)?0:1))*100/128

void Personality_1::mySendMessageInt(BT *bt,int mbox,int value)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     size = nxtGInt(msgbuff,GPADSCALE(value));
     size = nxtBTMailboxMsgCompose(mbox,msgbuff,size);
     (void)bt->btWrite(msgbuff,size);
}

void Personality_1::mySendMessageBool(BT *bt,int mbox, bool value)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     size = nxtGBool(msgbuff,value);
     size = nxtBTMailboxMsgCompose(mbox,msgbuff,size);
     (void)bt->btWrite(msgbuff,size);
}

//
// ChangeButton() - called when the button state changes.       
// 			send the status of the button on mailbox 0
//
void Personality_1::ChangeButton(BT *bt, bool button)
{
     // in the other personalities, this button could possibly start a program
     // for for this personality, there is no way to know what program to start

     if (bt->connected()) {
	  mySendMessageBool(bt,0,button);
     }
}

void Personality_1::Kill(BT *bt)
{
     if (nxtBTKillCommand(bt)){
         beeper.kill();
     }
}

void Personality_1::Loop(BT *bt, Gamepad *g1, Gamepad *g2)
{
     // nothing happens here for this personality
}
