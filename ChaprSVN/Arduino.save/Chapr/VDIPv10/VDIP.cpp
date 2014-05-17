
#include <Arduino.h>

#include "VDIPSPI.h"
#include "VDIP.h"

//////////////////////
//  TODO (as of ** Wed Mar 20 15:03:04 2013 **)
//	- still seems to be some case(s) where plugging things in/out fails
//	- could probably make the code better, but also a timeout on the blocking calls,
//	  with a resync and retry may make sense
//	- particularly in the device update code
//	- also, when a disk is put in/out I believe the "No Update" command is messing things
//	  up - it appears to be coming back looking like part of the device configuration
//	  (only happens dynamically)
/////////////////////

//
// constructor
//
VDIP::VDIP(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin, uint8_t resetPin) :
    VDIPSPI(clockPin,mosiPin,misoPin,csPin),
    _count(0),
    _p1(false),
    _p2(false),
    _p1_dev(-1),
    _p2_dev(-1),
    _p1_devtype(CLASS_NONE),
    _p2_devtype(CLASS_NONE),
    _ready(false),
    _diskprocessed(false),
    _resetPin(resetPin)
{
  digitalWrite(_resetPin,HIGH);   // low is reset, done before shifting to output mode
  pinMode(_resetPin,OUTPUT);     
}

//
// The reset delay should be 3 or greater so that init messages can be consumed.
// Beyond that, it gives the user time to discover and pair to the device (in other code)
//
#define RESET_DELAY_SECS	5

//
// reset() - reset the VDIP.  The reset algorithm is as follows:
//		- toggle the reset line for the VDIP
//  		- continue flushing the VDIP, processing incoming messages
//  		- process the messages like normal, particularly noting if devices
//  		  are connected
//
void VDIP::reset(int _resetLED)
{
     unsigned long	target;
     monmsg		match;

     _init = false;
     digitalWrite(_resetPin,LOW);   // low is reset
     digitalWrite(_resetPin,HIGH);   // low is reset

     target = millis() + (RESET_DELAY_SECS * 1000);

     while(target > millis()) {
	  digitalWrite(_resetLED,(millis() & 0x100)?LOW:HIGH);
	  flush();
     }
     digitalWrite(_resetLED,LOW);

//     Serial.println("out of reset");
//     Serial.println(_init?"initialized":"NOT initialized");

}

//
// readLine() - checks for a line from the VDIP monitor.  If one exists, TRUE
//             is returned.  If not, FALSE is returned.  Note
//             that the ending \r is NOT part of the buffer, a \0 is there
//             instead.  If the line gets too long and fills up the buffer, it
//             is returned.
//             THIS ROUTINE CAN BE BLOCKING OR NON-BLOCKING which is cool.
//	       Non-blocking is necessary because we don't necessarily have
//	       incoming lines, but we need to check often.
//		BIG NOTE - a 1ms delay was added between reads - it appears
//		that this code was pounding on the read too hard apparently
//		causing the VDIP distress so it wasn't consistent with its
//		data.  The delay "could" probably be lower.
//
bool VDIP::readLine(bool blocking, long timeout /* = 0 */)
{
  unsigned char  incoming;
  long		 target = millis() + timeout;

  do {
       if (recv(&incoming,false)) {
//	    Serial.print(incoming,HEX);
	    if(incoming == '\r' || _count == BUFFER_SIZE) { // the buffer is one bigger than BUFFER_SIZE
		 _buffer[_count] = '\0';
		 _count = 0;
//		 Serial.println("got a read line");
		 return(true);
	    } else {
		 _buffer[_count++] = incoming;
	    }
       }
       delayMicroseconds(500);		// slow this down a bit, seems to help
  } while (blocking && (!timeout || (millis() < target)));

  return(false);			// either non-blocking or timed-out waiting for response line
}

//
// these are the async message that the monitor generates
// (messages not in response to anything)
monmessage async_messages[] = {
  { "DD1",                MONDD1 },
  { "DD2",                MONDD2 },
  { "DR1",                MONDR1 },
  { "DR2",                MONDR2 },
  { ">",                  MONPROMPT },
  { "NU",                 MONNU },
  { "ND",                 MONND },
  { "Device Detected P1", MONDD1 },
  { "Device Detected P2", MONDD2 },
  { "Device Removed P1",  MONDR1 },
  { "Device Removed P2",  MONDR2 },
  { "D:\\>",              MONPROMPT },   // note that the backslash was escaped
  { "*On-Line:",	  MONINIT },	// wild-card match with leading *
  { "No Disk",            MONND },
  { "No Upgrade",         MONNU },
  { "",			  MONBLANK },	// just a single return - blank line
  { NULL,                 MONUNKNOWN }
};

monmessage response_messages[] = {
  { ">",                  MONPROMPT },
  { "BC",                 MONBC },
  { "CF",                 MONCF },
  { "Command Failed",     MONCF },
  { "Bad Command",        MONBC },
  { "D:\\>",              MONPROMPT },   // note that the backslash was escaped
  { "",			  MONBLANK },	// just a single return - blank line
  { NULL,                 MONUNKNOWN }
};

//
// matchLine() - given a line from the VDIP monitor, match it an return the
//		 appropriate MON* token indicating what came back.  The line
//               isn't disturbed.
//
monmsg VDIP::matchLine(char *line, monmessage *targets)
{
     for (int i=0; targets[i].message != NULL; i++) {
	  if (*targets[i].message == '*') {			// first try the wild-card matching
	       if (strstr(line,targets[i].message+1) != NULL) {
		    return(targets[i].cmd);
	       }
	  } else if (strcmp(line,targets[i].message) == 0) {	// then normal matching
	       return(targets[i].cmd);
	  }
     }
     return(MONNOP);
}

//
// readLineMatch() - combines the reading of a line and matching for commands.  Returns
//		     the line matched, or MONNOP if no line was matched and the call
//		     isn't blocking.
//
monmsg VDIP::readLineMatch(bool blocking, monmessage *targets, long timeout /* = 0 */)
{
     if(readLine(blocking,timeout)) {
	  return(matchLine(_buffer,targets));
     }
     return(MONNOP);
}

#define HAVE_JOY1	(_p1 && _p1_dev != -1 && !(_p1_devtype & CLASS_BOMS) )
#define HAVE_JOY2	(_p2 && _p2_dev != -1 && !(_p2_devtype & CLASS_BOMS) )

//
// deviceUpdate() - update the USB devices if necessary.
//
void VDIP::deviceUpdate()
{
     monmsg match;

     // first task: update the state of the VDIP connection with any new data
     //   only happens when the system is ready for commands.  This is important
     //   because a few lines come in upon start-up, but they should only be
     //   processed after the prompt is received.

     if((_p1 && _p1_dev == -1) || (_p2 && _p2_dev == -1)) {
	  updateDevices();
     }

     // second task:  if the P2 device is a "disk" then try to read
     //   data from the disk, which could include the name of the chapr (as
     //   opposed to the firmware-written name) and/or the name of the remote
     //   brick.

     if (_p2 && _p2_dev != -1 && (_p2_devtype & CLASS_BOMS) && !_diskprocessed) {
	  processDisk();
     }
}

//
// DEBUG_USB_QD() - given a buffer, print out the QD data
//
void DEBUG_USB_QD(unsigned char *buffer)
{
     Serial.print("UA:");
     Serial.print(buffer[0],HEX);
     Serial.print(" ");
     Serial.print(buffer[1],HEX);
     Serial.print(" ");
     Serial.print(buffer[2],HEX);
     Serial.print(" ");
     Serial.print(buffer[3],HEX);
     Serial.print(" ");
     Serial.print(buffer[4],HEX);
     Serial.print(" ");
     Serial.print(buffer[5],HEX);
     Serial.print(" ");
     Serial.print(buffer[6],HEX);
     Serial.print(" ");
     Serial.print(buffer[7],HEX);
     Serial.print(" ");
     Serial.print(buffer[8],HEX);
     Serial.print(" ");
     Serial.print(buffer[9],HEX);
     Serial.print(" ");
     Serial.print(buffer[10],HEX);
     Serial.print(" ");
     Serial.print(buffer[11],HEX);
     Serial.print(" ");
     Serial.print(buffer[12],HEX);
     Serial.print(" ");
     Serial.print(buffer[13],HEX);
     Serial.print(" ");
     Serial.print(buffer[14],HEX);
     Serial.print(" ");
     Serial.print(buffer[15],HEX);
     Serial.println();
}

//
// updateDevices() - (assumes that this is called with a good reason, like a message
//                    from the monitor) Update the port1 (P1) and port2 (P2) devices
//                    with the USB device number and device type.  ASSUMES THAT WE'RE
//                    IN READY STATE!
//
void VDIP::updateDevices()
{
  unsigned char  incoming[32];

  Serial.println("attempting device update");

  cmd(VDIP_QP1,incoming,100);
  _p1_devtype = incoming[0];

  Serial.println("qp1 done");

  cmd(VDIP_QP2,incoming,100);
  _p2_devtype = incoming[0];

  Serial.println("qp2 done");

  // then query the first two USB devices to figure out which is which.  Note, though,
  // that hubs aren't supported in this code. That is, if an HID is plugged into a HUB
  // which is plugged into P1/P2, it may have a good type, but this code won't deal
  // with it correctly.

  {
#define MAX_CHECKED_DEVICES    4

    for (int i = 0; i < MAX_CHECKED_DEVICES; i++) {
	 Serial.println("checking");

	 delay(100);				// slight delay between reading devices

	 cmd(VDIP_QD,incoming,100,i);

//	 DEBUG_USB_QD(incoming);

	 if (incoming[DEV_LOCATION] == 1) {
	      _p1_dev = i;			// used to be incoming[DEV_ADDRESS];
	 } else if (incoming[DEV_LOCATION] == 2) {
	      _p2_dev = i;			// used to be incoming[DEV_ADDRESS];
	 }
    }
  }

  if (_p1) {
       Serial.print("P1 ");
       Serial.print(_p1_dev,HEX);
       Serial.print(" ");
       Serial.print(_p1_devtype,HEX);
       Serial.println("");
  }
  if (_p2) {
       Serial.print("P2 ");
       Serial.print(_p2_dev,HEX);
       Serial.print(" ");
       Serial.print(_p2_devtype,HEX);
       Serial.println("");
  }

  Serial.println("done with device update");
}

//
// readBytes() - given a byte count, do a blocking read of the number of bytes
//              requested from the monitor.  The array better be big enough...
//              THIS IS A BLOCKING CALL, but has a timeout that will cause a return
//		if hit.  If timeout is given as zero, this is a blocking call.
//		the timeout is given in milliseconds.
//
void VDIP::readBytes(int count, unsigned char  *retarray, int timeout)
{
     long	 target = millis() + timeout;
     
//     Serial.println("reading bytes");

     for (int i=0; i < count && ((timeout == 0) || (millis() < target)); i++) {
	  while(!recv(retarray+i)) {
	       // loop waiting for a good byte
	  }
     }

     // there is no useful differentiation between a timed-out call and not
}

//
// sendBytes() - given a byte count, do a blocking send of those bytes.  It is
//               assumed that the monitor is in a good place to receive the
//               bytes, or this routine MAY BLOCK.  For example, to send a
//               command, it should be in _ready mode.
//
void VDIP::sendBytes(int count, const char *buffer)
{
//     Serial.println("sending bytes");
     for (int i=0; i < count; i++) {
//	  Serial.write(buffer[i]);
	  while(!send(buffer[i],false)) {
	       // loop waiting for a good send
	  }
     }
}

//
// cmd() - submit a VDIP command.  Will fill in the return buffer with
//	   the return data.  The caller needs to know what to look for
//	   from the command, however.
//
//	   Each command waits for one line of return feedback (after
//	   maybe waiting for data).
//
//	   Does a flush() before executing the command.
//
//	   The number of bytes put into rbuf is returned.  This hides
//	   a lot of errors...
//
#define LONGCODES
int VDIP::cmd(vdipcmd cmd, unsigned char *rbuf, int timeout, int arg /* = 0 */)
{
     int		i = 0;
     long		target = millis() + timeout;	// timeout given in milliseconds
     char		cbuf[10];
     int		rbytes = 0;		// how many bytes to expect
     bool		twoStage = false;	// true if return has a number of bytes as the first stage
     
     flush();

     switch(cmd) {

	  // Command SC:  "SC (byte)\r"  or  "\0x86 (byte)\r"

     case VDIP_SC:
	  rbytes = 0;		// don't expect any return bytes for SC
#	ifdef LONGCODES
          {	// long command
	       cbuf[i++] = 'S';	// long codes for testing - get rid of these for short codes
	       cbuf[i++] = 'C';
	       cbuf[i++] = ' ';
	       cbuf[i++] = (unsigned char)arg;
	  }
#	else
	  {	// short command
	       cbuf[i++] = (unsigned char)0x86;
	       cbuf[i++] = ' ';
	       cbuf[i++] = (unsigned char)arg;
	  }
#	endif
	  break;

     case VDIP_QD:
	  rbytes = 32;		// whole bunch back for a QD
#	ifdef LONGCODES
          {	// long command
	       cbuf[i++] = 'Q';	// long codes for testing - get rid of these for short codes
	       cbuf[i++] = 'D';
	       cbuf[i++] = ' ';
	       cbuf[i++] = (unsigned char)arg;
	  }
#	else
	  {	// short command
	       cbuf[i++] = (unsigned char)0x85;
	       cbuf[i++] = ' ';
	       cbuf[i++] = (unsigned char)arg;
	  }
#	endif
	  break;

     // Command DRD: "DRD\r"  or "0x84\r"
     // (this command has a two-stage return, where the first byte specifies how many are next

     case VDIP_DRD:
	  rbytes = 1;		// will return a certain number of bytes
	  twoStage = true;
#	ifdef LONGCODES
          {	// long command
	       cbuf[i++] = 'D';	// long codes for testing - get rid of these for short codes
	       cbuf[i++] = 'R';
	       cbuf[i++] = 'D';
	  }
#	else
	  {	// short command
	       cbuf[i++] = (unsigned char)0x84;
	  }
#	endif
	  break;


     // Command QP1:  "QP1\r"  or  "0x2b\r"
     // Command QP2:  "QP2\r"  or  "0x2c\r"

     case VDIP_QP1:
     case VDIP_QP2:
	  rbytes = 2;		// two bytes back for the QP commands
#	ifdef LONGCODES
        {   // long command - comment this out for short commands
	     cbuf[i++] = 'Q';
	     cbuf[i++] = 'P';
	     cbuf[i++] = (cmd == VDIP_QP1)?'1':'2';
	}
#	else
	{   // short command
	     cbuf[i++] = (unsigned char)((cmd == VDIP_QP1)?0x2b:0x2c);
	}
#endif
	  break;
     }
	       
     cbuf[i++] = '\r';

     sendBytes(i,cbuf);			// non-blocking send the command

     // at this point, bytes come back for good command - this code assumes
     // that the commands are all "good" - ie - no mispelled commands

     if (rbytes) {		// we are looking for return bytes
	  readBytes(rbytes,rbuf,timeout);

	  // we can have a two-stager at this point, where the first byte of the
	  // rbytes above specifies how many bytes to receive.  If it is 0, then
	  // only a return line is expected.

	  if (twoStage) {
	       rbytes = rbuf[0];
	       // there seems to be the possibility that either "Bad Command" (BC) or "Command Failed" (CF)
	       // comes back.  In this case, ignore it and return zero.

	       switch(rbytes) {
	       case '\r':
	       case 'B':
	       case 'C':		// any of these indicates zero
		    flush();		// so flush the rest and return
		    return(0);

	       default:	
		    readBytes(rbytes,rbuf,timeout);
		    break;
	       }
	  }
     }

     // blindly get the return

     readLineMatch(true,response_messages,timeout);

     return(rbytes);
}

//
// flush() -    Since it is possible that async data can come from the
//		monitor before or after executing a command, this routine
//		checks for data sitting in the buffer or waiting in the VDIP
//		buffer.  If there is data, it is passed on to the routine
//		that processes async messages.
//
//		This routine assumes that if there is nothing in the buffer
//		when it is called, then nothing will be in the buffer by
//		the time this routine finishes.  sure, that means that there
//		is a race condition.
//
//		If there IS something in the buffer, then the routine will wait
//		for the configured number of milliseconds for the buffer to clear,
//		while processing the async messages that were sitting there.
//
//		It would probably make sense to do an "echo" sync after the
//		flush to ensure that we are truly in sync with the board.
//
void VDIP::flush()
{
     long	flushTimeoutMillis = 100;
     long	flushTarget = millis() + flushTimeoutMillis;

     readLine(false);				// go ahead and read something,

     while (_count != 0 && millis() < flushTarget) {		// if non-zero, there's something in the buffer
	  if (readLine(false)) {				// is it a line?
	       asyncProcess();					//  ... process it.
	  }
     }								// otherwise loop around until timeout

     _count = 0;						// finally, forcibly zero-out buffer
}

//
// asyncProcess() - process the line in the _buffer to see if it one of the
//		    async messages that comes out of the monitor that is
//		    useful.  If so, adjust flags accordingly.
//		       
void VDIP::asyncProcess()
{
     switch(matchLine(_buffer, async_messages)) {
     case MONINIT:	_init = true; break;
     case MONDD1:	_p1 = true; _p1_dev = -1; Serial.println("noticed P1"); break; 
     case MONDD2:	_p2 = true; _p2_dev = -1;  Serial.println("noticed P2"); break; 
     case MONDR1:	_p1 = false; Serial.println("P1 removed"); break;
     case MONDR2:	_p2 = false; _diskprocessed = false; Serial.println("P2 removed"); break;
     case MONPROMPT:	break;
     case MONND:	_diskprocessed = false; break;
     case MONNU:	break;
     default:		break;
     }
}

//
// processDisk() - called when a disk is discovered and properly located
//                 on P2.  This routine does everything that the Chapr can
//                 do with a flash drive, like update the name of the Chapr.
//
void VDIP::processDisk()
{
  _diskprocessed = true;
  Serial.println("disk inserted apparently");
}

//
// getJoyStick() - try to read data from the given joystick.  Assumes
//		   that it is a Joystick!  (for now)
//

int VDIP::getJoystick(int num, unsigned char *databuf)
{
     if (num == 1 && HAVE_JOY1) {
	  cmd(VDIP_SC,NULL,100,_p1_dev);
	  delay(20);
	  return(cmd(VDIP_DRD,databuf,100));
     }

     if (num == 2 && HAVE_JOY2) {
	  cmd(VDIP_SC,NULL,100,_p2_dev);
	  delay(20);
	  return(cmd(VDIP_DRD,databuf,100));
     }
}
