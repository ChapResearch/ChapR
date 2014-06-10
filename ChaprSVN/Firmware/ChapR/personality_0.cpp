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
#include "settings.h"

#include "debug.h"

extern sound beeper;
extern settings myEEPROM;
 
Personality_0::Personality_0() : buttonToggle(false)
{
}

//
// Loop() - for the NXT-RobotC pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_0::Loop(BT *bt, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     char       buf[NXT_PRGM_NAME_SIZE];
     int	mode = myEEPROM.getMode();

     // if the program has just been started, then the button has go up first
     // before it generates the normal "wait for start" - the ChangeButton()
     // deals with reseting the startedProgram

     if (bt->connected()) {
       
       // deals with matchMode switching
       if (isInMatchMode()){
	 if (updateMode()){ // determines if the mode has changed
	   switch (getMatchMode()){
	   case AUTO :
	     mode = AUTO;
	     break;
	   case TELE :                     // just became teleOp
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf))
	       beeper.start();
	     mode = TELE;
	     break;
	   case END :                     // just entered endgame
	     //beeper.warning(); TODO
	     break;
	   case NONE :
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     break;
	   }
	 }
       } else {
	 mode = myEEPROM.getMode();
       }

       // first convert the gamepad data and button to the robotC structure
       size = robotcTranslate(msgbuff,enabled,g1,g2, mode);

       // then compose a NXT mailbox message (for BT transport) with that data
       // this routine operates within the given buffer.  Note that the
       // mailbox used is #0.
       size = nxtBTMailboxMsgCompose(0,msgbuff,size);

       // then send it over BT, again, operating on the message buffer
       (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_0::Kill(BT *bt)
{
  char  buf[NXT_PRGM_NAME_SIZE];

  if (nxtGetProgramName(bt, buf)){ // kill the program if one is running
     if (nxtBTKillCommand(bt)){
         beeper.kill();
     }
  } else { // deal with switching modes if no program running
    swapInMatchMode();
  }
}

void Personality_0::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

void Personality_0::ChangeButton(BT *bt, bool buttonIsDown)
{ 
     char  buf[NXT_PRGM_NAME_SIZE];
     char  buf2[NXT_PRGM_NAME_SIZE];

     if (isInMatchMode()) {

	 if (buttonIsDown && nxtGetProgramName(bt, buf)){ // program is running
	      nxtGetChosenProgram(bt, buf2);
	      if (strcmp(buf, buf2) != 0){ // auto is running
		   beginAuto(); // starts the match cycle at auto (if not already started)
	      } else { // tele is running
		   beginTele(); // starts the match cycle at tele (if not already started)
		   Serial.println("BEGIN TELE!");
	      }
	 } else { // no program running
	      beginAuto(); // waits the auto len even though no program is running
	 }

     } else { // normal operation here - no match mode

	  // in normal operation, pressing the action button starts a program
	  // if a program is already running, turn on/off enabled

	  if (buttonIsDown) {

	       // try to get the name of the program running, if there isn't one it returns false

	       if(nxtGetProgramName(bt, buf)){
		    // program is running so just turn on enabled (in FTC-speak this releases wait-for-start)
		    // (or toggle it in when buttonToggle is set - like in Labview personality)
		    if(buttonToggle) {
			 enabled = !enabled;
		    } else {
			 enabled = true;
		    }
	       } else {
		    // no program is running, so try to start it
		    if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
			 beeper.start();
			 enabled = false;		// always start off disabled UNTIL the button goes up & down again
		    } else {
			 beeper.icky();
		    }
	       }
	  } else { 		// when the button moves to up, either disable or do nothing when in toggling mode
	       if(!buttonToggle) {
		    enabled = false;
	       }
	  }
     }
}
