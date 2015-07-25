//
// RIO.cpp  (see RIO.h too)
//

#include <Arduino.h>
#include "VDIP.h"
#include "gamepad.h"
#include "RIO.h"
#include "settings.h"

byte RIO::RIO_xlateTH(byte th, char c)
{
  if (th == 0){
    return 0xFF; // special case
  }
  int th_new  = (th-1)*45;
  switch(c){
  case 'm': // MSB
    return th_new>>8;
  case 'l': // LSB
    return th_new&0x00FF;
  default:
    return 0;
  }
}

//
// createPacket() - create the packet to be sent to the RIO.
//		enable = TRUE will enable the program running on the RIO
//		mode = TRUE is teleop mode, FALSE is autonomous mode
//
int RIO::createPacket(byte *msgbuff, bool enable, Gamepad *g1, Gamepad *g2, bool mode, bool isRoboRIO)
{
  extern settings myEEPROM;
  int cmd;

  // mode is defined as true = teleop and false = autonomous

  if (isRoboRIO){
    cmd = RRIO_ENABLE(enable) | RRIO_TELEOP(mode);
    /*if ((!enable) && mode){
      cmd = RRIO_TELE_OFF;
    }
    if (enable && mode){
      cmd = RRIO_TELE_ON;
    }
    if (enable && !mode){
      cmd = RRIO_AUTO_ON;
    }*/
  } else {
    cmd = CRIO_ESTOP(false) | CRIO_ENABLE(enable) | CRIO_TELEOP(mode);
  }

  uint8_t size = 0;

  msgbuff[size++] = (byte) 0xff;                                  // 1st of 3 sync bytes
  msgbuff[size++] = (byte) 0xff;                                  // 2nd of 3 sync bytes
  msgbuff[size++] = (byte) 0xff;                                  // 3rd of 3 sync bytes
  msgbuff[size++] = (byte) cmd;                                   // auto/tele, enabled etc.
  msgbuff[size++] = (byte) RIO_xlateTH(g1->tophat,'m');           // MSB of tophat or D-pad, in degrees
  msgbuff[size++] = (byte) RIO_xlateTH(g1->tophat,'l');           // LSB of tophat or D-pad, in degrees 
  msgbuff[size++] = (byte) g1->type;                              // (based on index in driver table)
  msgbuff[size++] = (byte) g1->x1;				  // joystick 1 (left) X axis
  msgbuff[size++] = (byte) g1->y1;			          // joystick 1 (left) Y axis 
  msgbuff[size++] = (byte) g1->buttons&0x7f;                      // joystick 1 B0 to B6
  msgbuff[size++] = (byte) g1->x2;                                // joystick 1 (right) X axis
  msgbuff[size++] = (byte) g1->y2;                                // joystick 1 (right) X axis
  msgbuff[size++] = (byte) (g1->buttons>>7)&0xff;                 // joystick 1 B7 to B11
  msgbuff[size++] = (byte) g1->x3;				  // joystick 1 5th axis (usually x3)
  msgbuff[size++] = (byte) g1->y3;			          // joystick 1 6th axis (usually y3) 
  msgbuff[size++] = (byte) 0;
  msgbuff[size++] = (byte) RIO_xlateTH(g2->tophat, 'm');          // MSB of tophat or D-pad, in degrees 
  msgbuff[size++] = (byte) RIO_xlateTH(g2->tophat, 'l');          // LSB of tophat or D-pad, in degrees
  msgbuff[size++] = (byte) g2->type;                              // (based on index in driver table)
  msgbuff[size++] = (byte) g2->x1;			          // joystick 2 (left) X axis
  msgbuff[size++] = (byte) g2->y1;			          // joystick 2 (left) Y axis 
  msgbuff[size++] = (byte) g2->buttons&0x7f;                      // joystick 2 B0 to B6
  msgbuff[size++] = (byte) g2->x2;                                // joystick 2 (right) X axis
  msgbuff[size++] = (byte) g2->y2;                                // joystick 2 (right) Y axis
  msgbuff[size++] = (byte) (g2->buttons>>7)&0xff;                 // joystick 2 B7 to B11
  msgbuff[size++] = (byte) g2->x3;                                // joystick 2 5th axis (usually x3)
  msgbuff[size++] = (byte) g2->y3;                                // joystick 2 6th axis (usually y3)

  byte cs = checksum(msgbuff + 3, size - 3);
  msgbuff[size++] = cs;

  return(size);			// total size of the message going over BT
}

byte RIO::checksum(byte *msgbuff, int size)
{
  byte cs = 0;

  for (int i = 0; i < size; i++, msgbuff++){
    cs += *msgbuff;
  }
  
  cs &= 0x7f; // sets the first bit to zero so that the
              // checksum is never FF

  return(cs);
}

