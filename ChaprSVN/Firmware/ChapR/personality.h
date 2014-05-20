//
// personality.h
//
// The ChapR can operate with different "personalities".  Each "personality" describes
// how the ChapR interacts with the target device over bluetooth.  The things affected
// by the "personality" include:
//
//	- format of message being sent out
//	- message send on change only option
//	- frequency of message send (should also guarantee frequent calling to allow personalities to "keep alive")
//	- behavior of kill-switch situation (happens upon click of power switch, or power-down)
//
// While most devices that the ChapR talks to will be NXT bricks with differing libraries,
// personalities are meant to allow it to speak with completely different devices too.
//
//

#ifndef PERSONALITY_H
#define PERSONALITY_H

#define AUTO 0
#define TELE 1
#define END  2
#define NONE 3

//
// The Personality class is meant to be an "interface" that all true personalities
// reference.  It is used like this:
//
//    class RealPersonality : public Personality {
//	...
//    };
//
//  Where the body of the real personality implements the personality calls below.

class Personality
{
  
 private: 

  bool inMatchMode;          // determines whether the ChapR cycles through programs like FCS
  int  matchMode;            // which mode the ChapR is in (auto, tele, endgame or none)
  long pwrTarget;            // keeps track of when the power button was pressed
  long autoTarget;            // when the auto should end (in millis)
  long teleTarget;            // when tele should end (in millis)
  long endTarget;             // when the endgame should end (in millis)
  long timePassed;           // used to keep track of time when a mode is paused

 public:

  Personality();

  // the Loop() interface of the Personality is called at the end of each loop through
  //  the Arduino code.  It is used in personalities where a message is sent during each
  //  loop of the code (like the NXT-RobotC personality).  Note that the outer code that
  //  calls ther personality interface will NOT change the data in g1/g2 from loop to
  //  loop.  NOTE that is up to the personality to decide what to do if the BT is connected
  //  or not.

  virtual void Loop(BT *bt, bool button, Gamepad *g1, Gamepad *g2) = 0;

  // the ChangeXXXX() interface of the Personality is called whenever there is a change to
  //   the state of the input devices to the ChapR.  While this can be used by all
  //   personalities, it is used primarily by personalities that only send data upon
  //   a change (like the NXT-G personality).  Note that Change() is called potentially
  //   three times per loop, because there are potentially two input devices to the ChapR
  //   along with the WFS/pairing button.  The "device" is set to either 1 or 2 depending
  //	  upon which input device had the change.   NOTE that is up to the personality to
  //   decide what to do if the BT is connected or not.

  virtual void ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *gnu) = 0;
  virtual void ChangeButton(BT *bt, bool button) = 0;

  // the Kill() interface of the Personality is called whenever the ChapR issues a KILL
  //   to the remote device (like the NXT).  The "kill switch" is meant to cause the remote
  //   device to stop what it is doing immediately.  Kills are issued when the power switch
  //   is clicked, or held down to power off the ChapR.  Note that Kills are NOT issued when
  //   the ChapR auto-off's.   NOTE that is up to the personality to decide what to do if the
  //	  BT is connected or not.

  virtual void Kill(BT *bt) = 0;
     
  bool updateMode(bool = true);
  void swapInMatchMode();
  void beginAuto();
  void beginTele();
  void endCycle();
  void pauseMatchCycle();
  void playMatchCycle();
  int  getMatchMode();
  bool isInMatchMode();
};

#endif PERSONALITY_H
