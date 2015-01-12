/*
** cmdvdps.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DPS Commands
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
#include "vos.h"

#include "USBSlave.h"

#include "slave.h"
#include "monitor.h"

// Byte 1: status
// 3 Data available from slave (RX)
// 2 Data available to slave (TX)
// 1 Suspended
// 0 Connected
// Byte 2: TX waiting count
// Byte 3: RX waiting count

unsigned char cmd_qss()
{
	unsigned char stUsb;
	unsigned char st1 = 0, st2 = 0, st3 = 0;
	char cr = 0x0d;

	// no- parameters purge input until CR
	monReadCr();
	
	stUsb = slaveConnected(hUsb1);

	if (stUsb == usbsStateConfigured)
	{
		if (hFT232)
		{
			st1 = 0x01;
			st2 = slaveDataTx(hFT232);

			if (st2)
				st1 |= 0x04;

			st3 = slaveDataRx(hFT232);

			if (st3)
				st1 |= 0x08;
		}
	}
	else if (stUsb == usbsStateSuspended)
	{
		st1 = 0x02;
	}

	monAddNumberToConsole(&st1, 1);
	monAddNumberToConsole(&st2, 1);
	monAddNumberToConsole(&st3, 1);
	monWrite(&cr, 1);

	return MON_SUCCESS;
}

