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

Personality_2::Personality_2() : buttonToggle(false), forceMode(false)
{
    
}

//
// Loop() - for the NXT-RobotC pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_2::Loop(BT *bt, int mode, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     char       buf[NXT_PRGM_NAME_SIZE];

     // override the mode if we have run a program - stays there until a Kill() is called

     if (forceMode) {
       mode = USER_MODE_TELEOP;
     } 

     if (bt->connected()) {

       // deals with matchMode switching
       if (inMatchMode){
	 if (updateMode()){ // determines if the mode has changed
	   switch (getMatchMode()){
	   case AUTO :
	     Serial.println("auto mode");
	     break;
	   case TELE :                     // just became teleOp
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf))
	       beeper.start();
	     buttonToggle = false;
	     Serial.println("tele mode");
	     break;
	   case END :                     // just entered endgame
	     //beeper.warning(); TODO
	     Serial.println("end mode");
	     break;
	   case NONE :
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     Serial.println("none mode");
	     break;
	   }
	 }
       }
	 
	 /* if (teleStart > 0 && millis() - teleStart >= myEEPROM.getTeleLen()*1000){
	   Serial.println("thereotically ended tele");
	   if (nxtBTKillCommand(bt)){
	     beeper.kill();
	   }
	   teleStart = 0;
	   buttonToggle = false;
	 }
	 else if (autoStart > 0 && (millis() - autoStart) >= (myEEPROM.getAutoLen()*1000)){
	   Serial.println("thereotically ended auto");
	   nxtBTKillCommand(bt);
	   if (nxtGetChosenProgram(bt, buf) && nxtRunProgram(bt, buf)){
	     beeper.start();
	   }
	   autoStart = 0;
	   buttonToggle = false; // TODO where is this declared?
	 }
	 }*/
 
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


void Personality_2::Kill(BT *bt, int mode)
{
  char  buf[NXT_PRGM_NAME_SIZE];

  if (nxtGetProgramName(bt, buf)){ // kill the program if one is running
     if (nxtBTKillCommand(bt)){
         beeper.kill();
     }
  } else { // deal with switching modes if no program running
    swapInMatchMode();
  }

  // always turn off forcemode when the Kill is done
  forceMode = false;

  // reset everything
  buttonToggle = false;
  //autoStart = 0;
  //teleStart = 0;

}

void Personality_2::ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happens here for this personality
}

// NOTE - this is just like the Personality_0::ChangeButton() except that the
//	  button toggles

void Personality_2::ChangeButton(BT *bt, int mode, bool button)
{
     char  buf[NXT_PRGM_NAME_SIZE];
     char  buf2[NXT_PRGM_NAME_SIZE];
     
     if (button){ //only executes if the button is in the down position
       if (nxtGetProgramName(bt, buf)){
         // program is running so the action button functions as a WFS button 
         // and is handled by the loop call of the personality
	 buttonToggle = !buttonToggle;
	 (buttonToggle && bt->connected())?beeper.beep():beeper.boop();
	 nxtGetChosenProgram(bt, buf2);
	 if (strcmp(buf, buf2) != 0){ // if the program running is not the teleOp program
	   if (buttonToggle){
	     /* if (autoStart < 0){
	       autoStart = millis() - (-1*autoStart);
	       Serial.println("step 3");
	     } else {
	       autoStart = millis();
	       Serial.println("step 1");
	     }
	     Serial.print("autoStart");
	     Serial.println(autoStart);
	     */
	     if (getMatchMode() == NONE){
	       beginMatchCycle();
	       Serial.println("began match cycle");
	     }
	     else{
	       playMatchCycle();
	       Serial.println("Just theoretically started auto");
	     }
	   }else {
	     pauseMatchCycle();
	   }
	 }else { // teleOp program is running
	   if (buttonToggle){
	     /*if (teleStart < 0){
	       teleStart = millis() - (-1*teleStart);
	       Serial.println("t- step 3");
	     } else {
	       teleStart = millis();
	       Serial.println("t- step 1");
	     }
	     Serial.print("teleStart");
	     Serial.println(teleStart);
	     */
	     // playMatchCycle(); TODO
	     Serial.println("Just theoretically started tele");
	   }else {
	     // pauseMatchCycle(); TODO
	     /*teleStart = teleStart - millis();
	       Serial.println("t- step 2");*/
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
     }
}
