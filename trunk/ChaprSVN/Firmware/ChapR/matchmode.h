//
// matchmode.h
//
//    Implements the match-mode functionality.
//
//    Match-mode implements automatic match progression from autonomous,
//    through teleop, then ending with end-game.  It knows how to interface
//    with personalities to call the right functions during the match.
//    
//    See the .cpp for information on the state progression.
//
//
// Match Mode States
// -----------------
// MM_OFF           - the initial state of a match, also the "go-to" state when match is done
// MM_AUTO_PREP     - a match is starting where we first prep the autonomous - may start a program
// MM_AUTO_START    - starts the autonomous phase of the match - normally just enables the bot
// MM_AUTO_END      - ends the autonomous phase of the match - may kill programs and/or disable the bot
// MM_TELEOP_PREP   - teleop mode preparation state - may start a program
// MM_TELEOP_START  - starts the autonomous phase of the match - normally just enables the bot
// MM_ENDGAME_START - "end-game" is a sub-phase of teleop so no prep needed
// MM_ENDGAME_END   - end-game is over
// MM_TELEOP_END    - ends the autonomous phase of the match - may kill programs and/or disable the bot
// MM_KILL	    - the state where are killing the match - really just calls the callback and goes to MM_OFF
//

#ifndef MATCHMODE_H
#define MATCHMODE_H

typedef enum mmState {
     MM_OFF,
     MM_AUTO_PREP,
     MM_AUTO_START,
     MM_AUTO_END,
     MM_TELEOP_PREP,
     MM_TELEOP_START,
     MM_ENDGAME_START,
     MM_ENDGAME_END,
     MM_TELEOP_END,
     MM_KILL
} mmState;

class MatchMode
{
private:
     bool		active;			// true if matchmode is active currently
     mmState		currentState;
     void 		enterState(mmState);	// used when moving between states, runs the state entry stuff
     bool		callBack();		

     bool		timerActive;
     long		timerTarget;
     void		timerStart(int);	// fires-up the timer for a certainnumber of seconds
     void		timerOff();		// turns-off the timer
     bool		timerExpiredEvent();	// returns true if a timer has "gone off"

     long		lastPressTime;		// use to monitor matchmode switching with KILL switch

     void		*rock;			// the rock I am handed to pass to callbacks

public:
     MatchMode();

     void MatchLoopProcess(void*);
     void MatchButtonProcess(void*);
     void MatchKillProcess(void*);

     bool isMatchActive();		// returns TRUE if matchmode is active
     bool isMatchEnabled();		// returns TRUE if matchmode is enabled

     virtual bool matchStateProcess(mmState,void*) = 0;

};

#endif MATCHMODE_H
