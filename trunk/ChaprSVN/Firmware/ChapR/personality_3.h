//
// personality_3.h
//
//   This file implements the Personality #4, which is
//
//		cRIO-LabView
//
//   This personality is used to control cRIO robots when using LabView libraries and programs on
//   the cRIO.
//
//   This particular personality sends messages to the cRIO for each loop.  In fact, it doesn't
//   really care about changes...except KILL of course.  All gamepad/button data is sent in
//   each message.  (May want to look into only sending upon change, but also with a keep alive
//   that is sent periodically)
//

#ifndef PERSONALITY_3_H
#define PERSONALITY_3_H

class Personality_3 : public Personality
{

private:
     bool startedProgram; 	// if true, then the program was just started with the button

public:
     Personality_3();
     virtual void ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, bool button);
     virtual void Loop(BT *bt, bool, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt);
};


#endif PERSONALITY_3_H
