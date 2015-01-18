#pragma config(Sensor, S1,     ,               sensorTouch)
#pragma config(Sensor, S2,     ,               sensorTouch)
#pragma config(Sensor, S3,     ,               sensorTouch)
#pragma config(Sensor, S4,     ,               sensorTouch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

////////////////////////////////////////////////////////////////////////////////
//
// RRRRRRR   EEEEEEE  MM    MM   OOOOOO  TTTTTTTT  EEEEEEE
// RR    RR  EE       MMM  MMM  OO    OO    TT     EE
// RRRRRRR   EEEEE    MM MM MM  00    00    TT     EEEEE
// RR   RR   EE       MM    MM  OO    OO    TT     EE
// RR    RR  EEEEEEE  MM    MM   000000     TT     EEEEEEE
//
// REMOTE - this file, along with remote.h, implement the PC-less remote control.
//
////////////////////////////////////////////////////////////////////////////////

//
// remoteInit() - called to initialize the remote.  At this point
//                all it does is configure the sensors appropriately.
//                This routine is intimately tied to the way in which
//                the switches are wired.  Current configuration is:
//
//	  LEFT 	  	    		       	 RIGHT
//	 ,-----------,  ,---------,   ,---------------,
//     1-'-, 2---,   '--|3  MUX  2|---'       1---, 2-'-,
//     | + | | + |------|4  out  1|-----------| + | | + |
//     '---' '---'      '-----,---'           '---' '---'
//       ,------------,       |	       	   ,----------,
//     3-'-, 4---,    '-,   ,---------,  ,-'  3---, 4-'-,
//     | + | | + |---,  '---|1  MUX  4|--' ,--| + | | + |
//     '---' '---'   '------|2  out  3|----'  '---' '---'
//       ,---------- NC     '-----,---'   NC----------,
//     5-'-, 6---,            |   |           5---, 6-'-,
//     | + | | + |--------,   |   |   ,-------| + | | + |
//     '---' '---'        |   |   |   |       '---' '---'
//	 ,------------ NC |   |   |   |  NC ----------,
//     7-'-, 8---,        |   |   |   |       7---, 8-'-,
//     | + | | + |---- NC |   |   |   |  NC --| + | | + |
//     '---' '---'        S4  S1  S2  S3      '---' '---'
//
//
// Current implementation "mask" map is as follows.  It masks out bit 7 and 8
// from both sides and bit 5 from the left side and bit 6 from the right.

#define REMOTEMASK	0x2f1f

void remoteInit()
{
  SensorType[S1] = sensorHiTechnicTouchMux;
  SensorType[S2] = sensorHiTechnicTouchMux;

  eraseDisplay();
}

//
// remoteSample() - samples the remote and returns an unsigned short
//                  (two bytes) organized as described above.  Note
//                  that this routine is intimately tied to the way
//                  in which the remote is wired - see remoteInit()
//                  for the coded configuration of the hardware.
//
short remoteSample()
{
  short   left = 0;
  short   right = 0;

  right = (SensorValue[S1] & 0x0003) | (SensorValue[S2] & 0x000c) | (SensorValue[S3]?0x0010:0);
  left = ((SensorValue[S1] & 0x000c)>>2) | ((SensorValue[S2] & 0x0003)<<2) | (SensorValue[S4]?0x0020:0);

  return((left<<8) | right);
}

//
// remoteDisplay() - given a byte (actually the bottom eight bits of an
//                   unsigned short) display the state of the buttons
//                   beginning at the given x/y and moving down and to
//                   the right.  The 1's in the given mask tell which
//                   buttons to draw (ie - 0xff draws all eight buttons)
//
void remoteDisplay(short sample, int x, int y, short mask)
{
#define SPACING 14
  int    i;
  int    x_save;

  x_save = x;
  for( i = 0; i < 8; i++) {
    if(mask&0x01) {
      if(sample&0x01) {
	nxtDisplayRICFile(x,y,"buttonOn.ric");
      } else {
	nxtDisplayRICFile(x,y,"buttonOff.ric");
      }
    }
    sample = sample>>1;
    mask = mask>>1;
    x += SPACING;
    if (i % 2) {
      y -= SPACING;
      x = x_save;
    }
  }
}

//
// void remoteNXTDisplay() - displays the three NXT buttons.  The three are the
//				left and right and orange buttons.  Only one
//				button can be pressed at once - so it is pretty
//				simple.
//				-1 = nothing
//				0 = Grey button (exit) - not possible in this implementation
//				1 = Right
//				2 = Left
//				3 = Orange button
//
void remoteNXTDisplay(short sample)
{
	short buttonX[] = { 45, 59, 31, 45};
	short buttonY[] = { 0, 14, 14, 14};
	int	i;

	for( i=0; i <= 3; i++) {
		if (sample == i) {
			nxtDisplayRICFile(buttonX[i],buttonY[i],"buttonOn.ric");
		} else {
			nxtDisplayRICFile(buttonX[i],buttonY[i],"buttonOff.ric");
		}
	}
}

void remoteSplit(short sample, short &left, short &right)
{
	left = (sample >> 8) & 0x00ff;
	right = sample & 0x00ff;
}


//
// remoteCommand() - given a short value, as defined in remote.h for the
//		     joystick values, program it into the joystick structure.
//
void remoteCommand(short cmd, TJoystick &joystick) 
{
	switch(cmd) {
	case	NOTHING:	break;
	case	RELEASE:	joystick.StopPgm = false; break;
	case 	C1_Button1:	joystick.joy1_Buttons |= 0x0001; break;
	case 	C1_Button2:	joystick.joy1_Buttons |= 0x0002; break;
	case 	C1_Button3:	joystick.joy1_Buttons |= 0x0004; break;
	case 	C1_Button4:	joystick.joy1_Buttons |= 0x0008; break;
	case 	C1_Button5:	joystick.joy1_Buttons |= 0x0010; break;
	case 	C1_Button6:	joystick.joy1_Buttons |= 0x0020; break;
	case 	C1_Button7:	joystick.joy1_Buttons |= 0x0040; break;
	case 	C1_Button8:	joystick.joy1_Buttons |= 0x0080; break;
	case 	C1_Button9:	joystick.joy1_Buttons |= 0x0100; break;
	case 	C1_Button10:	joystick.joy1_Buttons |= 0x0200; break;
	case 	C1_Button11:	joystick.joy1_Buttons |= 0x0400; break;
	case 	C1_Button12:	joystick.joy1_Buttons |= 0x0800; break;
	case 	C1_X1_East:	joystick.joy1_x1 = 127; break;
	case 	C1_X1_West:	joystick.joy1_x1 = -128; break;
	case 	C1_Y1_North:	joystick.joy1_y1 = 127; break;
	case 	C1_Y1_South:	joystick.joy1_y1 = -128; break;
	case 	C1_X2_East:	joystick.joy1_x2 = 127; break;
	case 	C1_X2_West:	joystick.joy1_x2 = -128; break;
	case 	C1_Y2_North:	joystick.joy1_y2 = 127; break;
	case 	C1_Y2_South:	joystick.joy1_y2 = -128; break;
	case	C1_TH_0:	joystick.joy1_TopHat = 0; break;
	case	C1_TH_1:	joystick.joy1_TopHat = 1; break;
	case	C1_TH_2:	joystick.joy1_TopHat = 2; break;
	case	C1_TH_3:	joystick.joy1_TopHat = 3; break;
	case	C1_TH_4:	joystick.joy1_TopHat = 4; break;
	case	C1_TH_5:	joystick.joy1_TopHat = 5; break;
	case	C1_TH_6:	joystick.joy1_TopHat = 6; break;
	case	C1_TH_7:	joystick.joy1_TopHat = 7; break;
	case 	C2_Button1:	joystick.joy2_Buttons |= 0x0001; break;
	case 	C2_Button2:	joystick.joy2_Buttons |= 0x0002; break;
	case 	C2_Button3:	joystick.joy2_Buttons |= 0x0004; break;
	case 	C2_Button4:	joystick.joy2_Buttons |= 0x0008; break;
	case 	C2_Button5:	joystick.joy2_Buttons |= 0x0010; break;
	case 	C2_Button6:	joystick.joy2_Buttons |= 0x0020; break;
	case 	C2_Button7:	joystick.joy2_Buttons |= 0x0040; break;
	case 	C2_Button8:	joystick.joy2_Buttons |= 0x0080; break;
	case 	C2_Button9:	joystick.joy2_Buttons |= 0x0100; break;
	case 	C2_Button10:	joystick.joy2_Buttons |= 0x0200; break;
	case 	C2_Button11:	joystick.joy2_Buttons |= 0x0400; break;
	case 	C2_Button12:	joystick.joy2_Buttons |= 0x0800; break;
	case 	C2_X1_East:	joystick.joy2_x1 = 127; break;
	case 	C2_X1_West:	joystick.joy2_x1 = -128; break;
	case 	C2_Y1_North:	joystick.joy2_y1 = 127; break;
	case 	C2_Y1_South:	joystick.joy2_y1 = -128; break;
	case 	C2_X2_East:	joystick.joy2_x2 = 127; break;
	case 	C2_X2_West:	joystick.joy2_x2 = -128; break;
	case 	C2_Y2_North:	joystick.joy2_y2 = 127; break;
	case 	C2_Y2_South:	joystick.joy2_y2 = -128; break;
	case	C2_TH_0:	joystick.joy2_TopHat = 0; break;
	case	C2_TH_1:	joystick.joy2_TopHat = 1; break;
	case	C2_TH_2:	joystick.joy2_TopHat = 2; break;
	case	C2_TH_3:	joystick.joy2_TopHat = 3; break;
	case	C2_TH_4:	joystick.joy2_TopHat = 4; break;
	case	C2_TH_5:	joystick.joy2_TopHat = 5; break;
	case	C2_TH_6:	joystick.joy2_TopHat = 6; break;
	case	C2_TH_7:	joystick.joy2_TopHat = 7; break;

	}
}

//
// remoteTranslate() - this routine takes the current sample and translates it
//			to the correct joystick values given the translation
//			that the user programmed into RemoteTranslation[].
//
void remoteTranslate(short sample, short mask, short bsample, TJoystick &joystick)
{
	int	b;

	joystick.UserMode  = false;
	joystick.StopPgm   = true;

	joystick.joy1_x1 = 0;
	joystick.joy1_y1 = 0;
	joystick.joy1_x2 = 0;
	joystick.joy1_y2 = 0;
	joystick.joy1_Buttons = 0;
	joystick.joy1_TopHat = -1;

	joystick.joy2_x1 = 0;
	joystick.joy2_y1 = 0;
	joystick.joy2_x2 = 0;
	joystick.joy2_y2 = 0;
	joystick.joy2_Buttons = 0;
	joystick.joy2_TopHat = -1;

	// process the NXT Orange Button

	if (bsample == 3) {
		remoteCommand(ButtonTranslation[0],joystick);
	}

	// process the side buttons

	for( b=1; b < 17; b++) {
		if ((mask & 0x0001) && (sample&0x0001)) {	// this bit position is processed & set
			remoteCommand(ButtonTranslation[b],joystick);
		}
		mask = mask >> 1;	// prepare for the next bit
		sample = sample >> 1;	// prepare for the next bit
	}

	// process the two NXT arrow buttons

	if (bsample == 2) {	// left
		remoteCommand(ButtonTranslation[17],joystick);
	}
	if (bsample == 1) {	// right
		remoteCommand(ButtonTranslation[18],joystick);
	}

}

void remoteTransmit(TJoystick &joystick)
{
	TFileIOResult	messageOut;
	const int	MaxSizeOfMessage = 18;
	ubyte		buffer[MaxSizeOfMessage];

	buffer[0] = 0;			// I don't know WHAT is supposed to be here
	buffer[1] = (ubyte) joystick.UserMode;
	buffer[2] = (ubyte) joystick.StopPgm;

	buffer[3] = joystick.joy1_x1;
	buffer[4] = joystick.joy1_y1;
	buffer[5] = joystick.joy1_x2;
	buffer[6] = joystick.joy1_y2;
	buffer[7] = joystick.joy1_Buttons & 0x00ff;
	buffer[8] = (joystick.joy1_Buttons>>8) & 0x00ff;
	buffer[9] = joystick.joy1_TopHat;

	buffer[10] = joystick.joy2_x1;
	buffer[11] = joystick.joy2_y1;
	buffer[12] = joystick.joy2_x2;
	buffer[13] = joystick.joy2_y2;
	buffer[14] = joystick.joy2_Buttons & 0x00ff;
	buffer[15] = (joystick.joy2_Buttons>>8) & 0x00ff;
	buffer[16] = joystick.joy2_TopHat;

	messageOut = cCmdMessageWriteToBluetooth(buffer, sizeof(buffer), mailbox1);
	nxtDisplayStringAt(30,63,"%d  ",messageOut);
}

task main()
{
	short		left, right;
	short		sample, bsample;
	short		last_sample = 0;
	short		last_bsample = -1;
	TJoystick	joyout;

	// these two tasks are started by the JoystickDrive.c include
	// we need to include the file (in remote.h) to get the
	// joystick structure

	StopTask(readMsgFromPC);
	StopTask(displayDiagnostics);

	remoteInit();

	while (true) {

		sample = remoteSample();
		bsample = nNxtButtonPressed;

		remoteSplit(sample,left,right);

		remoteDisplay(right,75,53,REMOTEMASK & 0xff);
		remoteDisplay(left,0,53,(REMOTEMASK & 0xff00) >> 8);
		remoteNXTDisplay(bsample);

		if(sample != last_sample || bsample != last_bsample) {
			remoteTranslate(sample,REMOTEMASK,bsample,joyout);
			remoteTransmit(joyout);
			// sendmsg((ubyte)left, (ubyte)right);
			last_sample = sample;
			last_bsample = bsample;
		}
		wait1Msec(100);
	}
}