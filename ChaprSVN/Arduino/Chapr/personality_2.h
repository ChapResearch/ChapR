//
// personality_2.h
//
//   This file (along with the associated personality_0.cpp) implement the Personality #0, which is
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
     bool startedProgram; // if true, then the program was just started with the button
public:
     Personality_2();
     virtual void ChangeInput(BT *bt, int mode, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, int mode, bool button);
     virtual void Loop(BT *bt, int mode, bool, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt, int mode);
private:
     bool buttonToggle;
};

#endif PERSONALITY_2_H
