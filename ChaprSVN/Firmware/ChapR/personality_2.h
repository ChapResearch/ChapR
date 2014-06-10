//
// personality_2.h
//
//   This file (based off of the associated personality_0.cpp) implements the Personality #3, which is
//
//		NXT-LabView
//
//   This personality is used to control NXT robots when using LabView libraries and programs on
//   the NXT.
//
//   The personality works exactly the same as NXT-RobotC (see personality_0.h), except that the WFS
//   button is a toggle (which keeps the user from having to hold it down to keep the robot from being
//   disabled).
//

#ifndef PERSONALITY_2_H
#define PERSONALITY_2_H

class Personality_2 : public Personality
{

private:
     bool buttonToggle;		// make the "wait-for-start" into a toggle

public:
     Personality_2();
     virtual void ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, bool button);
     virtual void Loop(BT *bt, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt);
     
};

#endif PERSONALITY_2_H
