//
// cRIO.h
//
//   This file (and the associated cRIO.cpp) defines the command set and
//   message protocol for communicating with the cRIO
//

#ifndef CRIO_H
#define CRIO_H

// General Structure
// sync bytes - two 0xFFs (the following data is organized so that 
//              it is "impossible" to ever get two FFs in a row (if
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

// CRIO_INFO format
// dgtl_in : (each bit is an input)                   : 0
// zero    : (0)                                      : 1
// analog1 : (double)                                 : 2
// analog1 : (continued)                              : 3
// zero    : (0)                                      : 4
// analog2 : (double)                                 : 5
// analog2 : (continued)                              : 6
// zero    : (0)                                      : 7
// analog3 : (continued)                              : 8
// analog3 : (double)                                 : 9
// zero    : (0)                                      : 10
// analog4 : (double)                                 : 11
// analog4 : (continued)                              : 12
// zero    : (0)                                      : 13
//                                                    : 14 total bytes

// CRIO_DATA format
// joy1_x1 : (-128 to 127)                            : 0
// joy1_TH : (0, 1 to 8)                              : 1
// joy1_y1 : (-128 to 127)                            : 2
// joy1_B1 : (bitmap of B0 to B5, where B0 is bit 0)  : 3
// joy1_x2 : (-128 to 127)                            : 4
// joy1_B2 : (bitmap of B6 to B11, where B6 is bit 0) : 5
// joy1_y2 : (-128 to 127)                            : 6
// zero    : (0)                                      : 7
// joy2_x1 : (-128 to 127)                            : 8
// joy2_TH : (0, 1 to 8)                              : 9
// joy2_y1 : (-128 to 127)                            : 10
// joy2_B1 : (bitmap of B0 to B5, where B0 is bit 0)  : 11
// joy2_x2 : (-128 to 127)                            : 12
// joy2_B2 : (bitmap of B6 to B11, where B6 is bit 0) : 13
// joy2_y2 : (-128 to 127)                            : 14
// zero    : (0)                                      : 15
//                                                    : 16 total bytes

class cRIO
{
 public:
  int CMDCompose(byte *msgbuff, int cmd, int size);
  int translate(byte *msgbuf, Gamepad *g1, Gamepad *g2);

 private:
  byte checksum(byte *msgbuff, int size);
  int translateGamepad(byte *msgbuf, Gamepad *g1);

};

#endif CRIO_H

