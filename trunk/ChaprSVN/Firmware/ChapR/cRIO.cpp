//
// cRIO.cpp  (see cRIO.h too)
//

#include <arduino.h>
#include "VDIP.h"
#include "gamepad.h"
#include "cRIO.h"
#include "settings.h"

//
// createPacket() - create the packet to be sent to the cRIO.
//		enable = TRUE will enable the program running on the CRIO
//		mode = TRUE is teleop mode, FALSE is autonomous mode
//
int cRIO::createPacket(byte *msgbuff, bool enable, Gamepad *g1, Gamepad *g2, bool mode)
{
  extern settings myEEPROM;

  // mode is defined as true = teleop and false = autonomous

  int cmd = CRIO_ESTOP(false) | CRIO_ENABLE(enable) | CRIO_TELEOP(mode);

  int size = 0;

  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = 0xff;
  msgbuff[size++] = (byte) cmd;
  msgbuff[size++] = myEEPROM.getDigitalInputs();
  msgbuff[size++] = 0;
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput1()>>8)&0x00FF); // MSB
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput1()); // LSB
  msgbuff[size++] = g1->tophat;
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput2()>>8)&0x00FF); // MSB
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput2()); // LSB
  msgbuff[size++] = g1->buttons&0x3f;                              // joystick 1 B0 to B5
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput3()>>8)&0x00FF); // MSB
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput3()); // LSB
  msgbuff[size++] = (g1->buttons>>6)&0x3f;                         // joystick 1 B6 to B11
  msgbuff[size++] = (byte) ((myEEPROM.getAnalogInput4()>>8)&0x00FF); // MSB
  msgbuff[size++] = (byte) (0x00FF&myEEPROM.getAnalogInput4()); // LSB
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
