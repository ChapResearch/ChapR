/*
** svcdisk.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Disk Dervices
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

#include "USBHost.h"
#include "USB.h"
#include "msi.h"
#include "BOMS.h"
#include "FAT.h"
#include "FirmwareUpdate.h"

#include "monitor.h"
#include "cfgmonitor.h"
#include "host.h"
#include "cmdmon.h"
#include "disk.h"

extern fat_context fatContext;

extern VOS_HANDLE hBoms;

void diskCheckFwu();

/*
** diskCheckDisk
**
** Checks if a disk is connected to a USB port.
**
** Parameters: none
** Returns: MON_SUCCESS disk connected, MON_ERROR_INVALID no disk.
** Comments: Checks for BOMS disk and valid FAT file system.
*/
unsigned char diskCheckDisk(VOS_HANDLE hUsb)
{
	char cr = 0x0d;
	void *findHandle;
	unsigned char i;
	unsigned char lun, maxlun;
	unsigned char status = MON_ERROR_INVALID;
	msi_ioctl_cb_t boms_cb;
	boms_ioctl_cb_attach_t boms_att;
	char *updateFile = "FTRFBV2 FTD";

	fat_init();
	diskInit();
	
	for (i = 0; i < DEV_LIST_SIZE; i++)
	{
		if (deviceList[i].ifDev != NULL)
		{
			if (hUsb == deviceList[i].host)
			{
				if (deviceList[i].type == hostHasBOMSClass)
				{
					boms_cb.ioctl_code = MSI_IOCTL_BOMS_ATTACH;
					boms_cb.set = &boms_att;
					boms_att.hc_handle = hUsb;
					boms_att.ifDev = deviceList[i].ifDev;

					status = vos_dev_ioctl(hBoms, &boms_cb);

					if (status == MSI_OK)
					{
						boms_cb.ioctl_code = MSI_IOCTL_GET_MAX_LUN;
						boms_cb.get = &maxlun;
						vos_dev_ioctl(hBoms, &boms_cb);
						
						// No LUN support
						if (maxlun == 0)
						{
							fatContext = fat_open(hBoms, 0, &status);

							if ((fatContext) && (status == FAT_OK))
							{
								status = MON_SUCCESS;
							}
							else
							{
								status = MON_ERROR_INVALID;
							}
						}
						else
						{
							 
							for (lun = 1; lun <= maxlun; lun++)
							{
								boms_cb.ioctl_code = MSI_IOCTL_SET_LUN;
								boms_cb.set = &lun;
								vos_dev_ioctl(hBoms, &boms_cb);

								fatContext = fat_open(hBoms, 0, &status);

								if ((fatContext) && (status == FAT_OK))
								{
									status = MON_SUCCESS;
									break;
								}
								else
								{
									status = MON_ERROR_INVALID;
								}
							}

						}
					}
					else
					{
						status = MON_ERROR_INVALID;
					}

					break;
				}
			}
		}

		if (status == MON_SUCCESS)
		{
			break;
		}
	}

	if (status == MON_SUCCESS)
	{
#ifdef CFGMONITOR_FIRMWARE_UPDATE
		vos_memcpy(param1Data, updateFile, 11);
		diskCheckFwu();
#endif                                 // CFGMONITOR_FIRMWARE_UPDATE
	}

	return status;
}

/*
** diskRemoveDisk
**
** Removes a disk when disconnected from a USB port
**
** Parameters: none
** Returns: MON_SUCCESS.
** Comments: Frees setorage in API.
*/
unsigned char diskRemoveDisk(void)
{
	fat_close(fatContext);

	// reset file open state
	diskInit();

	return MON_SUCCESS;
}

/*
** diskResetDisk
**
** Detaches a disk allowing the file system to be reset.
**
** Parameters: none
** Returns: MON_SUCCESS disk connected, MON_ERROR_INVALID no disk.
** Comments:
*/
unsigned char diskResetDisk()
{
	VOS_HANDLE diskHandle;
	msi_ioctl_cb_t boms_cb;
	unsigned char status;

	diskHandle = fat_getDevHandle(fatContext);

	boms_cb.ioctl_code = MSI_IOCTL_BOMS_DETACH;

	status = vos_dev_ioctl(hBoms, &boms_cb);

	if (status == MSI_OK)
	{
	}

	return diskCheckDisk(diskHandle);
}

/*
** diskCheckFwu
**
** Checks a disk for a firmware update file.
**
** Parameters: param1: filename of update
** Returns: none
** Comments: Checks for BOMS disk and valid FAT file system.
*/
extern rom short userDataArea[8];
void diskCheckFwu()
{
	char *strChangeMain = "Change MAIN\r";
	unsigned char status;
	short verFile[8];
	short verROM;
	file_context_t fd;
	char i;

	// check for firmware update file: default is "FTRFBV2.FTD"
	status = fat_fileOpen(fatContext, &fd, (char *) param1Data, FILE_MODE_READ);

	if (status == FAT_OK)
	{
		// move to userDataArea in ROM file
		status = fat_fileSetPos(&fd, 0x30);
		if (status == FAT_OK)
		{
			// read 16 bytes from ROM file where userDataArea is located
			status = fat_fileRead(&fd, 16, (unsigned char *)verFile, NULL, NULL);
			if (status == FAT_OK)
			{
				// signify version identical
				status = FAT_EXISTS;
				
				// compare version number in ROM to file
				for (i = 0; i < 8; i++)
				{
					verROM = userDataArea[i];
					
					// data read from ROM is byte swapped
					if ((verFile[i] >> 8) != (verROM & 0xff)) 
					{
						status = FAT_OK;
					}
					if ((verFile[i] & 0xff) != (verROM >> 8)) 
					{
						status = FAT_OK;
					}
				}
				
				// if version numbers differ then update firmware
				if (status == FAT_OK)
				{
					fat_fileSetPos(&fd, 0);
					status = FirmwareUpdateFATFileFeedback(&fd, 0x1f000, FIRMWARE_UPDATE_FEEDBACK_UART);
				}
			}
		}
		fat_fileClose(&fd);
	}

	if (status != FAT_OK)
	{
		monNoUpgrade();
	}
}

