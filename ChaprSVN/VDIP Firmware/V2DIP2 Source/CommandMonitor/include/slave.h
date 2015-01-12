/*
** slave.h
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Header file for Vinculum II V2DAP USB Slave Services
**
** Author: Gordon McNab
** Project: Vinculum II
** Module: Vinculum II Applications
** Requires: VOS
** Comments:
**
** History:
**  1 – Initial version
**
*/

#ifndef __slave_h__
#define __slave_h__

unsigned char slaveConnected(VOS_HANDLE hUsbHost);
unsigned char slaveDataRx(VOS_HANDLE hUsbFT232);
unsigned char slaveDataTx(VOS_HANDLE hUsbFT232);
unsigned char slaveSelectDevice();

#endif                                 // __slave_h__

