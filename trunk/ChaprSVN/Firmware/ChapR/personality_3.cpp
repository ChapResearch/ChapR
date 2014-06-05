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
#include "settings.h"
#include "debug.h"

cRIO   cRIO;                    // the container for all of the cRIO calls (makes life pretty)
extern settings myEEPROM;
extern sound beeper;

int    cmd = TELE_OFF;          // specifies which command is being sent to the cRIO (teleop on/off
                                // or auto on/off)
bool   buttonToggle = true;

Personality_3::Personality_3() : startedProgram(false)
{
    
}

//
// Loop() - for the cRIO pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_3::Loop(BT *bt, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;

     if (bt->connected()) {

       // deals with matchMode switching
       if (isInMatchMode()){
	 if (updateMode()){ // determines if the mode has changed
	   switch (getMatchMode()){
	   case AUTO :                     // just started auto
	     cmd = AUTO_OFF;    // will be enabled when action button is pressed
	     Serial.println("----------auto mode-----------");
	     break;
	   case TELE :                     // just became teleOp 
	     if (nxtBTKillCommand(bt)) 
	       beeper.kill(); 
	     cmd = TELE_OFF;               // will be enabled when action button is pressed
	     Serial.println("-----------tele mode---------");
	     break;
	   case END :                     // just entered endgame
	     beeper.confirm();
	     Serial.println("-----------end mode-------------");
	     break;
	   case NONE :                    // just ended everything...
	     cmd = TELE_OFF;
	     Serial.println("----------none mode--------");
	     break;
	   }
	 }
       }

       // first create a packet using the cRIO structure
       size = cRIO.createPacket(msgbuff,cmd,g1,g2);
       
       // then send it over BT, again, operating on the message buffer
       (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_3::Kill(BT *bt)
{  
  if (cmd == TELE_ON || cmd == AUTO_ON){
    cmd = TELE_OFF;
     if (isInMatchMode()){
       endCycle();
     }
  } else { // no program running
    if (myEEPROM.matchModeIsEnabled()){
      swapInMatchMode();
    }
  }
}

void Personality_3::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happepns here for this personality
}

void Personality_3::ChangeButton(BT *bt, bool button)
{ 
  /*  if (button){
    if (buttonToggle){
      if (matchMode2){
	cmd = AUTO_ON;
	autoStart2 = millis();
      } else {
	cmd = TELE_ON;
      }
      beeper.beep();
    } else {
      cmd = TELE_OFF;
      beeper.boop();
    }
      buttonToggle = !buttonToggle;
      }*/
     if (button){
       // swaps buttonToggle
       buttonToggle = !buttonToggle;

       if (!isInMatchMode()){ // normal operation
	 cmd = TELE_ON;
	 (buttonToggle && bt->connected())?beeper.beep():beeper.boop();
	 buttonToggle = false;	// always starts as false after starting a program
       }
       else { // pretty much a single player FCS
	 if (cmd == AUTO_OFF){ // auto is "running", but not enabled
	   cmd = AUTO_ON;
	   beginAuto(); // starts the match cycle at auto (if not already started)
	 } else if (cmd == TELE_OFF){ // tele is running
	   cmd = TELE_ON;
	   beginTele(); // starts the match cycle at tele (if not already started)
	   Serial.println("BEGIN TELE!");
	 }
       }
     }
}
