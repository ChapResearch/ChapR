//
// personality_1.h
//
//   This file (along with the associated personality_1.cpp) implement the Personality #1, which is
//
//		NXT-G
//
//   This personality is used to control NXT robots when using NXT-G libraries and programs on
//   the NXT.
//
//   Unlike the RobotC personality, this one only sends messages when a change occurs.  And they
//   are sent in mailboxes as follows.  Changes are isolated to buttons and sticks separately, and
//   are sent to different "mailboxes" so that NXT-G can process the messages easily.  Note that
//   a message is sent for each of a button press and button release events.
//
//				Mailbox	Data
//				-------	----------------------------
//				0	wait-for-start (logic)
//				1	X1 mapped between -100 and 100 (number)
//				2	Y1 mapped between -100 and 100 (number)
//				3	X2 mapped between -100 and 100 (number)
//				4	Y2 mapped between -100 and 100 (number)
//				5	Button 1 (logic)
//				6	Button 2 (logic)
//				7	Button 3 (logic)
//				8	Button 4 (logic)
//				9	Button ???? (need to pick one)
//

#ifndef PERSONALITY_1_H
#define PERSONALITY_1_H

class Personality_1 : public Personality
{
private:
     void mySendMessageInt(BT *,int,int);
     void mySendMessageBool(BT *,int,bool);
public:
     virtual void ChangeInput(BT *bt, int device, Gamepad *old, Gamepad *);
     virtual void ChangeButton(BT *bt, bool button);
     virtual void Loop(BT *bt, Gamepad *, Gamepad *);
     virtual void Kill(BT *bt);
};


#endif PERSONALITY_1_H
