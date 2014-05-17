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

class Personality_0 : public Personality
{

private:
     bool startedProgram; 	// if true, then the program was just started with the button
     bool forceMode;		// used to make the mode follow the program starting

public:
     Personality_0();
     virtual void ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, int mode, bool button);
     virtual void Loop(BT *bt, int mode, bool, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt, int mode);
};


#endif PERSONALITY_0_H
