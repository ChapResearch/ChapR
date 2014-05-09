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
bool   matchMode2 = false;       // matchMode switches from autonomous to teleOp for the user, as opposed to
                                // just teleOp, which is unlimited drive practice

long  target2 = -2500;           // used in determining the mode of the ChapR (and dealing with time)
long  autoStart2;        // TODO
long  teleStart2;

Personality_3::Personality_3() : startedProgram(false)
{
    
}

//
// Loop() - for the cRIO pesonality, a message is sent out for each
//		loop through the Arduino code.  The message is simply the
//		appropriately formatted BT message with the translation of
//		the Gamepads and inclusion of the button.
//
void Personality_3::Loop(BT *bt, int mode, bool button, Gamepad *g1, Gamepad *g2)
{
     byte	msgbuff[64];	// max size of a BT message
     int	size;
     
     if (bt->connected()) {

       // deals with matchMode switching
       if (matchMode2){
	 if (millis() - autoStart2 == myEEPROM.getAutoLen()){
	   cmd = TELE_ON;
	   teleStart2 = millis();
	 }
	 if (millis() - teleStart2 == myEEPROM.getTeleLen()){
	   cmd = TELE_OFF;
	 }
       }

       // first convert the gamepad data and button to the cRIO structure
       size = cRIO.translate(msgbuff,g1,g2);
       
       // compose the message to the cRIO with appropriate framing
       size = cRIO.CMDCompose(msgbuff,cmd,size);
       
       // then send it over BT, again, operating on the message buffer
       (void)bt->btWrite(msgbuff,size);
     }
}

void Personality_3::Kill(BT *bt, int mode)
{
  if (matchMode2){
    beeper.select();
    delay(150);
    beeper.select();
  } else {
    beeper.select();
  }

  if (millis() - target2 <= 2000) //checks to see if the button was pressed twice within 2 seconds
      matchMode2 = !matchMode2;

    target2 = millis();

  /*  if (cmd == TELE_ON || cmd == AUTO_ON){ // if a program is running
    cmd = E_STOP;
    beeper.kill();
    // forceMode = false; <- was in the personality_0 file, but what does it do?
    }*/
}

void Personality_3::ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *gnu)
{
     // nothing happepns here for this personality
}

void Personality_3::ChangeButton(BT *bt, int mode, bool button)
{ 
  if (button){
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
  }
}
