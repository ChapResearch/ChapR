/*
** cmdft.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP FTDI Device Commands
** in the Command Interpreter
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments:
**
** History:
**  1 – Initial version
**
*/
#ifndef __disk_h__
#define __disk_h__

unsigned char diskCheckDisk(VOS_HANDLE hUsb);
unsigned char diskRemoveDisk();
unsigned char diskResetDisk();
void diskCheckFwu();
void diskInit();

#endif                                 // __disk_h__

