//
// cRIO.cpp  (see cRIO.h too)
//

#include <arduino.h>
#include "VDIP.h"
#include "gamepad.h"
#include "cRIO.h"
#include "settings.h"

// createPacket() - also translates 
int cRIO::createPacket(byte *msgbuff, int cmd, Gamepad *g1, Gamepad *g2)
{
  extern settings myEEPROM;
 
  int size = 0;

  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = (byte) cmd;
  msgbuff[size++] = myEEPROM.getDigitalInputs();
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput1());
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput1()>>8)&0x00FF);
  msgbuff[size++] = g1->tophat;
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput2());
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput2()>>8)&0x00FF);
  msgbuff[size++] = g1->buttons&0x3f;                              // joystick 1 B0 to B5
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput3());
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput3()>>8)&0x00FF);
  msgbuff[size++] = (g1->buttons>>6)&0x3f;                         // joystick 1 B6 to B11
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput4());
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput4()>>8)&0x00FF);
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) g1->x1;				   // joystick 1 (left) X axis
  msgbuff[size++] = (byte) g1->y1;			           // joystick 1 (left) Y axis 
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) g1->x2;
  msgbuff[size++] = (byte) g1->y2;
  msgbuff[size++] = (byte) (g2->buttons&0x3f);                     // joystick 2 B0 to B5
  msgbuff[size++] = (byte) g2->x1;			           // joystick 2 (left) X axis
  msgbuff[size++] = (byte) g2->y1;			           // joystick 2 (left) Y axis 
  msgbuff[size++] = (byte) ((g2->buttons>>6)&0x3f);                // joystick 2 B6 to B11
  msgbuff[size++] = (byte) g2->x2;
  msgbuff[size++] = (byte) g2->y2;
  msgbuff[size++] = (byte) g2->tophat;

  byte cs = checksum(msgbuff + 4, size - 4);
  msgbuff[size++] = cs;

  return(size);			// total size of the message going over BT
}

byte cRIO::checksum(byte *msgbuff, int size)
{
  byte cs = 0;

  for (int i = 0; i < size; i++, msgbuff++){
    cs += *msgbuff;
  }
  
  cs &= 0x7f; // sets the first bit to zero so that the
              // checksum is never FF

  return(cs);
}
