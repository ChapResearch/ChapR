/*
** v2f2f.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2F2F
** Main module
**
** Author: FTDI
** Project: Vinculum II
** Module: Vinculum II Applications
** Requires: VOS
** Comments: Refer to VNC1L Firmware User Manual for detailed information on operation
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"

#include "USB.h"
#include "USBHost.h"
#include "ioctl.h"
#include "UART.h"
#include "FAT.h"
#include "msi.h"
#include "BOMS.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define VOS_DEV_USBHOST_1  0
#define VOS_DEV_USBHOST_2  1
#define VOS_DEV_BOMS_1	   2
#define VOS_DEV_BOMS_2	   3
#define VOS_DEV_UART	   4

#define VOS_NUMBER_DEVICES 5

VOS_HANDLE hUSBHOST_1;                 // USB Host Port 1
VOS_HANDLE hUSBHOST_2;                 // USB Host Port 2
VOS_HANDLE hBOMS_1;                    // Bulk Only Mass Storage for USB disks
VOS_HANDLE hBOMS_2;                    // Bulk Only Mass Storage for USB disks
VOS_HANDLE hUART;                      // UART Interface Driver

vos_tcb_t *tcbFIRMWARE;
vos_tcb_t *tcbLEDTHREAD;

void firmware();
void ledThread();

// handles for both file systems
fat_context fatContext1, fatContext2;

// turn on LED flashing
char flash;

/* Declaration for IOMUx setup function */
void iomux_setup(void);

/* Main code - entry point to firmware */
void main(void)
{
	// UART Driver configuration context
	uart_context_t uartContext;
	// USB Host configuration context
	usbhost_context_t usbhostContext;

	vos_init(50, VOS_TICK_INTERVAL, VOS_NUMBER_DEVICES);
	vos_set_clock_frequency(VOS_48MHZ_CLOCK_FREQUENCY);
	vos_set_idle_thread_tcb_size(512);

	iomux_setup();

	// Initialise UART
	uartContext.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	uart_init(VOS_DEV_UART, &uartContext);

	// Initialise BOMS Device Drivers
	boms_init(VOS_DEV_BOMS_1);
	boms_init(VOS_DEV_BOMS_2);

	// Initialise USB Host
	usbhostContext.if_count = 8;
	usbhostContext.ep_count = 16;
	usbhostContext.xfer_count = 2;
	usbhostContext.iso_xfer_count = 2;
	usbhost_init(VOS_DEV_USBHOST_1, VOS_DEV_USBHOST_2, &usbhostContext);



	tcbFIRMWARE = vos_create_thread_ex(20, 4096, firmware, "Application", 0);
	tcbLEDTHREAD = vos_create_thread_ex(10, 256, ledThread, "LEDFlash", 0);

	vos_start_scheduler();

main_loop:
	goto main_loop;
}

VOS_HANDLE boms_attach(VOS_HANDLE hUSB, unsigned char devBOMS)
{
	usbhost_device_handle_ex ifDisk;
	usbhost_ioctl_cb_t hc_iocb;
	usbhost_ioctl_cb_class_t hc_iocb_class;
	msi_ioctl_cb_t boms_iocb;
	boms_ioctl_cb_attach_t boms_att;
	VOS_HANDLE hBOMS;

	// find BOMS class device
	hc_iocb_class.dev_class = USB_CLASS_MASS_STORAGE;
	hc_iocb_class.dev_subclass = USB_SUBCLASS_MASS_STORAGE_SCSI;
	hc_iocb_class.dev_protocol = USB_PROTOCOL_MASS_STORAGE_BOMS;

	// user ioctl to find first hub device
	hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_FIND_HANDLE_BY_CLASS;
	hc_iocb.handle.dif = NULL;
	hc_iocb.set = &hc_iocb_class;
	hc_iocb.get = &ifDisk;

	if (vos_dev_ioctl(hUSB, &hc_iocb) != USBHOST_OK)
	{
		printf("Disk not found");
		return NULL;
	}

	// now we have a device, intialise a BOMS driver for it
	hBOMS = vos_dev_open(devBOMS);

	// boms_attach
	boms_att.hc_handle = hUSB;
	boms_att.ifDev = ifDisk;

	boms_iocb.ioctl_code = MSI_IOCTL_BOMS_ATTACH;
	boms_iocb.set = &boms_att;
	boms_iocb.get = NULL;

	if (vos_dev_ioctl(hBOMS, &boms_iocb) != MSI_OK)
	{
		printf("BOMS attach failed");
		vos_dev_close(hBOMS);
		hBOMS = NULL;
	}

	return hBOMS;
}

void boms_detach(VOS_HANDLE hBOMS)
{
	msi_ioctl_cb_t boms_iocb;

	if (hBOMS)
	{
		boms_iocb.ioctl_code = MSI_IOCTL_BOMS_DETACH;
		boms_iocb.set = NULL;
		boms_iocb.get = NULL;

		if (vos_dev_ioctl(hBOMS, &boms_iocb) != MSI_OK)
		{
			printf("BOMS detach failed");
		}
	}

	vos_dev_close(hBOMS);
}

/*
** msgOnline
**
** Initialise the monitor.
**
** Parameters: none
** Returns: none
** Comments: Firmware version is stored in reserved area of ROM
*/
extern rom char userDataArea[16];
void msgOnline()
{
	char onlineStr[] = "\rVer V2F2F%s On-Line:\r";
	char letters[5];

	letters[0] = userDataArea[0];
	letters[1] = userDataArea[1];
	letters[2] = userDataArea[2];
	letters[3] = userDataArea[3];
	letters[4] = '\0';

	printf(onlineStr, letters);
}

/*
** monDeviceDetected
**
** Write the device detected message to the monitor.
**
** Parameters: hostPort: USB port that has detected a device
** Returns: none
** Comments:
*/
void monDeviceDetected(unsigned char hostPort)
{
	char strDeviceDet[] = "Device Detected P";
	char letter;

	printf(strDeviceDet);

	letter = hostPort + '0';
	printf("%c\r", letter);
}

/*
** monDeviceRemoved
**
** Write the device removed message to the monitor.
**
** Parameters: hostPort: USB port that has removed a device
** Returns: none
** Comments:
*/
void monDeviceRemoved(unsigned char hostPort)
{
	char strDeviceRem[] = "Device Removed P";
	char letter;

	printf(strDeviceRem);

	letter = hostPort + '0';
	printf("%c\r", letter);
}

/*
** monWriteFileName
**
** Read a filename in from the monitor.
**
** Parameters: dataToWrite: pointer to filename (padded to 8.3)
** Returns: nothing
** Comments: Filename is reformatted without padding and with '.' added for extension if required.
*/
void monWriteFileName(char *dataToWrite)
{
	unsigned char i;

	for (i = 0; i < 11; i++)
	{
		if (dataToWrite[i] == ' ')
		{
			if (i < 8)
			{
				if (dataToWrite[0] == '.')
					break;

				if (dataToWrite[8] == ' ')
					break;

				i = 8;
				printf(".");
			}
			else
			{
				break;
			}
		}
		else if (i == 8)
		{
			printf(".");
		}

		printf("%c", dataToWrite[i]);
	}
}

/*
** monAddNumberToConsole
**
** Write a number to the monitor.
**
** Parameters: dataToWrite: pointer to value to write
**  dataLength: size in bytes of number
**  Mon_Flags: different formats for IPA and IPH modes
** Returns: MON_SUCCESS for a number, MON_NO_CMD for no number
** Comments: Numbers are output LSB first.
**  Prepend number with $ for hexadecimal.
**  Numbers are displayed as single bytes.
*/
void monAddNumberToConsole(unsigned char *dataToWrite, unsigned char dataLength)
{
	char str[5];
	unsigned char data;
	unsigned char ch;

	str[0] = '$';
	str[3] = ' ';
	str[4] = '\0';

	while (dataLength)
	{
		dataLength--;
		data = *dataToWrite++;
		ch = data >> 4;

		if (ch > 9)
			str[1] = 'A' - 10 + ch;
		else
			str[1] = '0' + ch;

		ch = data & 0x0f;

		if (ch > 9)
			str[2] = 'A' - 10 + ch;
		else
			str[2] = '0' + ch;

		printf(str);
	}
}

/*
** monLedsGreen
**
** Turn on green LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsGreen()
{
	unsigned char data;

	vos_gpio_read_port(GPIO_PORT_A, &data);
	data &= (~0x60);
	data |= 0x20;
	vos_gpio_write_port(GPIO_PORT_A, data);
}

/*
** monLedsRed
**
** Turn on red LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsRed()
{
	unsigned char data;

	vos_gpio_read_port(GPIO_PORT_A, &data);
	data &= (~0x60);
	data |= 0x40;
	vos_gpio_write_port(GPIO_PORT_A, data);
}

/*
** destMakeUniqueDir
**
** Make a unique directory on the detination drive
** Format VF2Fxxxx where xxxx is a decimal counter
**
** Parameters: handle to destination file system
** Returns: status of file system operation
** Comments:
*/
unsigned char destMakeUniqueDir(fat_context fatContext)
{
	file_context_t fileFind;

#if 0
	const char *dirMatch = "VF2F";
	char dirFind[12];
#endif
	char dirName[12] = "VF2F0000   \0";
	// directory will begin at VF2F0001
	unsigned short count = 1;
	unsigned short dirnum;
	unsigned char i;
	unsigned char status;

	do
	{
		if (count > 9999)
			break;

		dirName[4] = ((count / 1000) % 10) + '0';
		dirName[5] = ((count / 100) % 10) + '0';
		dirName[6] = ((count / 10) % 10) + '0';
		dirName[7] = (count % 10) + '0';

		status = fat_dirTableFind(fatContext, &fileFind, (char *) dirName);

		if (status != FAT_OK)
		{
			monWriteFileName(dirName);
			printf("\r");
			status = fat_dirCreateDir(fatContext, (char *) dirName);

			if (status == FAT_OK)
			{
				status = fat_dirChangeDir(fatContext, (char *) dirName);

				if (status == FAT_OK)
				{
					return 0;          // passed
				}
			}

			break;
		}

		count++;
	}
	while (1);

	return -1;                         // failed
}

/*
** copyFile
**
** Copy a file from the source to the destination disk.
** Source file is opened for read only, destination is opened for write.
** Destination file will be overwritten if it exists.
**
** Parameters: handle to source file system
**             handle to destination file system
**             filename
** Returns: status of file system operation
** Comments:
*/
unsigned char copyFile(fat_context fatContextSrc, fat_context fatContextDest, char *filename)
{
	unsigned char status;
	// file handles for source and destination files
	static file_context_t fileSource;
	static file_context_t fileDest;
	unsigned long length;

	status = fat_fileOpen(fatContextSrc, &fileSource, filename, FILE_MODE_READ);
	status = fat_fileOpen(fatContextDest, &fileDest, filename, FILE_MODE_WRITE);
	length = fat_dirEntrySize(&fileSource);

	monWriteFileName(filename);

	status = fat_fileCopy(&fileSource, &fileDest);

	printf(" ");
	monAddNumberToConsole((unsigned char *) &length, 4);
	printf("\r");

	// close the file handles
	fat_fileClose(&fileSource);
	fat_fileClose(&fileDest);

	return status;
}

/*
** changeDirUp
**
** Change directory up one level on selected file system.
**
** Parameters: handle to  file system
** Returns: status of file system operation
** Comments:
*/
unsigned char changeDirUp(fat_context fatContext)
{
	unsigned char status;

	status = fat_dirChangeDir(fatContext, "..         ");
	return status;
}

/*
** copyDir
**
** Recursively copy a directory from the source to the destination disk.
**
** Parameters: handle to source file system
**             handle to destination file system
** Returns: status of file system operation
** Comments:
*/
unsigned char copyDir(fat_context fatContextSrc, fat_context fatContextDest)
{
	file_context_t fileFind;
	char filename[12];
	unsigned char status;

	memset(filename, 0, sizeof(filename));

	if (fat_dirTableFindFirst(fatContextSrc, &fileFind) == FAT_OK)
	{
		do
		{
			if (fat_dirEntryIsValid(&fileFind))
			{
				fat_dirEntryName(&fileFind, filename);

				if (fat_dirEntryIsDirectory(&fileFind))
				{
					monWriteFileName(filename);
					printf(" DIR\r");
					status = fat_dirChangeDir(fatContextSrc, (char *) filename);

					if (status == FAT_OK)
					{
						status = fat_dirCreateDir(fatContextDest, (char *) filename);

						if ((status == FAT_OK) || (status == FAT_EXISTS))
						{
							status = fat_dirChangeDir(fatContextDest, (char *) filename);

							if (status == FAT_OK)
							{
								copyDir(fatContextSrc, fatContextDest);
								status = changeDirUp(fatContextDest);

								if (status == FAT_OK)
								{
								}
							}
						}

						status = changeDirUp(fatContextSrc);

						if (status == FAT_OK)
						{
						}
					}

					printf(".. DIR\r");
				}
				else
				{
					copyFile(fatContextSrc, fatContextDest, filename);
				}
			}
		}
		while (fat_dirTableFindNext(fatContextSrc, &fileFind) == FAT_OK);
	}

	return status;
}

/*
** copyDisk
**
** Copy the source disk from the current directory to the destination disk
** current directory.
**
** Parameters: handle to source file system
**             handle to destination file system
** Returns: status of file system operation
** Comments:
*/
unsigned char copyDisk(fat_context fatContextSrc, fat_context fatContextDest)
{
	return copyDir(fatContextSrc, fatContextDest);
}

void firmware()
{
	unsigned char status;
	unsigned int handle;
	usbhost_ioctl_cb_t hc_iocb;
	unsigned char connect1, connect2;
	unsigned char state1, state2;
	common_ioctl_cb_t uart_iocb;

	// start UART and configure it for printf
	hUART = vos_dev_open(VOS_DEV_UART);
	
	/* set flow control to OFF */
	uart_iocb.ioctl_code = VOS_IOCTL_UART_SET_FLOW_CONTROL;
	uart_iocb.set.param = UART_FLOW_NONE;
	vos_dev_ioctl(hUART,&uart_iocb);
	/* default settings apply (9600 baud, 8 bits, 1 stop bit, no parity) */
	stdioAttach(hUART);

	msgOnline();

	hUSBHOST_1 = vos_dev_open(VOS_DEV_USBHOST_1);
	hUSBHOST_2 = vos_dev_open(VOS_DEV_USBHOST_2);

	state1 = state2 = PORT_STATE_DISCONNECTED;
	fat_init();

	do
	{
		// wait for enumeration to complete
		vos_delay_msecs(100);

		// user ioctl to see if bus available
		hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_GET_CONNECT_STATE;
		hc_iocb.get = &connect1;
		vos_dev_ioctl(hUSBHOST_1, &hc_iocb);

		if (connect1 == PORT_STATE_ENUMERATED)
		{
			if (state1 != PORT_STATE_ENUMERATED)
			{
				monDeviceDetected(1);
				state1 = connect1;
			}
		}
		else
		{
			if (state1 == PORT_STATE_ENUMERATED)
			{
				monDeviceRemoved(1);
				state1 = connect1;
			}
		}

		hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_GET_CONNECT_STATE;
		hc_iocb.get = &connect2;
		vos_dev_ioctl(hUSBHOST_2, &hc_iocb);

		if (connect2 == PORT_STATE_ENUMERATED)
		{
			if (state2 != PORT_STATE_ENUMERATED)
			{
				monDeviceDetected(2);
				state2 = connect2;
			}
		}
		else
		{
			if (state2 == PORT_STATE_ENUMERATED)
			{
				monDeviceRemoved(2);
				state2 = connect2;
			}
		}

		if ((state1 == PORT_STATE_ENUMERATED) && (state2 == PORT_STATE_ENUMERATED))
		{
			fatContext1 = fatContext2 = NULL;

			hBOMS_1 = boms_attach(hUSBHOST_1, VOS_DEV_BOMS_1);

			if (hBOMS_1)
			{
				fatContext1 = fat_open(hBOMS_1, 0, NULL);

				if (fatContext1)
				{
					// confirmation prompt (to UART) to show source disk is ready
					printf("A:\\>\r");
				}
			}

			hBOMS_2 = boms_attach(hUSBHOST_2, VOS_DEV_BOMS_2);

			if (hBOMS_2)
			{
				fatContext2 = fat_open(hBOMS_2, 0, NULL);

				if (fatContext2)
				{
					// confirmation prompt to show destination disk is ready
					printf("B:\\>\r");
				}
			}

			// both source and destination disks are configured properly
			if ((fatContext1 != NULL) && (fatContext2 != NULL))
			{
				// create (if required) and change into target direcory on destination disk
				if (destMakeUniqueDir(fatContext2) == 0)
				{
					copyDisk(fatContext1, fatContext2);

					// restore destination disk to top level directory
					changeDirUp(fatContext2);
				}

				// restore source disk to top level directory
				changeDirUp(fatContext1);

				printf("A:\\>\r");
			}

			if (fatContext1)
				fat_close(fatContext1);

			if (fatContext2)
				fat_close(fatContext2);

			boms_detach(hBOMS_1);
			boms_detach(hBOMS_2);

			// copy once only
			break;
		}
	}
	while (1);
}

void ledThread()
{
	while (1)
	{
		if (flash)
		{
			monLedsGreen();
		}

		vos_delay_msecs(250);

		if (flash)
		{
			monLedsRed();
		}

		vos_delay_msecs(250);
	}
}

