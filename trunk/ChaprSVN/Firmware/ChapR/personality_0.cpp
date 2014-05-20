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

     // override the mode if we have run a program - stays there until a Kill() is called

     if(forceMode) {
       mode = USER_MODE_TELEOP;
     }

     if (bt->connected()) {
       
       // deals with matchMode switching
       /*       if (matchMode){
	 if (updateMode()){ // determines if the mode has changed
	   switch (matchMode){
	   case AUTO :
	     break;
	   case TELE :                     // just became teleOp
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf))
	       beeper.start();
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
	 }*/

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
    /*if (target > 0 && millis() - target <= 3000) // checks to see if the button was pressed twice within 2 seconds
      matchMode = !matchMode;

    if (matchMode){
      beeper.select();
      delay(150);
      beeper.select();
    } else {
      beeper.select();
    }

    target = millis();*/
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

     /*     if (button){ // only executes if the button is in the down position
	  if (nxtGetProgramName(bt, buf)){
	            // program is running so the action button functions as a WFS button 
         // and is handled by the loop call of the personality
	 buttonToggle = !buttonToggle;
	 (buttonToggle && bt->connected())?beeper.beep():beeper.boop();
	 nxtGetChosenProgram(bt, buf2);
	 if (strcmp(buf, buf2) != 0){ // if the program running is not the teleOp program
	   if (buttonToggle){
	     if (autoStart < 0){
	       autoStart = millis() - (-1*autoStart);
	       Serial.println("step 3");
	     } else {
	       autoStart = millis();
	       Serial.println("step 1");
	     }
	     Serial.print("autoStart");
	     Serial.println(autoStart);

	     Serial.println("Just theoretically started auto");
	   }else {
	     autoStart = autoStart - millis();
	     Serial.println("step 2");
	     }
	 }else { // teleOp program is running
	   if (buttonToggle){
	     if (teleStart < 0){
	       teleStart = millis() - (-1*teleStart);
	       Serial.println("t- step 3");
	     } else {
	       teleStart = millis();
	       Serial.println("t- step 1");
	     }
	     Serial.print("teleStart");
	     Serial.println(teleStart);

	     Serial.println("Just theoretically started tele");
	   }else {
	     teleStart = teleStart - millis();
	     Serial.println("t- step 2");
	     }
	 }
       } else { // no program is running
	 if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	   beeper.start();
	 } else {
	   beeper.icky();
	 }
	 buttonToggle = false;	// always starts as false after starting a program
	 forceMode = true;		// the mode is forced to be teleop until a kill
       }
     } else {
       // for this personality, nothing ever happens when the button comes up
       }*/


     /*


	    // program is running so the action button functions as a WFS button 
	    // and is handled by the loop call of the personality
	    beeper.beep();
	  } else {
	    if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	      beeper.start();
	    } else {
	      beeper.icky();
	    }
	    startedProgram = true;
	    forceMode = true;
	  }
     } else {
       // if the last action triggered by having the button down 
       // was starting the program, don't make the "down" sound
       if (startedProgram){
         startedProgram = false;
       } else {
         if (bt->connected()){
           beeper.boop();
         }
       }
       }*/
}

