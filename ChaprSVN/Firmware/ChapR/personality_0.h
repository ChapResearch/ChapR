//
// personality_0.h
//
//   This file (along with the associated personality_0.cpp) implement the Personality #0, which is
//
//		NXT-RobotC
//
//   This personality is used to control NXT robots when using RobotC libraries and programs on
//   the NXT.
//
//   This particular personality sends messages to the NXT for each loop.  In fact, it doesn't
//   really care about changes...except KILL of course.  All gamepad/button data is sent in
//   each message.  (May want to look into only sending upon change, but also with a keep alive
//   that is sent periodically)
//

#ifndef PERSONALITY_0_H
#define PERSONALITY_0_H

#define MODE_TELEOP	1
#define MODE_AUTO	0

class Personality_0 : public Personality, public MatchMode
{

private:
     void myKill(BT *bt);

public:
     Personality_0();
     virtual void ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, bool button);
     virtual void Loop(BT *bt, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt);

     bool	 buttonToggle;		// used to swich between momentary action and toggle action

     virtual bool	 matchStateProcess(mmState,void*);

     int	 mode;			// either MODE_TELEOP or MODE_AUTO
     bool	 enabled;		// either TRUE when enabled for FALSE
};


#endif PERSONALITY_0_H
