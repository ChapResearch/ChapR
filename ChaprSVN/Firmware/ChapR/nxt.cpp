#include <Arduino.h>

//
// nxt.cpp
//
//   NXT communication routines.  This file implements all NXT communication routines.
//   It supports both USB and BT communication.
//
//   Note there are "normal" commands as well as a set of extended "direct commands."
//   These routines don't worry the user with the difference between them.
//

#include "BT.h"
#include "VDIP.h"
#include "nxt.h"
#include "settings.h"
#include "debug.h"

extern settings myEEPROM;

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
	  return(0);
     } else {
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
// nxtGetProgramName() - fills the char* buffer it is given
//                       with the name of the program running, if
//                       Bluetooth is connected. It returns false if
//                       no program is running or something goes wrong with
//                       the communication. The buffer won't be modified unless
//                       true is returned. If true is returned, buffer will contain
//                       the null-terminated name of the proram running on the NXT (including 
//                       ".rxe"). The buffer needs to be at least 20 bytes and will always be null-terminated.
//
bool nxtGetProgramName(BT *bt, char *buf)
{
     byte	outbuff[64];
     int	size = 0;
     
     if (!bt->connected()){
       return false;
     }
     
     delay(10);

     outbuff[size++] = 2;			// BT size does NOT include these two size bytes
     outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
     outbuff[size++] = NXT_DIR_CMD;
     outbuff[size++] = NXT_DIR_CURRENT;
     
     bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
     (void)bt->btWrite(outbuff,size);
     bt->recv(outbuff,2,1000); 
     size = outbuff[0];
       
     // check to make sure the proper message is received
       
     if (size != 23){ 
       bt->flushReturnData();
       return false;
     }
       
     // receive the rest of the data
       
     bt->recv(outbuff,size,1000); // timeout of 1000 milliseconds
       
     // check to see if a program is running
       
     if (outbuff[NXT_STATUS] == (byte) NXT_ERR_NOACT){ // the error constant is cast to a byte to prevent sign extension
       return false;
     }
       
     if (outbuff[NXT_PRGM_NAME] == 0){ // checks to see that the name returned is useful
       return false;
     }
       
     // copy the name to the return buffer
       
     strncpy(buf,(char *) outbuff + NXT_PRGM_NAME,NXT_PRGM_NAME_SIZE);
      
     return true;
}

//
// nxtOpenFileToRead() - returns the handle if the file is found properly (-1 otherwise).
//                       If the handle is not -1, the int * given will be filled with the 
//                       size.
int nxtOpenFileToRead(BT *bt, char *buf, long *fileSize)
{
     byte	outbuff[64];
     int	size = 0;
     
     if (!bt->connected()){
       return -1;
     }
       
     delay(10);

     // open the file on the nxt
       
     outbuff[size++] = 22;			// BT size does NOT include these two size bytes
     outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
     outbuff[size++] = NXT_SYS_CMD;
     outbuff[size++] = NXT_SYS_OPEN_R;
     strncpy((char *)outbuff + size, buf, NXT_PRGM_NAME_SIZE);
     size += NXT_PRGM_NAME_SIZE;
       
     bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
     (void)bt->btWrite(outbuff,size);
     bt->recv(outbuff,2,1000); 
     size = outbuff[0];
       
     // check to make sure the proper message is received
       
     if (size != 8){ 
       bt->flushReturnData();
       return -1;
     }
       
     //receive the rest of the data
       
     bt->recv(outbuff,size,1000);

     if(outbuff[2] != 0) {
	  // couldn't open file
	  return(-1);
     }

//     dumpDataHex("openFileToRead return packet", outbuff, 8);
//     delay(10);

     // determines the size of the file
     
     *fileSize = outbuff[4];
     *fileSize |= outbuff[5] << 8;
     *fileSize |= outbuff[6] << 16;
     *fileSize |= outbuff[7] << 24;
     
     // returns the handle of the file

     return outbuff[3];
}

//
// nxtReadFile() - if everything goes well, the number of bytes read will be returned. If the size of
//                 the Bluetooth message isn't what it is supposed to be or returns an error, the method
//                 will return a -1. If the method does not return a -1, the buffer given will be filled 
//                 with the contents of the file specified by the handle you passed to the method. The
//                 amount of data to read (numToRead) should never be over 255 - if so, this routine
//                 won't work as expected.
//
int nxtReadFile(BT *bt, char *buf, int numToRead, int handle)
{
     byte	outbuff[64];
     int	size = 0;
     
     if (!bt->connected()){
       return -1;
     }
     
     outbuff[size++] = 5;			// BT size does NOT include these two size bytes
     outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
     outbuff[size++] = NXT_SYS_CMD;
     outbuff[size++] = NXT_SYS_READ;
     outbuff[size++] = handle;
     outbuff[size++] = numToRead;
     outbuff[size++] = 0; // normally the most significant byte, but the size will never be over 255
            
     bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
     (void)bt->btWrite(outbuff,size);
     bt->recv(outbuff,2,1000);   //  we wait for the NXT for up to 1 second for a response
     size = outbuff[0];
       
     // since we asked for numToRead bytes, the return BT packet should be numToRead + 6
     // or something has gone wrong.
       
     if (size != numToRead+6) { 
       bt->flushReturnData();
       return -1;
     }
       
     //receive the rest of the data
       
     bt->recv(outbuff,size,1000);
       
     if (outbuff[2] != 0){ //checks that the read was successful
        return -1;
     }
       
     //copies the data into buf
       
     strncpy(buf, (char *) outbuff + 6, numToRead);
     
     return numToRead; // amount of data read
} 

//
// nxtCloseFile() - closes the file named by the given handle, returning true if everything works,
//                  false otherwise. If something goes wrong, we can't do anything but return false.
//
bool nxtCloseFile(BT *bt, int handle)
{
     byte	outbuff[64];
     int	size = 0;
     
     if (!bt->connected()){
       return false;
     }
  
     delay(10);

     // open the file on the nxt
       
     outbuff[size++] = 3;			// BT size does NOT include these two size bytes
     outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
     outbuff[size++] = NXT_SYS_CMD;
     outbuff[size++] = NXT_SYS_CLOSE;
     outbuff[size++] = handle;
       
     bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
     (void)bt->btWrite(outbuff,size);
     bt->recv(outbuff,2,1000); 
     size = outbuff[0];
       
     // check to make sure the proper message is received
       
     if (size != 4){
       bt->flushReturnData();
       return false;
     }
       
     // receive the rest of the data
       
     bt->recv(outbuff,size,1000);

//     dumpDataHex("return after trying to close", outbuff, size);
//     delay(10);

     // checks that the handle of the file closed matches the given handle

     if (outbuff[3] == handle){
       return true;
     } else {
       return false;
     }
}

//
// nxtGetChosenProgram() - fills the buffer with the name of the program loaded into program
//                         chooser and returns true if successful. If no file is found, false 
//                         is returned. Also, if an error is encountered along the way, the 
//                         file will simply be closed.
//
bool nxtGetChosenProgram(BT *bt, char *buf)
{
     long       fileSize;
     bool       retVal = false;
     int	handle;
     
     if (!bt->connected()){
       return false;
     }

     delay(10);

     handle = nxtOpenFileToRead(bt, "FTCConfig.txt", &fileSize);
     if(handle == -1) {
	  return(false);
     }
      
     if (fileSize < NXT_PRGM_NAME_SIZE){ //the name will not include the null terminator
     
         if (nxtReadFile(bt, buf, fileSize, handle) != -1){ //simply gets the bytes
           buf[fileSize] = '\0'; //null terminates the name of the file
           retVal = true;
         }
         
     }
 
     (void) nxtCloseFile(bt, handle);
     return retVal;
}

//
// nxtRunProgram() - takes in the name of the program to run, returning whether
//                   or not said program ran. A program would not run (and the 
//                   method would return false) because either the program was
//                   not found or the NXT brick is not connected via Bluetooth.
//
bool nxtRunProgram(BT *bt, char *buf)
{
     byte	outbuff[64];
     int	size = 0;
     
     if (bt->connected()) {

       delay(10);

       // open the file on the nxt
       
       outbuff[size++] = 22;			// BT size does NOT include these two size bytes
       outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
       outbuff[size++] = NXT_DIR_CMD;
       outbuff[size++] = NXT_DIR_START;
       strncpy((char *)outbuff + size, buf, NXT_PRGM_NAME_SIZE);
       size += NXT_PRGM_NAME_SIZE;
       
       bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
       (void)bt->btWrite(outbuff,size);
       bt->recv(outbuff,2,1000); 
       size = outbuff[0];
       
       if (size != 3) {
         bt->flushReturnData();
         return false;
       }
       
       bt->recv(outbuff,size,1000); 
       
       return outbuff[2] == NXT_ERR_NONE;
     }
     
     return false;
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

bool nxtBTKillCommand(BT *bt)
{
    byte	outbuff[64];
    int	size = 0;
     
    if (bt->connected()) {

       delay(10);

       outbuff[size++] = 2;			// BT size does NOT include these two size bytes
       outbuff[size++] = 0x00;			// this is the BT MSB of size - always zero
       outbuff[size++] = NXT_DIR_CMD;	// direct command, no response
       outbuff[size++] = NXT_DIR_STOP;		// stop command
       
       bt->flushReturnData(); // somehow there is data in the Bluetooth receive buffer TODO figure out why
     
       (void)bt->btWrite(outbuff,size);
       bt->recv(outbuff,2,1000); 
       size = outbuff[0];
       
       if (size != 3) {
         bt->flushReturnData();
         return false;
       }
       
       bt->recv(outbuff,size,1000); 
       
       return outbuff[2] == NXT_ERR_NONE;
     }
    
     return false;
}

