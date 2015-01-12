/*
** cmdconfig.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Configuration Commands
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

#include "SPISLAVE.h"
#include "FIFO.h"
#include "UART.h"
#include "USBHost.h"
#include "USB.h"
#include "MSI.h"
#include "BOMS.h"
#include "FAT.h"

#include "monitor.h"
#include "cmdmon.h"
#include "disk.h"

#define SECTOR_SIZE 512

extern fat_context fatContext;

/*
** cmd_cr
**
** Empty command (carriage return)
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Returns success for an empty command line
*/
unsigned char cmd_cr()
{
	return MON_SUCCESS;
}

/*
** cmd_fwv
**
** Display firmware version
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Calls monitor functions to obtain version number
*/
unsigned char cmd_fwv()
{
	char *strMain = "\rMAIN ";
	char *strRprg = "\rRPRG 1.00R\r";
#ifdef V2MSC_FIRMWARE
	char *strFrmw = "-V2MSC";
#endif // V2MSC_FIRMWARE	

	// no- parameters purge input until CR
	monReadCr();
	
	// example:
	// MAIN 03.69-RC5VDAPF
	// RPRG 1.00R
	monWrite(strMain, 6);
	monVersion();
#ifdef V2MSC_FIRMWARE
	monWrite(strFrmw, 6);
#endif // V2MSC_FIRMWARE	
	monWrite(strRprg, 12);
	return MON_SUCCESS;
}

/*
** cmd_echo
**
** Synchronisation echo command
**
** Parameters: none
** Returns: MON_NO_CMD
** Comments: Returns 'e' or 'E' to the monitor to aid in synchronisation.
**  Does not return a prompt after the command is processed.
*/
unsigned char cmd_echo()
{
	char cr = 0x0d;

	// no- parameters purge input until CR
	monReadCr();
	
	monWrite((char *) &cmdData[0], 1); // Echos back either an 'e' or 'E'
	monWrite(&cr, 1);
	return MON_NO_CMD;                 // no prompt!!!
}

/*
** cmd_scs
**
** Switch to Short Command Set
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Commands are entered as a single character in binary mode
*/
unsigned char cmd_scs()
{
	// no- parameters purge input until CR
	monReadCr();
	
	Mon_Flags |= MON_SCS;
	return MON_SUCCESS;
}

/*
** cmd_ecs
**
** Switch to Extended Command Set
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Use long command names
*/
unsigned char cmd_ecs()
{
	// no- parameters purge input until CR
	monReadCr();
	
	Mon_Flags &= ~MON_SCS;
	return MON_SUCCESS;
}

/*
** cmd_ipa
**
** Switch to ASCII input mode
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Can either accept 0x, $ for hexadecimal values or decimal values
*/
unsigned char cmd_ipa()
{
	// no- parameters purge input until CR
	monReadCr();
	
	Mon_Flags |= MON_IPA;
	return MON_SUCCESS;
}

/*
** cmd_iph
**
** Switch to binary imput mode
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_iph()
{
	// no- parameters purge input until CR
	monReadCr();
	
	Mon_Flags &= ~MON_IPA;
	return MON_SUCCESS;
}

/*
** cmd_sbd
**
** Change the monitor baud rate
**
** Parameters: param1: new baud rate (compatible with VNC1L settings)
** Returns: MON_SUCCESS
** Comments: Baud rate is converted from the VNC1L value to an actual
**  baud rate value before being sent to the UART driver.
**  The following tabe can be used as a guide:
**   Baud                VNC1L
**              Byte 1   Byte 2   Byte 3
**   300        0x10     0x27     0x00
**   600        0x88     0x13     0x00
**   1200       0xC4     0x09     0x00
**   2400       0xE2     0x04     0x00
**   4800       0x71     0x02     0x00
**   9600       0x38     0x41     0x00
**   19200      0x9C     0x80     0x00
**   38400      0x4E     0xC0     0x00
**   57600      0x34     0xC0     0x00
**   115200     0x1A     0x00     0x00
**   230400     0x0D     0x00     0x00
**   460800     0x06     0x40     0x00
**   921600     0x03     0x80     0x00
**   1000000    0x03     0x00     0x00
**   1500000    0x02     0x00     0x00
**   2000000    0x01     0x00     0x00
**   3000000    0x00     0x00     0x00
*/
unsigned char cmd_sbd()
{
	char cr = 0x0d;
	unsigned long baud;
	unsigned long div;
	common_ioctl_cb_t uart_iocb;
	const unsigned char divfrac[8] = {0, 4, 2, 1, 3, 5, 6, 7};

	// parameters are a single 3 byte number
	monReadNumber(1, 3);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 3);
	monPrompt();

	// byte swap the divisor bits of the baud rate
	div = (param1 >> 16) & 0xff;
	div |= (param1 & 0x3f00);

	// subinteger divisors below 1Mbaud only
	if (div == 0)
		baud = 3000000;
	else if (div == 1)
		baud = 2000000;
	else if (div == 2)
		baud = 1500000;
	else
	{
		div *= 8;
		// add in the subinteger divisor
		div += divfrac[(param1 >> 14) & 0x03];

		// calculate actual baud rate
		baud = 3000000 * 8 / div;
	}

	vos_delay_msecs(100);

	// set the baud rate on the UART
	uart_iocb.ioctl_code = VOS_IOCTL_UART_SET_BAUD_RATE;
	uart_iocb.set.uart_baud_rate = baud;
	vos_dev_ioctl(hMonitor, &uart_iocb);

	vos_delay_msecs(100);

	return MON_SUCCESS;
}

/*
** cmd_sd
**
** Dump the contents of a sector to the monitor.
**
** Parameters: param1: sector number to display
** Returns: MON_SUCCESS
** Comments: This command will always write 512 bytes to the monitor.
*/
unsigned char cmd_sd()                 // Sector Dump
{
	// Dumps a 512 byte sector of a BOMS device to the monitor.
	msi_xfer_cb_t xfer_cb;
	unsigned short numRead = 0;
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned char cr = 0x0D;
	unsigned char status;
	vos_semaphore_t s;
	unsigned char dataRead[SECTOR_SIZE];

	monReadNumber(1, 4);               // Read a number in from the monitor and validate it...
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 4);

	if (result == MON_SUCCESS)
	{
		vos_init_semaphore(&s, 0);
		xfer_cb.buf = dataRead;
		xfer_cb.buf_len = SECTOR_SIZE;
		xfer_cb.total_len = SECTOR_SIZE;
		xfer_cb.s = &s;
		xfer_cb.do_phases = MSI_PHASE_ALL;
		xfer_cb.sector = param1;       // Sector Number Read in from the monitor.
		xfer_cb.status = MSI_NOT_ACCESSED;

		status = vos_dev_read(hBoms, (unsigned char *) &xfer_cb, sizeof(msi_xfer_cb_t), NULL);

		if (status != MSI_OK)
			result = MON_ERROR_CMD_FAILED;
	}

	vos_dev_write(hMonitor, dataRead, SECTOR_SIZE, &numRead); // Write the sector out to the monitor.

	return result;
}

/*
** cmd_sw
**
** Write a 512 byte sector to a disk
**
** Parameters: param1: the sector number to write
** Returns: MON_SUCCESS
** Comments: The command must be followed by 512 bytes of data to fill the
**  selected sector.
*/
unsigned char cmd_sw()
{
	// Writes a 512 byte sector from a BOMS device to the monitor.
	msi_xfer_cb_t xfer_cb;
	unsigned short numWritten = 0;
	unsigned short numRead = 0;
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned char cr = 0x0D;
	unsigned char status;
	vos_semaphore_t s;
	unsigned char dataToWrite[SECTOR_SIZE];

	monReadNumber(1, 4);                                        // Read a number in from the monitor and validate it...
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 4);

	vos_dev_read(hMonitor, dataToWrite, SECTOR_SIZE, &numRead); // Read the 512 bytes of data from the monitor.

	if (result == MON_SUCCESS)
	{
		vos_init_semaphore(&s, 0);

		xfer_cb.buf = dataToWrite;
		xfer_cb.buf_len = SECTOR_SIZE;
		xfer_cb.total_len = SECTOR_SIZE;
		xfer_cb.s = &s;
		xfer_cb.do_phases = MSI_PHASE_ALL;
		xfer_cb.sector = param1;       // Sector Number Read in from the monitor.
		xfer_cb.status = MSI_NOT_ACCESSED;

		status = vos_dev_write(hBoms, (unsigned char *) &xfer_cb, sizeof(msi_xfer_cb_t), NULL);

		if (status != MSI_OK)
			result = MON_ERROR_CMD_FAILED;
	}

	return MON_SUCCESS;
}

/*
** cmd_fwu
**
** Check for a firmware update file on the current disk
**
** Parameters: param1: filename to use as an update
** Returns: MON_SUCCESS
** Comments: No error is reported if the file is not found.
**  The firmware update from a disk is not supported at present.
*/
unsigned char cmd_fwu()
{
	unsigned char status;
	unsigned char result;

	monReadFileName(1);
	// purge until CR
	monReadCr();

	result = monValidateFileName(1);

	if (result == MON_SUCCESS)
	{
		diskCheckFwu();
	}

	return result;
}

/*
** cmd_ior
**
** Read from an IO port
**
** Parameters: param1: port number
** Returns: MON_SUCCESS
** Comments: Select port A, B, C or D from the table below
**
**    VNC1L Port   VNC2 GPIO Port   Byte 1
**        AD           C             0x00
**        AC           D             0x01
**        BD           A             0x02
**        BC           B             0x03
*/
unsigned char cmd_ior()
{
	unsigned char bitVal;
	char cr = 0x0D;

	// Read in the port number.
	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 1);

	// Read the appropriate GPIO pins.
	switch (param1)
	{
	case 0:
		vos_gpio_read_port(GPIO_PORT_C, &bitVal);
		break;

	case 1:
		vos_gpio_read_port(GPIO_PORT_D, &bitVal);
		break;

	case 2:
		vos_gpio_read_port(GPIO_PORT_A, &bitVal);
		break;

	default:
		vos_gpio_read_port(GPIO_PORT_B, &bitVal);
		break;
	}

	// Got data from the GPIOs, send to the monitor.
	monAddNumberToConsole(&bitVal, 1);
	monWrite(&cr, 1);

	return MON_SUCCESS;
}

/*
** cmd_iow
**
** Write to an IO port
**
** Parameters: param1: 3 bytes with port number, mask and bits to set
** Returns: MON_SUCCESS
** Comments: Select port A, B, C or D from the table below
**
**    VNC1L Port   VNC2 GPIO Port   Byte 1
**        AD           C             0x00
**        AC           D             0x01
**        BD           A             0x02
**        BC           B             0x03
*/
unsigned char cmd_iow()
{
	unsigned char portID;
	unsigned char bitSel;
	unsigned char bitVal;

	// Read in 3 bytes from the monitor.
	monReadNumber(1, 3);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 3);

	// Mask of the appropriate data from param1...
	portID = ((param1 & 0xFF0000) >> 16);
	bitSel = ((param1 & 0x00FF00) >> 8);
	bitVal = param1 & 0x0000FF;

	switch (portID)
	{
	case 0:
		vos_gpio_set_port_mode(GPIO_PORT_C, bitSel); // Set the ports to input or output
		vos_gpio_write_port(GPIO_PORT_C, bitVal);
		break;

	case 1:
		vos_gpio_set_port_mode(GPIO_PORT_D, bitSel); // Set the ports to input or output
		vos_gpio_write_port(GPIO_PORT_D, bitVal);
		break;

	case 2:
		vos_gpio_set_port_mode(GPIO_PORT_A, bitSel); // Set the ports to input or output
		vos_gpio_write_port(GPIO_PORT_A, bitVal);
		break;

	default:
		vos_gpio_set_port_mode(GPIO_PORT_B, bitSel); // Set the ports to input or output
		vos_gpio_write_port(GPIO_PORT_B, bitVal);
		break;
	}

	return MON_SUCCESS;
}

/*
** cmd_ini
**
** Restart the firmware
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: this is not supported
*/
typedef void (*RestartLocation)(void);
unsigned char cmd_ini()
{
	RestartLocation pfwRestart;

	pfwRestart = 0x3c0;                // ignore type mismatch warning
	pfwRestart();
	return MON_SUCCESS;
}

