//
// personality_2.cpp
//
//   Implements the personality:  LabView
//

#include <arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "personality.h"
#include "personality_2.h"
#include "robotc.h"
#include "sound.h"
#include "settings.h"
#include "debug.h"

extern sound beeper;
extern settings myEEPROM;

Personality_2::Personality_2() : buttonToggle(false)
{
    
}

//
// Loop() - for the NXT-RobotC pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_2::Loop(BT *bt, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     char       buf[NXT_PRGM_NAME_SIZE];
     int        mode;           // 0 is auto and 1 is tele (but the defines happen to overlap)

     if (bt->connected()) {

       // deals with matchMode switching
       if (isInMatchMode()){
	 if (updateMode()){ // determines if the mode has changed
	   switch (getMatchMode()){
	   case AUTO :                     // just started auto
	     mode = AUTO;
	     Serial.println("----------auto mode-----------");
	     break;
	   case TELE :                     // just became teleOp
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf))
	       beeper.start();
	     mode = TELE;
	     buttonToggle = false;
	     Serial.println("-----------tele mode---------");
	     break;
	   case END :                     // just entered endgame
	     beeper.confirm();
	     Serial.println("-----------end mode-------------");
	     break;
	   case NONE :                    // just ended everything...
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     buttonToggle = false;
	     Serial.println("----------none mode--------");
	     break;
	   }
	 }
       } else {
	 mode = myEEPROM.getMode();
       }
       
       // first convert the gamepad data and button to the robotC structure
       size = robotcTranslate(msgbuff,buttonToggle,g1,g2,mode);
       
       // then compose a NXT mailbox message (for BT transport) with that data
       // this routine operates within the given buffer.  Note that the
       // mailbox used is #0.
       size = nxtBTMailboxMsgCompose(0,msgbuff,size);

       // then send it over BT, again, operating on the message buffer
       (void)bt->btWrite(msgbuff,size);
     }
}


void Personality_2::Kill(BT *bt)
{
  char  buf[NXT_PRGM_NAME_SIZE];

  if (nxtGetProgramName(bt, buf)){ // kill the program if one is running
     if (nxtBTKillCommand(bt)){
         beeper.kill();
     }
     if (isInMatchMode()){
       endCycle();
     }
  } else { // no program running
    if (myEEPROM.matchModeIsEnabled()){
      swapInMatchMode();
    }
  }

  // reset everything
  buttonToggle = false;

}

void Personality_2::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

// NOTE - this is just like the Personality_0::ChangeButton() except that the
//	  button toggles

void Personality_2::ChangeButton(BT *bt, bool button)
{
     char  buf[NXT_PRGM_NAME_SIZE];
     char  buf2[NXT_PRGM_NAME_SIZE];
     
     if (button){
       // swaps buttonToggle
       buttonToggle = !buttonToggle;

       if (!isInMatchMode()){ // normal operation
	 if (nxtGetProgramName(bt, buf)){ // no program is running
	   (buttonToggle && bt->connected())?beeper.beep():beeper.boop();
	 } else {
	   // starts the teleOp program remotely
	   if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	     beeper.start();
	   } else {
	     beeper.icky();
	   }
	   buttonToggle = false;	// always starts as false after starting a program
	   forceMode = true;		// the mode is forced to be teleop until a kill
	 }
       }
       else { // pretty much a single player FCS
	 if (nxtGetProgramName(bt, buf)){ // program is running
	   nxtGetChosenProgram(bt, buf2);
	   if (strcmp(buf, buf2) != 0){ // auto is running
	     buttonToggle = true;
	     beginAuto(); // starts the match cycle at auto (if not already started)
	   } else { // tele is running
	     buttonToggle = true;
	     beginTele(); // starts the match cycle at tele (if not already started)
	     Serial.println("BEGIN TELE!");
	   }
	 } else { // no program running
	   beginAuto(); // waits the auto len even though no program is running
	 }
       }
     }
}
