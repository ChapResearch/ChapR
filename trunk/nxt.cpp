#include <arduino.h>

//
// nxt.cpp
//
//   NXT communication routines.
//

typedef unsigned char byte;

/////////////////////////////////////////////////////////////////////////////
// NXT Direct Commands - the following calls implement the NXT protocol
//			for "direct commands".  see the NXT Direct Commands
//			protocol reference.
/////////////////////////////////////////////////////////////////////////////

//
// nxtGamepadUSBTranslate() - translate a single USB gamepad reading into the fields that would
//				part of a RobotC joystick structure as serialized to be transmitted
//				by BT.  This routine is COMPLETELY dependent upon the way in which
//				the RobotC "Joystick.h" is coded, and will need to change if it does.
//				- the output array must be 7 bytes
//				- the input array must be 6 bytes
//				- NOTE that this routine doesn't take into account the MODE SWITCH
//				  which is byte #7 in the USB data.
//
void nxtGamepadUSBTranslate(byte *usbdata, byte *output)
{
     output[0] = usbdata[0];					// joystick 1 (left) X axis 
     output[1] = usbdata[1];					// joystick 1 (left) Y axis 
     output[2] = usbdata[2];					// joystick 2 (right) X axis 
     output[3] = usbdata[3];					// joystick 2 (right) Y axis 
     output[4] = (usbdata[4]&0xf0)>>4 | (usbdata[5]&0x0f<<4);	// buttons 1-8
     output[5] = (usbdata[5]&0xf0)>>4;				// buttons 9-12
     output[6] = usbdata[4] & 0x0f;				// tophat
}

//
// nxtJoystickTranslate() - given the joystick output from a USB joystick
//			    report, convert it the joystick structure as
//			    defined in RobotC - which, therefore, is used
//			    by all of the RobotC-style programs.
//			    Note that the USB joystick report is 8 bytes,
//			    and the RobotC joystick structure is 18 bytes
//			    with the last being a NULL.
//			    Note, too, that you need to specify the setting
//			    of "wait for start" and UserMode as arguments
//			    to this routine.
//			    If one/both of the joysticks aren't connected, this
//			    routine should be sent "blank" readings for that
//			    joystick.  
//
int nxtJoystickTranslate(byte *output, byte UserMode, byte StopPgm, byte *USBJoystick1, byte *USBJoystick2)
{
     output[0] = 0;			// this isn't actually used in the Joystick.c code
     output[1] = UserMode;		// normally set to zero
     output[2] = StopPgm;		// false to turn OFF wait for start

     nxtGamepadUSBTranslate(USBJoystick1,&output[3]);
     nxtGamepadUSBTranslate(USBJoystick2,&output[10]);

     output[17] = 0;

     return(18);
}

// nxtDCMessageWrite() - do the Direct Command for MessageWrite
//			as well as wrap the message for transport.
//			Fills in the output buffer and returns the
//			size of the resultant message.
//			NOTE that the message MUST have a '\0' at
//			the end to be legal, so this routine
//			enforces that.
//
int nxtDCMessageWrite(byte *output, byte *input, int size, int mbox)
{
     input[size-1] = '\0';		// enforce the NULL termination

     output[0] = size + 4;		// size does NOT include this byte itself
     output[1] = 0x80;			// direct command with no response
     output[2] = 0x09;			// the direct command for MessageWrite
     output[3] = mbox;			// should normally use zero here
     output[4] = size;
     memcpy(output+5,input,size);

     return(size+4);
}

//
// nxtMessage() - create a direct command to send a simple message.
//
//void nxtMessage