//
// cRIO.h
//
//   This file (and the associated cRIO.cpp) defines the command set and
//   message protocol for communicating with the cRIO
//

#ifndef CRIO_H
#define CRIO_H

// General Structure - 30 bytes total
// sync bytes - three 0xFFs (the following data is organized so that 
//              it is "impossible" to ever get three FFs in a row (if
//              this ever occurs, the packet should be thrown out
// command    - corresponds to the type of data following
// data       - the information actually being sent
// checksum   - verifies that the data sent is accurate (cannot be FF)

//#define TELE_OFF                                       64
//#define TELE_ON                                        96
//#define AUTO_OFF                                       80 // potentially obsolete because the cRIO can start teleOp after this mode just fine
//#define AUTO_ON                                       112
#define TELE_OFF                                       64 0x40
#define TELE_ON                                        96 0x60
#define AUTO_OFF                                       80 0x50// the cRIO can start teleOp after this mode just fine
#define AUTO_ON                                       112 0x70
#define E_STOP                                          0

// the following values are empirical data gathered from the Driver's Station software
// representing what it sends when the modes are set.

#define TELE_OFF                                       0x40	//  0100 0000
#define TELE_ON                                        0x60	//  0110 0000
#define AUTO_OFF                                       0x50	//  0101 0000
#define AUTO_ON                                        0x70	//  0111 0000
#define E_STOP                                         0x00

// These defines are used by createPacket() to compose a command based upon the
// logical settings of enable and mode (teleop or auto).

#define CRIO_AUTO_BIT	0x10
#define CRIO_ESTOP_BIT	0x40
#define CRIO_ENABLE_BIT	0x20

#define CRIO_ESTOP(x)	((x)?0x00:CRIO_ESTOP_BIT)
#define CRIO_ENABLE(x)	((x)?CRIO_ENABLE_BIT:0x00)
#define CRIO_TELEOP(x)	((x)?0x00:CRIO_AUTO_BIT)

// CRIO_CMD byte format -- CAN NEVER BE 0xFF!!!!!!!!!!!!
// bit 0 : FPGA Checksum (unknown use)                - 0
// bit 1 : Test (                                     - 0
// bit 2 : Resync                                     - 0
// bit 3 : FMS Attached                               - 0
// bit 4 : Autonomous                                 - 0 (USB w/ auto.txt)
// bit 5 : Enabled/disabled (0 - disabled)            - 0 (action button)
// bit 6 : Emergency Stop (0 to stop)                 - 1 (kill switch)
// bit 7 : Reset                                      - 0

// CRIO_DATA format
// dgtl_in : (each bit is an input)                   : 0 FF
// zero    : (0)                                      : 1
// analog1 : (double)                                 : 2 FF
// analog1 : (continued)                              : 3 FF
// joy1_TH : (0, 1 to 8)                              : 4
// analog2 : (double)                                 : 5 FF
// analog2 : (continued)                              : 6 FF
// joy1_B1 : (bitmap of B0 to B5, where B0 is bit 0)  : 10
// analog3 : (continued)                              : 8 FF
// analog3 : (double)                                 : 9 FF
// joy1_B2 : (bitmap of B6 to B11, where B6 is bit 0) : 13
// analog4 : (double)                                 : 11 FF
// analog4 : (continued)                              : 12 FF
// zero    : (0)                                      : 22
// joy1_x1 : (-128 to 127)                            : 14 FF
// joy1_y1 : (-128 to 127)                            : 15 FF
// zero    : (0)                                      : 25
// joy1_x2 : (-128 to 127)                            : 17 FF
// joy1_y2 : (-128 to 127)                            : 18 FF
// joy2_B1 : (bitmap of B0 to B5, where B0 is bit 0)  : 16
// joy2_x1 : (-128 to 127)                            : 20 FF
// joy2_y1 : (-128 to 127)                            : 21 FF
// joy2_B2 : (bitmap of B6 to B11, where B6 is bit 0) : 19
// joy2_x2 : (-128 to 127)                            : 23 FF
// joy2_y2 : (-128 to 127)                            : 24 FF
// joy2_TH : (0, 1 to 8)                              : 7
//                                                    : 26 total bytes

class cRIO
{
 public:
     int createPacket(byte *msgbuff, bool enable, Gamepad *g1, Gamepad *g2, bool mode);
 private:
     byte checksum(byte *msgbuff, int size);

};

#endif CRIO_H

