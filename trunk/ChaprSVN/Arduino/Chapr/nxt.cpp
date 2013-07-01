#include <arduino.h>

//
// nxt.cpp
//
//   NXT communication routines.  This file implements all NXT communication routines.
//   It supports both USB and BT communication.
//
//   Note there are "normal" commands as well as a set of extended "direct commands."
//   These routines don't worry the user with the difference between them.
//

#include "vdip.h"
#include "nxt.h"

typedef unsigned char byte;

void dumpDataHex(char *buffer, int count)
{	
     int i = 0, value;

     for(;count > 0; count--, i++, buffer++) {
	  value = (unsigned char)*buffer;
	  Serial.print("0x");
	  if( value< 16) {
	       Serial.print("0");
	  }
	  Serial.print(value,HEX);
	  Serial.print(" ");
	  if( i%8 == 7) {
	       Serial.println("");
	  }
     }
}

//
// nxtQueryDevice() - USB - queries the NXT device for it's settings by issuing a GET DEVICE INFO
//		      command.  This should only be done via USB (for now).  This routine returns
//		      TRUE (non-zero) if the command worked or FALSE (zero) if it didn't.  If it
//		      did work, the name buffer is filled in with the name of the NXT, the btaddress
//		      buffer is filled in, and the freeMemory long is filled in with the amount
//		      of free memory on the NXT.
//
//		      Since only one NXT is supposed to be connected at any one time, this routine
//		      uses simple static buffers to return the name and btAddress.  The next call
//		      will wipe them out.
//
bool nxtQueryDevice(VDIP *vdip, int usbDev, char **name, char **btAddress, long *freeMemory)
{
     // assumes we are connected, otherwise this routine shouldn't be called

     char		cbuf[50];		// enough for return data from NXT (plus slop)
     static char 	namebuf[15];		// 14 chars plus \0 termination
     static char	btAddressbuf[7];	// 7 bytes of BT address

     *name = namebuf;
     *btAddress = btAddressbuf;

     vdip->cmd(VDIP_SC,NULL,100,usbDev);	// set the current VDIP port to the NXT

     cbuf[0] = NXT_SYS_CMD;
     cbuf[1] = NXT_GET_DEV_INFO;

     vdip->cmd(VDIP_DSD,cbuf,100,2);		// send the command

     delay(1000);			       	// half-second delay for return of command

     int r = vdip->cmd(VDIP_DRD,cbuf,100);

     Serial.println(r,HEX);

     if(r != 33) {
	  Serial.println("yikes, didn't get values back");
	  return(0);
     } else {
	  Serial.println("yeah!  got 33 back");
	  dumpDataHex(cbuf,33);
	  return(1);
     }
}



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
//     output[0] = usbdata[0];					// joystick 1 (left) X axis 
//     output[1] = usbdata[1];					// joystick 1 (left) Y axis 
//     output[2] = usbdata[2];					// joystick 2 (right) X axis 
//     output[3] = usbdata[3];					// joystick 2 (right) Y axis 

     output[0] = usbdata[0]-128;				// joystick 1 (left) X axis 
     output[1] = usbdata[1]-128;				// joystick 1 (left) Y axis 
     output[2] = usbdata[2]-128;				// joystick 2 (right) X axis 
     output[3] = usbdata[3]-128;				// joystick 2 (right) Y axis 

     output[4] = (usbdata[4]&0xf0)>>4 | (usbdata[5]&0x0f)<<4;	// buttons 1-8
     output[5] = (usbdata[5]&0xf0)>>4;				// buttons 9-12
     output[6] = usbdata[4] & 0x0f;				// tophat
     if (output[6] & 0x08) {					//   NXT expects -1 if no tophat pressed
	  output[6] = 0xff;
     }

     // we don't send the last byte that is mode and controller data
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

     nxtGamepadUSBTranslate(USBJoystick1,&output[3]);	// used to have +1 when the \r wasn't being read
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
int nxtDCMessagePackage(byte *output, byte *input, int size, int mbox)
{
     // note that in the code below only the LSB of size is sent - that's
     // because we will never send a message that needs the MSB of size

     input[size-1] = '\0';		// enforce the NULL termination

     output[0] = size + 4;		// size does NOT include this byte itself
     output[1] = 0x00;			// this is the MSB of size - always zero
     output[2] = 0x80;			// direct command with no response
     output[3] = 0x09;			// the direct command for MessageWrite
     output[4] = mbox;			// should normally use zero here
     output[5] = size;
     memcpy(output+6,input,size);

     return(size+6);			// includes the size byte
}

//
// nxtMsgCompose() - given two joystick structures and other data, compose a message to
//			be sent to the NXT.
//
int nxtMsgCompose(byte *output, 	// the output buffer to scribble things to - min 24 bytes
		  byte UserMode,	// the usermode value
		  byte StopPgm,		// the wait-for-start value
		  byte *USBJoystick1,	// buffers for the two joysticks
		  byte *USBJoystick2)
{
     byte	buffer[20];
     int	size;
     int	mbox = 0;

     size = nxtJoystickTranslate(buffer, UserMode, StopPgm, USBJoystick1, USBJoystick2);
     return(nxtDCMessagePackage(output,buffer,size,mbox));
}
