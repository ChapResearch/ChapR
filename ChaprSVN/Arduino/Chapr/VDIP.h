
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
#define DEV_VID		14   // VID (LSB on 14, MSB on 15)
#define DEV_PID		16   // PID (LSB on 16, MSB on 17)
#define DEV_BCD		18   // BCD (LSB on 18, MSB on 19)
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


#define DEFAULTTIMEOUT 100


#define CLASS_NONE      0x00
#define CLASS_FTDI      0x01
#define CLASS_PRINTER   0x04
#define CLASS_HID       0x08
#define CLASS_CDC       0x10
#define CLASS_BOMS      0x20
#define CLASS_UNKNOWN   0x40
#define CLASS_HUB       0x80

typedef enum vcmd {
     VDIP_SC,			// Set Current - takes an arg
     VDIP_QP,			// Query Port - arg is port number
     VDIP_QD,			// Query Device - arg is device
     VDIP_DRD,			// Data Read from current device
     VDIP_DSD,			// Data Send to current device
     VDIP_SUM,			// suspend monitor (low power mode)
     VDIP_OPR,                  // open a file for reading
     VDIP_RDF,                  // read from file (specifies how many bytes)
     VDIP_CLF,                  //closes the currently open file
     VDIP_FWV                   //returns the firmware version
} vdipcmd;

typedef enum _deviceType {
     DEVICE_UNKNOWN,
     DEVICE_CONTROLLER,
     DEVICE_DISK,
     DEVICE_NXT
} deviceType;

typedef struct {
     int	port;		// the physical port number for the configuration
     int	usbDev;		// the USB device associated with this config (-1 means not in use)
     deviceType	type;		// the type of device in use on the port
     int	flag;		// the updated/touched flag
} portConfig;

//
// BUFFER_SIZE is the size of the VDIP communications buffer
//
#define BUFFER_SIZE   100

class VDIP : public VDIPSPI
{
public:

     VDIP(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin, uint8_t csReset);
     void deviceUpdate();
     bool sync();
     int getJoystick(int, char *);
     void zombieMode();
     int cmd(vdipcmd, char *rbuf, int timeout, int arg = 0);
     void reset();

private:
     uint8_t _resetPin;

     portConfig ports[2];	// two physical ports [0] = port1, [1] = port2

     int  _resetDelay;	// we've been reset, and are still in reset mode (set by reset())
     long _resetTarget;	// target time before out of reset (set by reset())

     bool readBytes(int count, char *, int);
     bool sendBytes(int count, const char *, int);
     bool readFile(char *name, char *buf, byte numToRead);
     void processDisk(portConfig *portConfigBuffer);
     void ejectDisk();
     void processNXT(portConfig *);
     void ejectNXT();
     void flush(int = 100);
     void init();

     void updateDevices();
     void mapDevice(int, char *, portConfig *);
     void debugPortConfig(portConfig*);
     void bootBrick();

};

#endif /* VDIP_h */





