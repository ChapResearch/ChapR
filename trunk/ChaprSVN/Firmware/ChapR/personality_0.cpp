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

Personality_0::Personality_0() : startedProgram(false)
{
    
}

//
// Loop() - for the NXT-RobotC pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_0::Loop(BT *bt, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     char       buf[NXT_PRGM_NAME_SIZE];
     int	mode = myEEPROM.getMode();

     // if the program has just been started, then the button has go up first
     // before it generates the normal "wait for start" - the ChangeButton()
     // deals with reseting the startedProgram

     if(startedProgram) {
       button = false;
     }

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
       size = robotcTranslate(msgbuff,button,g1,g2, mode);

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

void Personality_0::ChangeButton(BT *bt, bool button)
{ 
     char  buf[NXT_PRGM_NAME_SIZE];
     char  buf2[NXT_PRGM_NAME_SIZE];

     if (button){

       if (!isInMatchMode()){ // normal operation
	 if (!nxtGetProgramName(bt, buf)){ // no program is running
	   // starts the teleOp program remotely
	   if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	     beeper.start();
	   } else {
	     beeper.icky();
	   }
	 }
       }
       else { // pretty much a single player FCS
	 if (nxtGetProgramName(bt, buf)){ // program is running
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
       }
     }
}

