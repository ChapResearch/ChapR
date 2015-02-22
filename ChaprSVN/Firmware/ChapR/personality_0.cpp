//
// personality_0.cpp
//
//   Implements the personality:  NXT-RobotC
//
#include <Arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "matchmode.h"
#include "personality.h"
#include "personality_0.h"
#include "robotc.h"
#include "sound.h"
#include "settings.h"

#include "debug.h"

extern sound beeper;
extern settings myEEPROM;

//
// matchStateProcess() - the matchmode callback that is used by matchmode to do
//		  whatever needs to be done with the state changes during
//		  match mode.  The NEW state that was just entered is
//		  passed to the callback routine.
//
//	NOTE: this routine is handed the "rock" that was given to it to, so that
//		it could be handed back during callbacks.
//
//	NOTE: Processing of the MM_OFF state needs to be "idempotent" - it should
//		be callable any number of times.
//
bool Personality_0::matchStateProcess(mmState mmState, void *rock)
{
     switch(mmState) {
     case MM_OFF:
	  enabled = false;	// ensure we are always disabled when in OFF state
	  mode = MODE_AUTO; 
	  break;

     case MM_AUTO_PREP:		// prepare for autonomous - allow the standard "true" to be returned
	  mode = MODE_AUTO;	//   because we don't wait for anything to get going in autonomous
	  break;

     case MM_AUTO_START:
          beeper.beep();
	  enabled = true;
	  break;

     case MM_AUTO_END:		// autonomous is ending
	  myKill((BT *)rock);	// this does the end program falling sound too
	  break;

     case MM_TELEOP_PREP:      	// prepare for teleop
	  mode = MODE_TELEOP;		// Note that this does the start program rising sound too
	  myTeleopStart((BT*)rock);	// also turns OFF enabled
	  return(false);		// return false to cause wait for button press to start teleop

     case MM_TELEOP_START:	// teleop is starting
          beeper.beep();
	  enabled = true;
	  break;

     case MM_ENDGAME_START:	// endgame (within teleop) is starting
          beeper.beep();
	  break;

     case MM_TELEOP_END:	// teleop is ending
	  // FALL THROUGH to next case - 'cause we need to kill the program at the end of Teleop

     case MM_KILL:		// the match as been killed
	  myKill((BT*)rock);	// kill any running program
	  break;

     default:
	  // we don't need MM_ENDGAME_END at this point
	  break;
     }

     return(true);
}

Personality_0::Personality_0()
{
     buttonToggle = false;
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

     // if we're not connected to Bluetooth, then ingore the loop
     if (!bt->connected()) {
          enabled = false;
	  if (isMatchActive()){
	    MatchReset();
	  }
	  return;
     }
       
     // only deal with matchmode when it is active

     if (isMatchActive()){
	     MatchLoopProcess((void *)bt);	// mode is set in the match callback above
     } else {
	     mode = myEEPROM.getMode();		// mode is set by the EEPROM setting
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

//
// Kill() - called when someone presses the "kill" button (the power button)
//
void Personality_0::Kill(BT *bt)
{
  // it is the Kill() that will turn matchmode active, so process all kills when enabled

  if(isMatchEnabled()) {
	  MatchKillProcess((void *)bt);
  } else {
	  myKill(bt);
  }
}

// 
// myKill() - the low-level routine that is used to kill a program on the NXT
//		after it is determined that we SHOULD kill the program
//
void Personality_0::myKill(BT *bt)
{
	char  buf[NXT_PRGM_NAME_SIZE];

	if(bt->connected()) {
		if (nxtGetProgramName(bt, buf)){ // kill the program if one is running
			if (nxtBTKillCommand(bt)){
				beeper.kill();
			}
		}
	}
}

//
// myTeleopStart() - start the Teleop program
//
void Personality_0::myTeleopStart(BT *bt)
{
     char  buf[NXT_PRGM_NAME_SIZE];

     if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	  beeper.start();
	  enabled = false;		// always start off disabled UNTIL the button goes up & down again
     } else {
	  beeper.icky();
     }
}


void Personality_0::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

void Personality_0::ChangeButton(BT *bt, bool buttonIsDown)
{ 
     char  buf[NXT_PRGM_NAME_SIZE];

     // only do the match stuff if matchmode is currently active

     if (isMatchActive()) {
       if (buttonIsDown){
	 MatchButtonProcess((void *)bt);
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

		    // always generate a beep one way or the other
		    (enabled)?beeper.beep():beeper.boop();

	       } else {
		    // no program is running, so try to start it
		    myTeleopStart(bt);
	       }
	  } else { 	

	       // when the button moves to up, either disable, or do nothing when in toggling mode

	       if(!buttonToggle) {
		    if(enabled) {		// after starting a program, enabled is left false so that
			 beeper.boop();		//   it will only be true after the button is pressed down AGAIN.
		    }
		    enabled = false;
	       }
	  }
     }
}
