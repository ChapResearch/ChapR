/*
** cmdvdfc.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DFC Commands
** in the Command Interpreter
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments: Stubs only
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"

#include "monitor.h"

unsigned char cmd_a()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

unsigned char cmd_b()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

unsigned char cmd_ims()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

unsigned char cmd_imf()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

unsigned char cmd_cps()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

unsigned char cmd_cpf()
{
	// no- parameters purge input until CR
	monReadCr();
	return MON_SUCCESS;
}

