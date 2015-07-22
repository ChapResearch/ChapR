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
    return '0xFF'; // special case
  }
  th = (th-1)*45;
  Serial.println(th);
  switch(c){
  case 'm': // MSB
    return th>>4;
  case 'l': // LSB
    return th&0x00FF;
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

  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = (byte) cmd;
  msgbuff[size++] = (short) g1->vid;
  msgbuff[size++] = (short) g1->pid;
  msgbuff[size++] = RIO_xlateTH(g1->tophat,'m'); // MSB
  msgbuff[size++] = RIO_xlateTH(g1->tophat,'l'); // LSB
  msgbuff[size++] = g1->buttons&0xff;                              // joystick 1 B0 to B7
  msgbuff[size++] = (g1->buttons>>8)&0xff;                         // joystick 1 B8 to B11
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) g1->x1;				   // joystick 1 (left) X axis
  msgbuff[size++] = (byte) g1->y1;			           // joystick 1 (left) Y axis 
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) g1->x2;
  msgbuff[size++] = (byte) g1->y2;
  msgbuff[size++] = (byte) (g2->buttons&0xff);                     // joystick 2 B0 to B7
  msgbuff[size++] = (byte) g1->x3;				   // joystick 1 5th axis (usually x3)
  msgbuff[size++] = (byte) g1->y3;			           // joystick 1 6th axis (usually y3) 
  msgbuff[size++] = 0;
  msgbuff[size++] = (short) g2->vid;
  msgbuff[size++] = (short) g2->pid;
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) g2->x1;			           // joystick 2 (left) X axis
  msgbuff[size++] = (byte) g2->y1;			           // joystick 2 (left) Y axis 
  msgbuff[size++] = (byte) ((g2->buttons>>8)&0xff);                // joystick 2 B8 to B11
  msgbuff[size++] = (byte) g2->x2;
  msgbuff[size++] = (byte) g2->y2;
  msgbuff[size++] = (byte) RIO_xlateTH(g2->tophat, 'm'); // MSB
  msgbuff[size++] = (byte) RIO_xlateTH(g2->tophat, 'l'); // LSB
  msgbuff[size++] = (byte) g2->x3;
  msgbuff[size++] = (byte) g2->y3;
  msgbuff[size++] = 0;

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

