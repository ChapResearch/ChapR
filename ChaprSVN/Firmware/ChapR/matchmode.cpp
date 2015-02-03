//
// matchmode.cpp
//
//   The idea behind this object is to encapsulate all of the "matchmode" stuff.  This
//   means both the processing of a match itself as well as switching in and out of
//   matchmode.
//
//   The implementation of matchmode is a little overkill at this point. It is "event
//   driven" where as events occur, call-backs are made to the current personality
//   object so that it can decide what to do when that event occurs.  Some events
//   aren't used by anyone currently, but have very little overhead fortunately.
//
//   Here is a list of the states that occur within matchmode, and what callbacks get
//   triggered for the personality and when.  In this list the "Event" is what happens
//   in the system, and how it resonds.  For example, the Event "Button" means that the
//   action button is pressed.  The Event "Kill" means that the power button is pressed.
//   The "(timer)" event means that the timer has gone off.  And the "(entry)" event
//   simply means that the new state has been entered (just used to indicate callbacks).
//
//   You'll see that some of the calls have ANDs (&&) and ORs (||) between them.  This
//   indicates that the first call is done, and depending upon the outcome, the second
//   one is done.  For &&, this effectively means that if the first one returns TRUE
//   that the second should be called.  For || (or), this means that the second one is
//   going to be called after the first, no matter what.
//
// State	    Event	Calls							Meaning
// ------------	    -----------	-------------------------------------------------------	-----------------
// MM_OFF	    (entry)	timerOff()
//		    Button	nextState(MM_AUTO_PREP)					start of a match
//		    Kill	nextState(MM_OFF)					(useless) kill of the match
//
// MM_AUTO_PREP	    (entry)	callback(MM_AUTO_PREP) && nextState(MM_AUTO_START)
//		    Button	nextState(MM_AUTO_START)
//		    Kill	nextState(MM_KILL)
//
// MM_AUTO_START    (entry)	callback(MM_AUTO_START) || timerstart(AUTOTIME)
//		    (timer)	nextState(MM_AUTO_END)
//		    Kill	nextState(MM_KILL)
//
// MM_AUTO_END	    (entry)	callback(MM_AUTO_END) || nextState(MM_TELEOP_PREP)
//
// MM_TELEOP_PREP   (entry)	callback(MM_TELEOP_PREP) && nextState(MM_TELEOPSTART)
//		    Button	nextState(MM_TELEOPSTART)
//		    Kill	nextState(MM_KILL)
//
// MM_TELEOP_START  (entry)	callback(MM_TELEOP_START) && timerstart(TELEOPTIME)
//		    (timer)	nextState(MM_ENDGAME_START)
//		    Kill	nextState(MM_KILL)
//
// MM_ENDGAME_START (entry)	callback(MM_ENDGAME_START) || timerstart(ENDGAMETIME)
//		    (timer)	nextState(MM_ENDGAME_END)
//		    Kill	nextState(MM_KILL)
//
// MM_ENDGAME_END   (entry)	callback(MM_ENDGAME_END) || nextState(MM_TELEOP_END)
//
// MM_TELEOP_END    (entry)	callback(MM_TELEOP_END) || nextState(MM_OFF)
//
// MM_KILL	    (entry)	callback(MM_KILL) || nextState(MM_OFF)
//

#include <Arduino.h>
#include "matchmode.h"
#include "settings.h"
#include "sound.h"

// note that the sound is necessary because the matchmode active/inactive sound is
// done in this routine.  Settings is necessary because the settings are checked
// to see if matchmode should ever be active (in other words, is it enabled).  Settings
// are also used to see how long the auto, teleop, and end periods are.

extern settings myEEPROM;
extern sound beeper;

MatchMode::MatchMode() : active(false),
			 currentState(MM_OFF),
			 timerActive(false),
			 lastPressTime(0)
{
}

//
// isMatchActive() - returns TRUE if matchmode is currently active.  Note that it is NEVER
//			active if matchmode is disabled in EEPROM.  If not diabled in EEPROM,
//			then matchmode can become active.
//
bool MatchMode::isMatchActive()
{
     return(active);
}

//
// isEnabled() - checks the EEPROM to see if matchmode is enabled.  Returns TRUE if it is enabled.
//
bool MatchMode::isMatchEnabled()
{
     return(myEEPROM.matchModeIsEnabled());
}

//
// Button() - Should be called by the object owner when the button is pressed down.
//	      The matchmode state is changed appropriately.  Note that this is only
//	      useful for a few states.  Note, too, that sometimes, like for AUTO_PREP
//	      this doesn't normally get called because the button isn't required to
//	      start the auto period.
//
void MatchMode::MatchButtonProcess(void *rock_incoming)
{
     rock = rock_incoming;

     switch(currentState){

     case MM_OFF:
	  enterState(MM_AUTO_PREP);
	  break;

     case MM_AUTO_PREP:
	  enterState(MM_AUTO_START);
	  break;

     case MM_TELEOP_PREP:
	  enterState(MM_TELEOP_START);
	  break;

     default:
	  break;
     }
}

//
// Kill() - process the Kill event for each state
//	    Called by the object owner when the Kill is pressed.
//	    Should only be called if match mode is enabled, but just in case...
//	    When the kill switch isn't active, it switches match mode in/out.
//
//	Note that the MATCHMODE_SWITCH_DELAY is the number of milliseconds between
//	kill's that cause the matchmode to switch.
//
#define MATCHMODE_SWITCH_DELAY	2000
void MatchMode::MatchKillProcess(void *rock_incoming)
{
     rock = rock_incoming;

     if(isMatchEnabled()) {
	  switch(currentState) {

	  case MM_OFF:
	       // if we're in the MM_OFF state, this is the only time when we can
	       // switch match modes.  BUT - when the kill button is hit, even if
	       // we DO switch modes, we first need to kill what is currently going
	       // on.  So to do this, we go ahead and jump to the MM_KILL state
	       // which will call the kill callback, and then jump back to this state.
	       // Note that this is done RIGHT AWAY because kill should happen immediately

	       enterState(MM_KILL);

	       // check to see if the kill switch was pressed twice within the MATCHMODE_SWITCH_DELAY
	       // if so, deactivate matchmode

	       if (lastPressTime > 0 && millis() - lastPressTime <= MATCHMODE_SWITCH_DELAY) {
		    active = !active;
		    beeper.select();
		    if (active){
			 delay(150);
			 beeper.select();
		    }
	       }
	       lastPressTime = millis();
	       break;

	       // in any other state, the kill is issued
	  default:
	       enterState(MM_KILL);
	       break;
	  }
     }
}

//
// enterState() - implement the (entry) of a new state.  Note that this routine can be recursive,
//		  as it jumps from state to state
//
void MatchMode::enterState(mmState newState)
{
     currentState = newState;

     switch(newState) {

     case MM_OFF:
	  callBack();
	  timerOff();
	  break;

     case MM_AUTO_PREP:
	  if(callBack()) {	 // if it returns TRUE, then we go on, not waiting for a button press
	       enterState(MM_AUTO_START);
	  }
	  break;

    case MM_AUTO_START:
	 callBack();
	 timerStart(myEEPROM.getAutoLen()*1000);
	 break;

    case MM_AUTO_END:
	 callBack();
	 enterState(MM_TELEOP_PREP);
	 break;

    case MM_TELEOP_PREP:
	 if(callBack()) {	// if it returns TRUE, then we go on, not waiting for a button press
	       enterState(MM_TELEOP_START);
	 }
	 break;
	 
    case MM_TELEOP_START:
	 callBack();
	 timerStart(myEEPROM.getTeleLen()*1000);
	 break;

    case MM_ENDGAME_START:
	 callBack();
	 timerStart(myEEPROM.getEndLen()*1000);
	 break;

    case MM_ENDGAME_END:
	 callBack();
	 enterState(MM_TELEOP_END);
	 break;

    case MM_TELEOP_END:				// both TELEOP_END and KILL have the same behavior
    case MM_KILL:				//   do the callback, then go to OFF
	 callBack();
	 enterState(MM_OFF);
	 break;
    }
}

//
// Loop() - called during the standard processing loop.  Only used to notice when
//	    the timers go off.  Button presses are processed elsewhere.
//
void MatchMode::MatchLoopProcess(void *rock_incoming)
{
     rock = rock_incoming;

     if(timerExpiredEvent()) {
	  timerOff();

	  switch(currentState) {

	  case MM_AUTO_START:
	       enterState(MM_AUTO_END);
	       break;

	  case MM_TELEOP_START:
	       enterState(MM_ENDGAME_START);
	       break;

	  case MM_ENDGAME_START:
	       enterState(MM_ENDGAME_END);
	       break;

	  default:
	       break;
	  }
     }
}

void MatchMode::timerStart(int timerMillis)
{
     timerActive = true;
     timerTarget = millis() + timerMillis;
}

//
// timerExpiredEvent () - returns true if the a "timer expired" event has occured.  This
//		    means that the timer was active, and has reached its time.
//
bool MatchMode::timerExpiredEvent()
{
     return(timerActive && millis() > timerTarget);
}

void MatchMode::timerOff()
{
     timerActive = false;
}

bool MatchMode::callBack()
{
	return(matchStateProcess(currentState,rock));
}

void MatchMode::MatchReset()
{
  currentState = MM_OFF;
  timerOff();
}
