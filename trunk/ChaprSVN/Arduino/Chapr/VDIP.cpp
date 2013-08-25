
#include <Arduino.h>

#include "VDIPSPI.h"
#include "VDIP.h"
#include "nxt.h"
#include "BT.h"

//#define HAVE_JOY1	(_p1 && _p1_dev != -1 && !(_p1_devtype & CLASS_BOMS) )
//#define HAVE_JOY2	(_p2 && _p2_dev != -1 && !(_p2_devtype & CLASS_BOMS) )

//
// deviceUpdate() - update the USB devices if necessary.
//
void VDIP::deviceUpdate()
{
     char		incoming[32];
     portConfig		portConfigBuffer;
     int		i;

//     Serial.println("attempting device update");

     for(i=2; i--; ) {
	  ports[i].flag = false;
     }

     // first, get the data for the connected devices - note that only the first
     // two logical USB ports are checked.  If this code supported HUBs, it would
     // be different, but it doesn't...

     // Note, too, that each port must be accounted for, or its configuration gets
     // zero'd out.

     for (int i = 0; i < 2; i++) {

	  cmd(VDIP_QD,incoming,100,i);

	  // given a configuration buffer, get this port's data

	  mapDevice(i,incoming,&portConfigBuffer);

	  if(portConfigBuffer.port >= 0 && portConfigBuffer.port < 2) {	// we have a good assignment

	       ports[portConfigBuffer.port].flag = true;	// mark this port as assigned

	       if(ports[portConfigBuffer.port].port != portConfigBuffer.port ||
		  ports[portConfigBuffer.port].type != portConfigBuffer.type ||
		  ports[portConfigBuffer.port].usbDev != portConfigBuffer.usbDev) {
		    
		    // this port HAS CHANGED

		    ports[portConfigBuffer.port].port = portConfigBuffer.port;
		    ports[portConfigBuffer.port].usbDev = portConfigBuffer.usbDev;
		    ports[portConfigBuffer.port].type = portConfigBuffer.type;

		    // check incoming device and call process routines if needed

		    if(portConfigBuffer.type == DEVICE_DISK) {
			 processDisk();
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

//     Serial.println("done with device update");
}

// #define DEBUG
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

void VDIP::debug_port_config()
{
     DEBUG_PORT_CONFIG(&ports[0]);
     DEBUG_PORT_CONFIG(&ports[1]);
}

#endif

//
// mapDevice() - given an incoming array from a QD command, map the device to the
//		 internal variables used for interacting with the device.
//		 The given "dev" is the logical USB port that is used when talking
//		 to the VDIP.  Each of two USB ports can have a logical dev, but
//		 only if it is connected.
//
void VDIP::mapDevice(int dev, char *deviceReport, portConfig *returnPortConfig)
{
//     DEBUG_USB_QD(dev, deviceReport);

     // given the report, map the port number = -1 means that it not a useful report

     returnPortConfig->port = deviceReport[DEV_LOCATION] - 1;	// maps 1 => 0, and 2=> 1, and 0 => -1
     returnPortConfig->usbDev = dev;

     if((deviceReport[DEV_VID] == '\x94') && (deviceReport[DEV_VID+1] == '\x06')) {
	  returnPortConfig->type = DEVICE_NXT;
     } else if(deviceReport[DEV_TYPE] == '\x08') {
	  returnPortConfig->type = DEVICE_CONTROLLER;
     } else if(deviceReport[DEV_TYPE] == '\x20') {
	  returnPortConfig->type = DEVICE_DISK;
     } else {
	  returnPortConfig->type = DEVICE_UNKNOWN;
     }
}


//
// cmd() - submit a VDIP command.
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

//
// processDisk() - called when a disk is discovered and properly located
//                 on P2.  This routine does everything that the Chapr can
//                 do with a flash drive, like update the name of the Chapr.
//
void VDIP::processDisk()
{
     //check that it's in port two (beep annoyingly otherwise)
     //read through VDIP stuff looking for a text file with the name, personality etc.
     
     Serial.println("disk inserted apparently");
}

void VDIP::ejectDisk()
{
     Serial.println("ejected disk");
}

void VDIP::processNXT(portConfig *portConfigBuffer)
{
     Serial.println("NXT connected apparently");

     byte	output[50];
     int	size;
     int	i = 0;

     switch(1) {
     case 1:
	  output[i++] = 0x80;	// direct command with no response
	  output[i++] = 0x02;	// the direct command for play sound file
	  output[i++] = 0x00;	// play sound only once
	  output[i++] = 'w';	// name of the sound file - '\0' terminated
	  output[i++] = 'o';	// 2
	  output[i++] = 'o';	// 3
	  output[i++] = 'p';	// 4
	  output[i++] = 's';	// 5
	  output[i++] = '.';	// 6
	  output[i++] = 'r';	// 7
	  output[i++] = 's';	// 8
	  output[i++] = 'o';	// 9
	  output[i++] = '\0';	// 10
	  output[i++] = '\0';	// 11
	  output[i++] = '\0';	// 12
	  output[i++] = '\0';	// 13
	  output[i++] = '\0';	// 14
	  output[i++] = '\0';	// 15
	  output[i++] = '\0';	// 16
	  output[i++] = '\0';	// 17
	  output[i++] = '\0';	// 18
	  output[i++] = '\0';	// 19
	  output[i++] = '\0';	// 20
	  break;

     case 2:
	  output[i++] = 0x80;	// direct command with no response
	  output[i++] = 0x03;	// the direct command for play tone
	  output[i++] = 0xb8;	// 440 Hz (LSB)
	  output[i++] = 0x02;	//     (MSB)
	  output[i++] = 0x88;	// 5000 msecs (0x1388) (LSB)
	  output[i++] = 0x13;	//     (MSB)
	  break;

     case 3:
	  output[i++] = 0x80;	// direct command with no response
	  output[i++] = 0x01;	// STOP!
	  break;
     }

     //cmd(VDIP_SC,NULL,100,portConfigBuffer->usbDev);
     //cmd(VDIP_DSD,(char *)output,100,i);

     {
	  char *name;
	  char *btAddress;
	  long	freeMemory;
          extern BT bt;
          
	  if(nxtQueryDevice(this,portConfigBuffer->usbDev,&name,&btAddress,&freeMemory)){
            //Serial.println(name);
            //Serial.println(freeMemory);
            //Serial.print("\"");
            //Serial.print(btAddress);
            //Serial.println("\"");
            bt.setRemoteAddress(btAddress);
            delay(100);
          }
     }

}
void VDIP::ejectNXT()
{  
     extern void software_Reset();
     Serial.println("eject NXT");
     software_Reset();
}

//
// getJoyStick() - try to read data from the given joystick.  Assumes
//		   that it is a Joystick!  (for now)
//

int VDIP::getJoystick(int num, char *databuf)
{
     if (ports[num].usbDev >= 0 && ports[num].type == DEVICE_CONTROLLER) {
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

     
