

#ifndef VDIP_h
#define VDIP_h

#include <stdint.h>

#include "VDIPSPI.h"

// when you query a USB device report on the VDIP, you get 32 bytes back with the
// following meanings.  The given defines can be used to index into the array of
// returned bytes:           // BYTES FROM VDIP REPORT - table 6.12
                             // Description
                             // ---------------------
#define DEV_ADDRESS	0    // USB address
#define DEV_CEP0	1    // Control End Point 0 Size
#define DEV_PIEPNO	2    // Pipe In End Point Number
#define DEV_PIEPSIZE	3    // Pipe In End Point Size
#define DEV_POEPNO	4    // Pipe Out End Point Number
#define DEV_POEPSIZE	5    // Pipe Out End Point Size
#define DEV_DATATOGGLES	6    // Data Toggles
#define DEV_TYPE	7    // Device Type: See Table 6.11
#define DEV_RESV8	8    // Reserved
#define DEV_LOCATION	9    // Location: 1 - USB Port 1, 2 - USB Port 2
#define DEV_MIINDEX	10   // MI Index
#define DEV_CLASS	11   // Device Class
#define DEV_SUBCLASS	12   // Device Sub Class
#define DEV_PROTOCOL	13   // Device Protocol
#define DEV_VID		14   // VID (MSB on 14, LSB on 15)
#define DEV_PID		16   // PID (MSB on 16, LSB on 17)
#define DEV_BCD		18   // BCD (MSB on 18, LSB on 19)
#define DEV_SPEED	20   // Device speed: 1 - Full Speed, 2 - Low Speed,
                             //               4 - Low Speed Device on Hub,
                             //               8 - Device on Low Speed Hub
#define DEV_RESV21_31	21   // Reserved (21 through 31)

// a VDIP port report is a single byte that indicates what's connected to the port.
// these values can be or'd together - eg 0x8C is a HUB with a printer and HID
//
//   0x00 - nothing connected
//   0x01 - FTDI-class device
//   0x02 - reserved (always zero)
//   0x04 - Printer
//   0x08 - HID
//   0x10 - CDC
//   0x20 - BOMS (flash disk for example)
//   0x40 - Unknown device (never or'd with anything else)
//   0x80 - HUB device

#define CLASS_NONE      0x00
#define CLASS_FTDI      0x01
#define CLASS_PRINTER   0x04
#define CLASS_HID       0x08
#define CLASS_CDC       0x10
#define CLASS_BOMS      0x20
#define CLASS_UNKNOWN   0x40
#define CLASS_HUB       0x80

  //   Description           Extended         Short & 0x      Note
  //   -----------------     ---------------- -------------   ----------------------------------
  //   cmd completion        "D:\>"           ">"  0x3e       Ready for a(nother) incoming cmd
  //   no disk               "No Disk"        "ND" 0x4e 0x44  In response to a single '\r'
  //   bad cmd               "Bad Command"    "BC" 0x42 0x43  command not recognized
  //   
  //   ASYNC RESPONSES
  //   Description           Extended               Short & 0x            Note
  //   -----------------     ----------------       --------------        ----------------------------------
  //   device into usb 1     "Device Detected P1"   "DD1" 0x44 0x44 0x31  may come up during power-up too
  //   device removed usb 1  "Device Removed P1"    "DR1" 0x44 0x52 0x31
  //   device into usb 2     "Device Detected P2"   "DD1" 0x44 0x44 0x32  may come up during power-up too
  //   device removed usb 2  "Device Removed P2"    "DR1" 0x44 0x52 0x32


  //   DISK OPS RESPONSES
  //   Description           Extended            Short & 0x       Note
  //   -----------------     ----------------    --------------   ----------------------------------
  //   file/dir not found    "Command Failed"    "CF" 0x43 0x46   comes up for USB dev # out of range
  //   disk full             "Disk Full"         "DF" 0x44 0x46   should never come up
  //   invalid access        "Invalid"           "FI" 0x46 0x49
  //   read only file        "Read Only"         "RO" 0x52 0x4f
  //   file open already     "File Open"         "FO" 0x46 0x4f
  //   dir not empty         "Dir Not Empty"     "NE" 0x4e 0x45
  //   filename invalid      "Filename Invalid"  "FN" 0x46 0x4e
  //   no upgrade file found "No Upgrade"        "NU" 0x4e 0x55   may come up during power-up too (only on P2)
  //
  //
  // completion of a command

typedef enum cmd { MONNOP = 0, 
		   MONDD1 = 1,
		   MONDR1 = 2,
		   MONDD2 = 3,
		   MONDR2 = 4,
		   MONINIT = 5,
		   MONPROMPT = 6,
		   MONND = 7,
		   MONBC = 8,
		   MONCF = 9,
		   MONNU = 10,
		   MONBLANK = 11,
		   MONUNKNOWN = 12
} monmsg;

typedef struct {
  const char	*message;
  monmsg	 cmd;
} monmessage;

typedef enum vcmd {
     VDIP_SC,			// Set Current - takes an arg
     VDIP_QP1,
     VDIP_QP2,
     VDIP_QD,
     VDIP_DRD			// Data Read
} vdipcmd;


//
// BUFFER_SIZE is the size of the VDIP communications buffer
//
#define BUFFER_SIZE   100

class VDIP : public VDIPSPI
{
public:

     VDIP(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin, uint8_t csReset);
     void deviceUpdate();
     void reset(int);
     int getJoystick(int, unsigned char *);
     void flush();

private:
     uint8_t _resetPin;
     char    _buffer[BUFFER_SIZE+1];
     int     _count;

  // _p1/2 is set true if the port is connected
  // _p1/2_dev is set to -1 if the USB device is unknown, otherwise the device number
  // _p1/2_devtype is the usb type/class

  bool _p1;            // true if something is connected to P1
  int  _p1_dev;        // the USB device number of the thing in P1
  int  _p1_devtype;    // the type of device in P1
  bool _p2;            // true if something is connected to P2
  int  _p2_dev;        // the USB device number of the thing in P2
  int  _p2_devtype;    // the type of device in P2

  int  _ready;         // true if the monitor is ready for a command
  int  _diskprocessed; // true if an inserted disk has been processed
  int  _init;	       // true if the VDIP has returned the initialized string

  void asyncProcess();
  int cmd(vdipcmd, unsigned char *rbuf, int timeout, int arg = 0);
  monmsg matchLine(char *, monmessage *);
  bool readLine(bool, long = 0);
  monmsg readLineMatch(bool, monmessage *, long = 0);
  void updateDevices();
  void readBytes(int count, unsigned char *, int);
  void sendBytes(int count, const char *);
  void processDisk();

};

#endif /* VDIP_h */





