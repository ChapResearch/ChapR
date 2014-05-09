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

#define TELE_OFF                                       64
#define TELE_ON                                        96
#define AUTO_OFF                                       80 // potentially obsolete because the cRIO can start teleOp after this mode just fine
#define AUTO_ON                                       112
#define E_STOP                                          0

// CRIO_CMD byte format -- CAN NEVER BE 0xFF!!!!!!!!!!!!
// bit 0 : FPGA Checksum (unknown use)                - 0
// bit 1 : Test (                                     - 0
// bit 2 : Resync                                     - 0
// bit 3 : FMS Attached                               - 0
// bit 4 : Autonomous                                 - 0 (USB w/ auto.txt)
// bit 5 : Enabled/disabled (0 - disabled)            - 0 (action button)
// bit 6 : Emergency Stop (0 to stop)                 - 1 (kill switch)
// bit 7 : Reset                                      - 0

// CRIO_PACKET format
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
  int createPacket(byte *msgbuff, int cmd, int size, Gamepad *g1, Gamepad *g2);
 private:
  byte checksum(byte *msgbuff, int size);

};

#endif CRIO_H

