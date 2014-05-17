#include <Arduino.h>

#include "VDIPSPI.h"
#include "VDIP.h"
#include "BT.h"
#include "nxt.h"
#include "sound.h"
#include "debug.h"
#include "config.h"
#include "settings.h"

extern void software_Reset();

extern settings myEEPROM;

extern sound beeper;

//#define HAVE_JOY1	(_p1 && _p1_dev != -1 && !(_p1_devtype & CLASS_BOMS) )
//#define HAVE_JOY2	(_p2 && _p2_dev != -1 && !(_p2_devtype & CLASS_BOMS) )

// turn this on for some useful debugging code
//#define DEBUG

#ifdef DEBUG
void DEBUG_PORT_CONFIG(portConfig *config)
{
     Serial.print("port: ");
     Serial.print(config->port);
     Serial.print(" - logical dev: ");
     Serial.print(config->usbDev);
     Serial.print(" - type: ");
     Serial.print(config->type);
     Serial.println("");
}

/*void VDIP::debug_port_config()
{
     DEBUG_PORT_CONFIG(&ports[0]);
     DEBUG_PORT_CONFIG(&ports[1]);
}*/

#endif

//
// deviceUpdate() - update the USB devices if necessary.  If there was a change
//			then this returns true, otherwise false.  Note that there
//			isn't any differentiation as to which port changed in
//			the return value.
//
bool VDIP::deviceUpdate()
{
     char		incoming[32];
     portConfig		portConfigBuffer;
     int		i;
     bool		changed = false;

     // mark the two ports so we know when they've been assigned

     for(i=2; i--; ) {
	  ports[i].flag = false;
     }

     // first, get the data for the connected devices - note that only the first
     // two logical USB ports are checked.  If this code supported HUBs, it would
     // be different, but it doesn't...

     // Note, too, that each port must be accounted for, or its configuration gets
     // zero'd out.

//     for (int i = 0; i < 2; i++) {
     for (int i = 0; i < 8; i++) {	// each xbox controller uses 4 usb devices - plan for two
	                                // potential xbox controllers
	  cmd(VDIP_QD,incoming,100,i);

	  // given a configuration buffer, get this port's data

	  mapDevice(i,incoming,&portConfigBuffer);

	  if(portConfigBuffer.port >= 0 && portConfigBuffer.port < 2) {	// we have a good assignment

	       if(ports[portConfigBuffer.port].flag == true) {
		    // this port has already been assigned during this update
		    // so ignore the rest of the logical devices. We assume that
		    // the first one is always the command device.
		    continue;
	       }

	       ports[portConfigBuffer.port].flag = true;	// mark this port as assigned

	       if(ports[portConfigBuffer.port].port != portConfigBuffer.port ||
		  ports[portConfigBuffer.port].type != portConfigBuffer.type ||
		  ports[portConfigBuffer.port].usbDev != portConfigBuffer.usbDev ||
		  ports[portConfigBuffer.port].vid != portConfigBuffer.vid ||
		  ports[portConfigBuffer.port].pid != portConfigBuffer.pid) {
		    
		    // this port HAS CHANGED

		    changed = true;

		    ports[portConfigBuffer.port].port = portConfigBuffer.port;
		    ports[portConfigBuffer.port].usbDev = portConfigBuffer.usbDev;
		    ports[portConfigBuffer.port].type = portConfigBuffer.type;
		    ports[portConfigBuffer.port].vid = portConfigBuffer.vid;
		    ports[portConfigBuffer.port].pid = portConfigBuffer.pid;

		    // check incoming device and call process routines if needed
                    //DEBUG_PORT_CONFIG(&portConfigBuffer);
		    if(portConfigBuffer.type == DEVICE_DISK) {
                         //debugPortConfig(&portConfigBuffer);
			 processDisk(&portConfigBuffer);
		    }

		    if(portConfigBuffer.type == DEVICE_NXT) {
			 processNXT(&portConfigBuffer);
		    }
	       }
	  }
     }

     for(i=2; i--; ) {
	  if(!ports[i].flag) {
	       if (ports[i].usbDev >= 0) {
		    // this port must now be empty

		    changed = true;

		    ports[i].usbDev = -1;

		    if(ports[i].type == DEVICE_DISK) {
			 ejectDisk();
		    }
		    if(ports[i].type == DEVICE_NXT) {
			 ejectNXT();
		    }
	       }
	  }
     }

     return(changed);
}

#ifdef DEBUG

void DEBUG_HEX_BYTE(unsigned char c)
{
     if(c < 16) {
	  Serial.print("0");
     }
     Serial.print(c,HEX);
}

//
// DEBUG_USB_QD() - given a buffer, print out the QD data
//
void DEBUG_USB_QD(int dev, unsigned char *buffer)
{
     Serial.print("UA (");
     Serial.print(dev);
     Serial.print("): ");
     DEBUG_HEX_BYTE(buffer[0]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[1]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[2]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[3]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[4]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[5]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[6]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[7]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[8]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[9]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[10]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[11]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[12]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[13]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[15]);
     DEBUG_HEX_BYTE(buffer[14]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[17]);
     DEBUG_HEX_BYTE(buffer[16]); Serial.print(" ");
     DEBUG_HEX_BYTE(buffer[19]);
     DEBUG_HEX_BYTE(buffer[18]); Serial.print(" ");
     Serial.println();
}

#endif

//
// portConnection() - returns the type, the VID, and the PID of the device connected to 
//		      the given port (either 0 or 1).  This "port" refers to the ChapR/VDIP
//		      USB connector 1 and 2.  Normally this is called by the gamepad object
//		      that is associated with a particular gamepad (1 or 2).
//	RETURNS: true if there is something connected, and the pointers are filled in
//		false if nothing is connected, and the pointers aren't touched
//
bool VDIP::portConnection(int port, int *type, unsigned short *vid, unsigned short *pid)
{
     if (ports[port].usbDev != -1) {
	  *type = ports[port].type;
	  *vid = ports[port].vid;
	  *pid = ports[port].pid;
	  return(true);
     }

     return(false);
}

//
// mapDevice() - given an incoming array from a QD command, map the device to the
//		 internal variables used for interacting with the device.
//		 The given "dev" is the logical USB port that is used when talking
//		 to the VDIP.  Each of two USB ports can have a logical dev, but
//		 only if it is connected.
//
void VDIP::mapDevice(int dev, char *deviceReport, portConfig *returnPortConfig)
{
//     DEBUG_USB_QD(dev, (unsigned char *) deviceReport);

     // given the report, map the port number = -1 means that it not a useful report

     returnPortConfig->port = deviceReport[DEV_LOCATION] - 1;	// maps 1 => 0, and 2=> 1, and 0 => -1
     returnPortConfig->usbDev = dev;

     returnPortConfig->vid = (deviceReport[DEV_VID+1] << 8) | (deviceReport[DEV_VID]&0x00ff);
     returnPortConfig->pid = (deviceReport[DEV_PID+1] << 8) | (deviceReport[DEV_PID]&0x00ff);

// OLD WAY TO DO THIS
//     if(deviceReport[DEV_VID] == '\x94') && (deviceReport[DEV_VID+1] == '\x06')) {

     if(returnPortConfig->vid == (unsigned short)0x0694) {
	  returnPortConfig->type = DEVICE_NXT;
     } else if(deviceReport[DEV_TYPE] == '\x08') {
	  returnPortConfig->type = DEVICE_CONTROLLER;
     } else if(deviceReport[DEV_TYPE] == '\x20') {
	  returnPortConfig->type = DEVICE_DISK;
     } else {
	  returnPortConfig->type = DEVICE_UNKNOWN;
     }

#ifdef DEBUG
     Serial.print("vid 0x");
     Serial.print(returnPortConfig->vid,HEX);
     Serial.print(" pid 0x");
     Serial.print(returnPortConfig->pid,HEX);
     Serial.print(" usbDev 0x");
     Serial.print(returnPortConfig->usbDev,HEX);

     Serial.print(" type 0x");
     Serial.print(returnPortConfig->type,HEX);
     Serial.print(" (0x");
     Serial.print(deviceReport[DEV_TYPE],HEX);
     Serial.println(")");
#endif
}


//
// cmd() - submit a VDIP command.
// portCmd() - a version of cmd that uses the right usb device based upon the port number (1 or 2)
//
//	This routine is somewhat "low-level" in that it communicates with the VDIP
//	about sending/receiving data - but much of that data is to/from the devices
//	that are CONNECTED to the VDIP.  So other routines often use this lower-level
//	routine to implement sending/recieving to the specific device.
//
//	A single VDIP command can send or receive a variable amount of data - but not
//	both send AND receive a variable amount of data in a single command.  So, some
//	commands need a buffer of data to send out while some need a buffer for return
//	data.  This command uses the same buffer for both input and output.
//
//	The given argument is used for the command or to specify the number of bytes
//	that should be SENT with the buffer.  For routines that return data, this
//	routine will fill in the buffer	with the return data.  The caller needs to
//	know what to look for from the command, however.
//
//	Each command waits for one line of return feedback (after maybe waiting for data).
//
//      Does a sync() before executing the command.
//
//	The number of bytes put into buf is returned.  This hides a lot of errors...
//
// 	NOTE - the timeout isn't fully implemented here - could be a problem

int VDIP::portCmd(int port, vdipcmd vcmd, char *buf, int timeout, int arg /* = 0 */)
{
     cmd(VDIP_SC,NULL,100,ports[port-1].usbDev);	// set the target VDIP port

     // first set the right output port, then issue the command

     return(cmd(vcmd,buf,timeout,arg));
}

int VDIP::cmd(vdipcmd cmd, char *buf, int timeout, int arg /* = 0 */)
{
     int	i = 0;
     char	cbuf[32];		// command buffer - must be big enough for biggest "rbytes" below
     int	rbytes = 0;		// how many bytes to expect
     bool	twoStage = false;	// true if return has a number of bytes as the first stage
     int	sendingCmd = false;	// true if the command sends out data
     
     sync();

     switch(cmd) {

     case VDIP_SUM:		// suspend monitor
	  {
	       cbuf[i++] = '\x17';
	  }
	  break;

     case VDIP_DSD:		// send data out command - the arg is the message size
	  rbytes = 0;		// don't expect any return bytes for DSD
	  sendingCmd = true;
	  {
	       cbuf[i++] = '\x83';
	       cbuf[i++] = '\x20';
	       cbuf[i++] = (char) arg;
	  }
	  break;


     case VDIP_SC:		// "set current" command - the arg is the device
	  rbytes = 0;		// don't expect any return bytes for SC
	  {
	       cbuf[i++] = '\x86';
	       cbuf[i++] = ' ';
	       cbuf[i++] = (char)arg;
	  }
	  break;

     case VDIP_QD:		// "query device" command - the arg is the device
	  rbytes = 32;		// whole bunch back for a QD
	  {
	       cbuf[i++] = '\x85';
	       cbuf[i++] = ' ';
	       cbuf[i++] = (char)arg;
	  }
	  break;

     case VDIP_DRD:		// "data read" - reads data from current device
	  rbytes = 1;		// will return a certain number of bytes
	  twoStage = true;
	  {
	       cbuf[i++] = '\x84';
	  }
	  break;

     case VDIP_QP:		// "query port" command
	  rbytes = 2;		// two bytes back for the QP commands
	  {
	       cbuf[i++] = ((arg == 1)?'\x2b':'\x2c');
	  }
	  break;

    case VDIP_OPR:
         rbytes = 0;
          {
               cbuf[i++] = '\x0E';
	       cbuf[i++] = ' ';
               for (int x = 0; x < 15; x++){
                 if (buf[x] == '\0'){
                   break;
                 }
                 cbuf[i] = buf[x];
                 i++;
               }       
          }
          break;
          
    case VDIP_RDF:
        rbytes = arg;
          {
              cbuf[i++] = '\x0B';
              cbuf[i++] = ' ';
              cbuf[i++] = '\x0';
              cbuf[i++] = '\x0';
              cbuf[i++] = '\x0';
              cbuf[i++] = arg;
          }
          break;
          
    case VDIP_CLF:
        rbytes = 0;
          {
            cbuf[i++] = '\x0A';
            cbuf[i++] = ' ';
            for (int x = 0; x < 15; x++){
                 if (buf[x] == '\0'){
                   break;
                 }
                 cbuf[i] = buf[x];
                 i++;
               }
          }
     case VDIP_FWV:
         rbytes = arg;
            {
              cbuf[i++] = '\x13';
            }
          break;
     }
          	       
     cbuf[i++] = '\r';

     (void)sendBytes(i,cbuf,0);		// send the command - blocking-style

     if(sendingCmd) {
	  (void)sendBytes(arg,buf,timeout);
     }

     // at this point, bytes come back for good command - this code assumes
     // that the commands are all "good" - ie - no mispelled commands

     if (rbytes) {		// we are looking for return bytes

	  if(!twoStage) {

	       // not a two stager, so just read directly into the return buffer

	       readBytes(rbytes,buf,timeout);

	  } else {

	       // we have a two-stager at this point, where the first byte of the
	       // rbytes above specifies how many bytes to receive.  If it is 0, then
	       // only a return line is expected.

	       readBytes(rbytes,cbuf,timeout);	// don't overwrite buffer yet

	       rbytes = cbuf[0];

	       // there seems to be the possibility that either "Bad Command" (BC) or "Command Failed" (CF)
	       // comes back.  In this case, ignore it and return zero.

	       switch(rbytes) {
	       case '\r':
	       case 'B':
	       case 'C':		// any of these indicates zero
		    flush();		// so flush the rest and return
		    return(0);

	       default:	
		    readBytes(1,cbuf,timeout);		// consume the '\r'
		    readBytes(rbytes,buf,timeout);	// then return the real bytes
		    break;
	       }
	  }
     }

     return(rbytes);
}

#define BIGENOUGH 25

//
// readFile() - helper method used to read a file on the disk and returns
//		the entire contents of the file, but only up to the "numToRead"
//		number of bytes.  If "lineOnly" is set true, the only the first
//		line is returned (up to the return or newline) - defaults to false.
//
//	RETURNS:  true if there was SOMETHING read, false if nothing
//	NOTE: this routine counts on the fact that if the file doesn't exist,
//		the VDIP will return the \xFE's.  It should "probabaly"
//		check the return of the first open, and not do anything
//		if the file can't be found.
//
bool VDIP::readFile(char *filename, char *buf, byte numToRead, bool lineOnly)
{
     // open the file for reading, then read, then close
     
     cmd(VDIP_OPR, filename, DEFAULTTIMEOUT, 0);
     cmd(VDIP_RDF, buf, DEFAULTTIMEOUT, numToRead);
     cmd(VDIP_CLF, filename, DEFAULTTIMEOUT, 0);

     // make sure it is null terminated

     buf[numToRead - 1] = '\0';

     // also replace any \xFE's with \0 because an \xFE is returned by the
     // VDIP when trying to read more bytes than are available in the file

     for (int i = 0; i < numToRead; i++){
	  if (buf[i] == '\xFE'){
	       buf[i] = '\0';
	  }
	  if(lineOnly && (buf[i] == '\r' || buf[i] == '\n')) {
	       buf[i] = '\0';
	  }
	       
     }
     return buf[0] != '\0';
}

//insert defines here for maxes and mins!!!!!!!!!!!!!!!!!!!!!!!!!!!

//
// processDisk() - called when a disk is discovered and properly located
//                 on P2.  This routine does everything that the Chapr can
//                 do with a flash drive, like update the name of the Chapr.
//
void VDIP::processDisk(portConfig *portConfigBuffer)
{    
     char buf[BIGENOUGH];

     // check that it's in port two (beep annoyingly otherwise)

     if(portConfigBuffer->port == 1) {

       // read through VDIP stuff looking for a text file with the name, personality etc.

       // PLEASE NOTE -- FILE NAMES MUST BE FEWER THAN 8 CHARACTERS

       // get the new name of the ChapR

       if(readFile("name.txt", buf, BIGENOUGH)){
         if (buf[EEPROM_NAMELENGTH - 1] == '\0'){
           myEEPROM.setName(buf);
         }
       }

       // get the new personality

       if(readFile("person.txt", buf, BIGENOUGH)){
         byte newNum = (byte) atoi(buf);
         if (newNum > 0 && newNum <= EEPROM_LASTPERSON){
           myEEPROM.setPersonality(newNum);
         }
       }

       // get the power-down timeout

       if(readFile("timeout.txt", buf, BIGENOUGH)){
         byte newNum = (byte) atoi(buf);
         if (newNum >= 0 && newNum <= EEPROM_MAXTIMEOUT){
         myEEPROM.setTimeout(newNum);
         }
       }

       // get the lag time

       if(readFile("lag.txt", buf, BIGENOUGH)){
         byte newNum = (byte) atoi(buf);
         if (newNum >= 0 && newNum <= EEPROM_MAXLAG){
         myEEPROM.setSpeed(newNum);
         }
       }

       // get the user mode

       if(readFile("mode.txt", buf, BIGENOUGH)){
         byte newNum = (byte) atoi(buf);
         if (newNum >= 0 && newNum <= EEPROM_MAXMODE){
         myEEPROM.setMode(newNum);
         }
       }
       
       // allows user to determine number of seconds in autonomous, teleOp and endgame (ChapR3 of EEPROM)
       // zero for either mode skips the mode

       if(readFile("mConfig.txt",buf, BIGENOUGH)){
	 char *ptr = buf;
	 for (int i = 0; i < 3; i++){
	   switch(i){
	   case 0: myEEPROM.setAutoLen(atoi(ptr));break;
	   case 1: myEEPROM.setTeleLen(atoi(ptr));break;
	   case 2: myEEPROM.setEndLen(atoi(ptr));break;
	   }
	   while (*ptr != '\r' && *ptr != '\0' && *ptr != '\n'){
	     ptr++;
	   }
	   while (*ptr == '\r' && *ptr == '\n'){
	     ptr++;
	   }
	   if (*ptr == '\0'){
	     break;
	   }
	 }
       }
     

       // contains the settings for the digital I/O pins (for FRC, aka ChapR3 of EEPROM)
     
       if (readFile("dgtlIn.txt", buf, BIGENOUGH)){
	 byte newNum = 0;
	 
	 for (int i = 0, ptr = 0; i < 8; i++){
	   byte bit = (buf[ptr] == '1')?1:0;
	   newNum |= bit<<i;
	   while (buf[ptr] != '\r' && buf[ptr] != '\0' && buf[ptr] != '\n'){
	     ptr++;
	   }
	   while (buf[ptr] == '\r' || buf[ptr] == '\n'){
	     ptr++;
	   }
	   if (buf[ptr] == '\0'){
	     break;
	   }
	 }
	 
	 myEEPROM.setDigitalInputs(newNum);
       }

       // contains the 4 analog inputs (for FRC, aka ChapR3 of EEPROM)
       
       if (readFile("analogIn.txt", buf, BIGENOUGH)){
	 char *ptr = buf;
	 for (int i = 0; i < 4; i++){
	   double value = atof(ptr);
	   if (value > 0 && value <= 5) {
	     value = (value*1023)/5;
		// TODO - translate to labview preferences here
		switch(i) {
		case 0:		myEEPROM.setAnalogInput1(value); break;
		case 1:		myEEPROM.setAnalogInput2(value); break;
		case 2:		myEEPROM.setAnalogInput3(value); break;
		case 3:		myEEPROM.setAnalogInput4(value); break;
		}
	   }
	   while (*ptr != '\r' && *ptr != '\0' && *ptr != '\n'){
	     ptr++;
	   }
	   while (*ptr == '\r' || *ptr == '\n'){
	     ptr++;
	   }
	   if (*ptr == '\0'){
	     break;
	   }
	 }
       }

       // get a target bluetooth connection name/ID AND connect if it is there
       // this MAY need to be changed to do the connection AFTER getting
       // done with the flash drive.  Note that this data IS NOT stored in
       // the EEPROM - instead, it is just used as the current paired device
       // and will be reset (like normal) whenever a new pairing is done.

       extern BT bt;

       if(readFile("targetID.txt", buf, BIGENOUGH,true)){
	    if (bt.addressFilter(buf,BIGENOUGH)) {	// useful address?
		 bt.setRemoteAddress(buf);
		 delay(100);
	    }
       }

// TODO: make target.txt work - it should take a NAME and find the BT ID for it

       if(readFile("target.txt", buf, BIGENOUGH,true)){
//	    Serial.print("target: \"");
//	    Serial.print(buf);
//	    Serial.println("\"");
//	    if(bt.nameToAddress(buf)) {
//		 bt.setRemoteAddress(buf);
//		 delay(100);
//	    }
       }

       // the confirm beep indicates that all files that existed were read
       // it doesn't confirm that all data was cool

       beeper.confirm();			 

     } else {

       // the disk was put in the wrong USB port!

       beeper.icky();

     } 
}

void VDIP::ejectDisk()
{
     //Serial.println("ejected disk");
}

void VDIP::processNXT(portConfig *portConfigBuffer)
{
	  char *name;
	  char *btAddress;
	  long	freeMemory;
          extern BT bt;
          if (myEEPROM.getResetStatus() == (byte) 0){
	    if(nxtQueryDevice(this,portConfigBuffer->usbDev,&name,&btAddress,&freeMemory)){
//	      Serial.print("btAddress: \"");
//	      Serial.print(btAddress);
//	      Serial.print("\"");
              bt.setRemoteAddress(btAddress);
              delay(100);
//	      Serial.print(myEEPROM.getResetStatus());
              myEEPROM.setResetStatus(1); // increments the "status" so that the ChapR knows it has been reset
//	      Serial.print(myEEPROM.getResetStatus());

              delay(1000);
              software_Reset();
            }
          }    
}

void VDIP::ejectNXT()
{  
     myEEPROM.setResetStatus(0);
}

//
// getJoyStick() - try to read data from the given joystick.  Assumes
//		   that it is a Joystick!  (for now)
//

int VDIP::getJoystick(int num, char *databuf)
{
//     if (ports[num].usbDev >= 0 && ports[num].type == DEVICE_CONTROLLER) {
     if (ports[num].usbDev >= 0) {
	  cmd(VDIP_SC,NULL,100,ports[num].usbDev);
	  return(cmd(VDIP_DRD,databuf,100));
     } else {
	  return(0);		// if not a controller, return zero bytes
     }
}

//
// zombieMode() - enters low-power zombie mode on the VDIP.  There is no
//		  coming back from this - you have to reset the device.
//

void VDIP::zombieMode()
{
     cmd(VDIP_SUM,NULL,100);
}

     
