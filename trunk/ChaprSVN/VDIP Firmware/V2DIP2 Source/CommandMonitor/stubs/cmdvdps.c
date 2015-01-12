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
** Comments: Stubs only in this file
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"

#include "monitor.h"

// only used in V2DPS - needed for correct linking
VOS_HANDLE hFT232;

unsigned char cmd_qss()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_ERROR_BAD_CMD;
}

