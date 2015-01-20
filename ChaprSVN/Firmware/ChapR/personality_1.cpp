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

// this little define scales the joystick value from -100 to 100

#define GPADSCALE(x)	(x+((x<0)?0:1))*100/128

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
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     if (device != 1) {
	  return;
     }

     if (!bt->connected()) {
	  return;
     }

     if (gnu->x1 != old->x1) {		// if change, scale to 100, send on 1
	  size = nxtGInt(msgbuff,GPADSCALE(gnu->x1));
	  size = nxtBTMailboxMsgCompose(1,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (gnu->y1 != old->y1) {		// if change, scale to 100, send on 2
	  size = nxtGInt(msgbuff,GPADSCALE(gnu->y1));
	  size = nxtBTMailboxMsgCompose(2,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (gnu->x2 != old->x2) {		// if change, scale to 100, send on 3
	  size = nxtGInt(msgbuff,GPADSCALE(gnu->x2));
	  size = nxtBTMailboxMsgCompose(3,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (gnu->y2 != old->y2) {		// if change, scale to 100, send on 4
	  size = nxtGInt(msgbuff,GPADSCALE(gnu->y2));
	  size = nxtBTMailboxMsgCompose(4,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }

     if (GAMEPAD_B1(gnu) != GAMEPAD_B1(old)) {		// if change, send on 5
	  size = nxtGBool(msgbuff,GAMEPAD_B1(gnu));
	  size = nxtBTMailboxMsgCompose(5,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (GAMEPAD_B2(gnu) != GAMEPAD_B2(old)) {		// if change, send on 6
	  size = nxtGBool(msgbuff,GAMEPAD_B2(gnu));
	  size = nxtBTMailboxMsgCompose(6,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (GAMEPAD_B3(gnu) != GAMEPAD_B3(old)) {		// if change, send on 7
	  size = nxtGBool(msgbuff,GAMEPAD_B3(gnu));
	  size = nxtBTMailboxMsgCompose(7,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
     if (GAMEPAD_B4(gnu) != GAMEPAD_B4(old)) {		// if change, send on 8
	  size = nxtGBool(msgbuff,GAMEPAD_B4(gnu));
	  size = nxtBTMailboxMsgCompose(8,msgbuff,size);
	  (void)bt->btWrite(msgbuff,size);
     }
}

//
// ChangeButton() - called when the button state changes.       
// 			send the status of the button on mailbox 0
//
void Personality_1::ChangeButton(BT *bt, bool button)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     if (!bt->connected()) {
	  return;
     }

     // in the other personalities, this button could possibly start a program
     // for for this personality, there is no way to know what program to start

     size = nxtGBool(msgbuff,button);
     size = nxtBTMailboxMsgCompose(0,msgbuff,size);
     (void)bt->btWrite(msgbuff,size);
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
