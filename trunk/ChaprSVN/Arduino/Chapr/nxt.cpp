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
#include "ChapREEPROM.h"

ChapREEPROM myEEPROM3;

typedef unsigned char byte;

char hexConverter[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


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
     static char	btAddressbuf[13];	// 6 bytes of BT address (ignores the last one)

     *name = namebuf;
     *btAddress = btAddressbuf;
     *freeMemory = 0;
     
     vdip->cmd(VDIP_SC,NULL,100,usbDev);	// set the current VDIP port to the NXT

     cbuf[0] = NXT_SYS_CMD;
     cbuf[1] = NXT_GET_DEV_INFO;

     vdip->cmd(VDIP_DSD,cbuf,100,2);		// send the command

     delay(1000);			       	// second delay for return of command

     int r = vdip->cmd(VDIP_DRD,cbuf,100);

     if(r != 33) {
	  Serial.println("yikes, didn't get values back");
	  return(0);
     } else {
	  //Serial.println("yeah!  got 33 back");
	  //dumpDataHex(cbuf,33);
          for (int i = 0; i < 15; i++){
            namebuf[i] = cbuf[3 + i];
          }
          
          int j = 0;
          for (int i = 18; i < 24; i++){
            //the last byte is always zero (sorta like a null terminator)
            btAddressbuf[j++] = hexConverter[(cbuf[i]>>4)&0x0F];  
            btAddressbuf[j++] = hexConverter[cbuf[i]&0x0F];
          }
          btAddressbuf[j] = '\0';
          
          long m = 1;
          for (int i = 29; i < 33; i++, m *= 256){
            *freeMemory += cbuf[i]*m;
          }
          
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
// nxtBTMailboxMsgCompose() - given a message, compose a BT message to be sent to
//				a particular mailbox on the NXT.  This COULD be split
//				into two commands - one to compose the message and
//				another to wrap it in bluetooth - but by combining
//				them we save copy time.  It is ASSUMED that the incoming
//				message buffer has enough space to add the 6 bytes
//				needed for headers.  NOTE too that the NULL termination
//				that is required for messages is enforced.  that is, the
//				last byte of the incoming message is always set to '\0'
//
int nxtBTMailboxMsgCompose(int mbox, byte *msgbuff, int size)
{
     // BT messages need two extra bytes on the front for BT packet size (+2)
     // and we also need to add two bytes for the standard NXT command (+2)
     // then two bytes for the mailbox direct command (+2)

     msgbuff[size-1] = '\0';		// enforce the NULL termination

     memmove(msgbuff+6,msgbuff,size);	// shift over by the 6 needed to package the message

     msgbuff[0] = size + 4;		// BT size does NOT include these two size bytes
     msgbuff[1] = 0x00;			// this is the BT MSB of size - always zero
     msgbuff[2] = NXT_DIR_CMD_NR;	// direct command with no response
     msgbuff[3] = NXT_DIR_SEND;		// the direct command for MessageWrite
     msgbuff[4] = mbox;			// the mailbox to use
     msgbuff[5] = size;			// and this is the size of actual mailbox message

     return(size+6);			// total size of the message going over BT
}


//
// nxtBTKillCommand() - create a BT message for killing the current program running
//			on the next.  As above, this COULD be split into two commands -
//			one that composes the kill message and the other wrap it in
//			BT - but it is in one to make it more efficient.
//

int nxtBTKillCommand(byte *msgbuff)
{
     msgbuff[0] = 2;			// BT size does NOT include these two size bytes
     msgbuff[1] = 0x00;			// this is the BT MSB of size - always zero
     msgbuff[2] = NXT_DIR_CMD_NR;	// direct command, no response
     msgbuff[3] = NXT_DIR_STOP;		// stop command

     return(4);
}

