//
// personality_3.cpp
//
//   Implements the personality:  RIO-LabView
//

#include <Arduino.h>
#include "VDIP.h"
#include "BT.h"
#include "gamepad.h"
#include "nxt.h"
#include "RIO.h"
#include "personality.h"
#include "personality_3.h"
#include "robotc.h"
#include "sound.h"
#include "settings.h"
#include "debug.h"

RIO   RIO;                    // the container for all of the RIO calls (makes life pretty)
extern settings myEEPROM;
extern sound beeper;

//int    cmd = AUTO_OFF;          // specifies which command is being sent to the RIO (teleop on/off
                                // or auto on/off)

Personality_3::Personality_3() : startedProgram(false)
{
    
}

//
// Loop() - for the RIO pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_3::Loop(BT *bt, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     bool       isTele;

     if (bt->connected()) {

       // deals with matchMode switching
       if (isInMatchMode()){
	 if (updateMode()){ // determines if the mode has changed
	   Serial.print("state: ");
	   Serial.println(getMatchMode());
	   switch (getMatchMode()){
	   case AUTO :                     // just started auto
	     // cmd = AUTO_OFF;    // will be enabled when action button is pressed
	     enabled = false;
	     isTele = false;
	     Serial.println("----------auto mode-----------");
	     break;
	   case TELE :                     // just became teleOp 
	     // cmd = TELE_OFF;               // will be enabled when action button is pressed
	     enabled = false;
	     isTele = true;
	     Serial.println("-----------tele mode---------");
	     break;
	   case END :                     // just entered endgame
	     beeper.confirm();
	     Serial.println("-----------end mode-------------");
	     break;
	   case NONE :                    // just ended everything...
	     // cmd = AUTO_OFF;
	     enabled = false;
	     isTele = false;
	     Serial.println("----------none mode--------");
	     break;
	   }
	 }
       } else {
	 if (myEEPROM.getMode() == AUTO){
	   Serial.println("something horrible is happening");
	   isTele = false;
	 } else {
	   isTele = true;
	 }
       }

       bool isRoboRIO = true;

       // first create a packet using the RIO structure
       size = RIO.createPacket(msgbuff,enabled,g1,g2,isTele,isRoboRIO);

       // then send it over BT, again, operating on the message buffer
       (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_3::Kill(BT *bt)
{  
  enabled = false;

  if (isInMatchMode()){
    endCycle();
  }
  else if (myEEPROM.matchModeIsEnabled() && getMatchMode() == NONE){
    swapInMatchMode();
  }
}

void Personality_3::ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happepns here for this personality
}

void Personality_3::ChangeButton(BT *bt, bool button)
{ 
  if (button){
    // swaps enabled
    enabled = !enabled;

    if (!isInMatchMode()){ // normal operation
      (enabled && bt->connected())?beeper.beep():beeper.boop();
      Serial.println("not in match mode");
    }
    else { // pretty much a single player FCS
      Serial.println(getMatchMode());
      if (getMatchMode()==TELE){
	Serial.println("Tele start!");
	beginTele();
      } else if (getMatchMode()==NONE){
	Serial.println("Auto start!");
	beginAuto();
      }
    }
  }
}
