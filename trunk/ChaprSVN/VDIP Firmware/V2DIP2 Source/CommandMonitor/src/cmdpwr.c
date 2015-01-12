/*
** cmdprinter.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Printer Commands
** in the Command Interpreter
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments: Refer to VNC1L Firmware User Manual for detailed command information
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"

#include "USBHost.h"
#include "USB.h"

#include "monitor.h"
#include "cmdmon.h"
#include "host.h"

/*
** cmd_sud
**
** Suspend disks after use.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_sud()
{
	// no- parameters purge input until CR
	monReadCr();

	Mon_Flags |= MON_SUD;
	// suspend mode will be entered after return
	return MON_SUCCESS;
}

/*
** cmd_wkd
**
** Wake disks so they are not suspended.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_wkd()
{
	// no- parameters purge input until CR
	monReadCr();

	Mon_Flags &= ~MON_SUD;
	hostWake(hUsb2);
	return MON_SUCCESS;
}

/*
** cmd_sum
**
** Suspend monitor.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Puts the CPU into a low power state until a RI (Ring Indicator) is
**  detected by the UART.
*/
unsigned char cmd_sum()
{
	// no- parameters purge input until CR
	monReadCr();

	monLedsOffNow();

#ifndef V2MSC_FIRMWARE
	hostSuspend(hUsb1);                // Suspend both host ports.
#endif // V2MSC_FIRMWARE	
	hostSuspend(hUsb2);

	vos_power_down(VOS_WAKE_ON_UART_RI);

	return MON_SUCCESS;
}

