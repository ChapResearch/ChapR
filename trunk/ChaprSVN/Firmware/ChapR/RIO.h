//
// RIO.h
//
//   This file (and the associated RIO.cpp) defines the command set and
//   message protocol for communicating with both the roboRIO and the cRIO
//

#ifndef RIO_H
#define RIO_H

#include "VDIP.h"
#include "gamepad.h"

// General Structure - 30 bytes total
// sync bytes - three 0xFFs (the following data is organized so that 
//              it is "impossible" to ever get three FFs in a row (if
//              this ever occurs, the packet should be thrown out
// command    - RIO commands (start/end tele/auto)
// data       - the information actually being sent (joysticks, alliance/position etc.)
// checksum   - verifies that the data sent is accurate (cannot be FF)

// These defines are used by createPacket() to compose a command based upon the
// logical settings of enable and mode (teleop or auto).

// cRIO command byte format -- CAN NEVER BE 0xFF!!!!!!!!!!!!
// bit 0 : FPGA Checksum (unknown use)                - 0
// bit 1 : Test (                                     - 0
// bit 2 : Resync                                     - 0
// bit 3 : FMS Attached                               - 0
// bit 4 : Autonomous                                 - 0 (USB w/ auto.txt)
// bit 5 : Enabled/disabled (0 - disabled)            - 0 (action button)
// bit 6 : Emergency Stop (0 to stop)                 - 1 (kill switch)
// bit 7 : Reset                                      - 0

// roboRIO command byte format (v1 of comms)
// bit 0:     eStop
// bit 1 - 3: Control_Reserved
// bit 4:     FMSAttached
// bit 5:     Enabled
// bit 6:     Autonomous
// bit 7:     Test

// cRIO commands (for old comms system)
#define CRIO_AUTO_BIT	0x10
#define CRIO_ESTOP_BIT	0x40
#define CRIO_ENABLE_BIT	0x20

#define CRIO_ESTOP(x)	((x)?0x00:CRIO_ESTOP_BIT)
#define CRIO_ENABLE(x)	((x)?CRIO_ENABLE_BIT:0x00)
#define CRIO_TELEOP(x)	((x)?0x00:CRIO_AUTO_BIT)

// roboRIO commands (for v0 of comms)
#define RRIO_TELE_OFF                                   0x00
#define RRIO_TELE_ON                                    0x04
#define RRIO_AUTO_ON                                    0x05

// roboRIO commands (for v1 of comms)
#define RRIO_AUTO_BIT   0x02
#define RRIO_ENABLE_BIT 0x04

#define RRIO_ENABLE(x)  ((x)?RRIO_ENABLE_BIT:0x00)
#define RRIO_TELEOP(x)  ((x)?0x00:RRIO_AUTO_BIT)

// Data Format (sent by personality_3 (aka 4) for both cRIO and roboRIO)
//------------------------------------------------------------------------
// cmd        : (starts or stops tele/auto etc.)         : 0
// joy1_TH_m  : (MSB of tophat or D-pad, in degrees      : 1  // can be FF
// joy1_TH_l  : (LSB of tophat or D-pad, in degrees      : 2  // can be FF
// joy1_type  : (based on index in drivers table)        : 3
// joy1_x1    : (-128 to 127)                            : 4  // can be FF
// joy1_y1    : (-128 to 127)                            : 5  // can be FF
// joy1_B1    : (bitmap of B0 to B6, where B0 is bit 0)  : 6
// joy1_x2    : (-128 to 127)                            : 7  // can be FF
// joy1_y2    : (-128 to 127)                            : 8  // can be FF
// joy1_B2    : (bitmap of B7 to B11, where B7 is bit 0) : 9
// joy1_x3    : (-128 to 127)                            : 10 // can be FF
// joy1_y3    : (-128 to 127)                            : 11 // can be FF
// zero       : (to prevent 3 FF's in a row)             : 12
// joy2_TH_m  : (MSB of tophat or D-pad, in degrees      : 13 // can be FF
// joy2_TH_l  : (LSB of tophat or D-pad, in degrees      : 14 // can be FF
// joy2_type  : (based on index in drivers table)        : 15
// joy2_x1    : (-128 to 127)                            : 16 // can be FF
// joy2_y1    : (-128 to 127)                            : 17 // can be FF
// joy2_B1    : (bitmap of B0 to B5, where B0 is bit 0)  : 18
// joy2_x2    : (-128 to 127)                            : 19 // can be FF
// joy2_y2    : (-128 to 127)                            : 20 // can be FF
// joy2_B2    : (bitmap of B6 to B11, where B6 is bit 0) : 21
// joy2_x3    : (-128 to 127)                            : 22 // can be FF
// joy2_y3    : (-128 to 127)                            : 23 // can be FF
//------------------------------------------------------------------------
//                                                         24 bytes total

class RIO
{
 public:
  int createPacket(byte *msgbuff, bool enable, Gamepad *g1, Gamepad *g2, bool mode, bool isRoboRIO);
  bool firePlugBT_ID(VDIP *vdip, int usbDev, char **btAddress);

 private:
  byte RIO_xlateTH(byte th, char c);
  byte checksum(byte *msgbuff, int size);
};

#endif RIO_H

