//
// cRIO.h
//
//   This file (and the associated cRIO.cpp) defines the command set and
//   message protocol for communicating with the cRIO
//

#ifndef CRIO_H
#define CRIO_H

#define CRIO_KILL            0x00 // stops the program running on the cRIO
#define CRIO_DATA            0x01 // sends the current status of the joysticks (buttons and sticks)
#define CRIO_STARTAUTO       0x02 // begins the autonomous portion of the program
#define CRIO_STARTTELE       0x03 // begins the teleOp portion of the program

// General Structure
// sync bytes - two 0xFFs (the following data is organized so that 
//              it is "impossible" to ever get two FFs in a row (if
//              this ever occurs, the packet should be thrown out
// command    - corresponds to the type of data following
// data       - the information actually being sent
// checksum   - verifies that the data sent is accurate (cannot be FF)

// CRIO_DATA packet
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

