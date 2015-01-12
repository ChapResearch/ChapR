/*
** svchost.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP USB Host Services
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

#include "vos.h"
#include "devman.h"

#include "USBHost.h"
#include "USB.h"

#include "monitor.h"
#include "host.h"
#include "cfgmonitor.h"

// USB device lists
// currently selected device
unsigned char curDevice;
// table of handles to devices
usbDevList deviceList[DEV_LIST_SIZE];

unsigned char hostConnected(VOS_HANDLE hUsbHost)
{
	usbhost_ioctl_cb_t usbhost_iocb;
	unsigned char i;

	usbhost_iocb.ioctl_code = VOS_IOCTL_USBHOST_GET_CONNECT_STATE;
	usbhost_iocb.get = &i;
	vos_dev_ioctl(hUsbHost, &usbhost_iocb);

	return i;
}

unsigned char hostAddToDeviceList(VOS_HANDLE hUsbHost)
{
	// use handle for IOCTL to usb host and get next device (first one is NULL!)
	// then find first empty slot int deviceList and add this handle there
	usbhost_ioctl_cb_t hc_iocb;        // ioctl block
	usbhost_device_handle_ex ifDev;    // handle to the next device.
	unsigned char i;
	unsigned char status = MON_ERROR_CMD_FAILED;
	unsigned char curSet = 0;

	// Don't update current device if we are in FT232 mode
	if (curDevice == 'S')
	{
		curSet = 1;
	}
	
	ifDev = NULL;
	hc_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_NEXT_HANDLE;
	hc_iocb.get = &ifDev;

	do
	{
		hc_iocb.handle.dif = ifDev;

		status = vos_dev_ioctl(hUsbHost, &hc_iocb);

		if (status != USBHOST_OK)
		{
			break;
		}

		if (ifDev)
		{
			for (i = 0; i < DEV_LIST_SIZE; i++)
			{
				if (deviceList[i].ifDev == NULL)
				{
					deviceList[i].ifDev = ifDev; // Set to the handle of our device...
					deviceList[i].host = hUsbHost;
					deviceList[i].type = hostGetDevType(hUsbHost, ifDev);

					// move the currently selected device to the first inserted device
#ifdef CFGMONITOR_SC_SET_FOR_INSERTED_DEVICES
					if (curSet == 0)
					{
						// do not select hub devices or disk devices
						if ((deviceList[i].type & (hostHasHub | hostHasBOMSClass)) == 0)
						{
							// set currently selected device to this device
							curDevice = i;
							// do not do this more than once
							curSet = 1;
						}
					}
#endif
					break;
				}
			}
		}
	}
	while (ifDev != NULL);

	return curDevice;
}

unsigned char hostRemoveFromDeviceList(VOS_HANDLE hUsbHost)
{
	// go throught device list and remove all devices that are on this usb host
	// might be able to do an IOCTL on the deivce handle and see if it returns an error?
	unsigned char i;

	for (i = 0; i < DEV_LIST_SIZE; i++)
	{
		// selected device removed, restore to default
#ifdef CFGMONITOR_SC_RESET_FOR_REMOVED_DEVICES
		if (curDevice == i)
		{
			curDevice = 0;
		}
#endif
		if (deviceList[i].ifDev != NULL)
		{
			if (hUsbHost == deviceList[i].host)
			{
				deviceList[i].ifDev = NULL;
				deviceList[i].host = NULL;
				deviceList[i].type = 0;
			}
		}
	}

	return MON_SUCCESS;
}

unsigned char hostSuspend(VOS_HANDLE hUsbHost)
{
	usbhost_ioctl_cb_t hc_iocb;        // IOCTL block

	hc_iocb.ioctl_code = VOS_IOCTL_USBHUB_SET_PORT_SUSPEND;
	hc_iocb.hub_port = 0;
	hc_iocb.handle.dif = 0;

	if (vos_dev_ioctl(hUsbHost, &hc_iocb) == USBHOST_OK)
	{
		return MON_SUCCESS;
	}

	return MON_NO_CMD;
}

unsigned char hostWake(VOS_HANDLE hUsbHost)
{
	usbhost_ioctl_cb_t hc_iocb;        // IOCTL block

	hc_iocb.ioctl_code = VOS_IOCTL_USBHUB_CLEAR_PORT_SUSPEND;
	hc_iocb.hub_port = 0;
	hc_iocb.handle.dif = 0;

	if (vos_dev_ioctl(hUsbHost, &hc_iocb) == USBHOST_OK)
	{
		return MON_SUCCESS;
	}

	return MON_NO_CMD;
}

unsigned char hostGetDevType(VOS_HANDLE hUsbHost, usbhost_device_handle_ex ifDev)
{
	unsigned char mask = 0;
	unsigned char usbClass;
	usbhost_ioctl_cb_t host_ioctl_cb;
	usbhost_ioctl_cb_t hc_iocb_ftdiCheck;
	usbhost_ioctl_cb_class_t hc_iocb_class;
	usbhost_ioctl_cb_vid_pid_t hc_iocb_vid_pid;

	host_ioctl_cb.handle.dif = ifDev;

	// find device classes present on the bus
	host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_VID_PID;
	host_ioctl_cb.get = &hc_iocb_vid_pid;
	vos_dev_ioctl(hUsbHost, &host_ioctl_cb);

	if (hc_iocb_vid_pid.vid == USB_VID_FTDI)
	{
		mask = hostHasFTDI;
	}
	else
	{
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CLASS_INFO;
		host_ioctl_cb.get = &hc_iocb_class;
		vos_dev_ioctl(hUsbHost, &host_ioctl_cb);

		switch (hc_iocb_class.dev_class)
		{
		case USB_CLASS_CDC_CONTROL:
			mask = hostHasCDCClass;
			break;

		case USB_CLASS_PRINTER:
			mask = hostHasPrnClass;
			break;

		case USB_CLASS_HID:
			mask = hostHasHIDClass;
			break;

		case USB_CLASS_HUB:
			mask = hostHasHub;
			break;

		case USB_CLASS_MASS_STORAGE:
			mask = hostHasBOMSClass;
			break;

		default:
			mask = hostHasUnknown;
		}
	}

	return mask;
}

unsigned char hostGetPortDevType(VOS_HANDLE hUsbHost)
{
	unsigned char mask = 0;
	unsigned char i;

	for (i = 0; i < DEV_LIST_SIZE; i++)
	{
		if (deviceList[i].ifDev != NULL)
		{
			if (hUsbHost == deviceList[i].host)
			{
				mask |= deviceList[i].type;
			}
		}
	}

	return mask;
}

unsigned char hostSetDeviceFtdi(unsigned char device)
{
	// select a device from the device table
	if ((device < DEV_LIST_SIZE) || (device == 'S'))
	{
		if (deviceList[device].ifDev != NULL)
		{
			deviceList[device].type = hostHasFTDI;
		}

		return MON_SUCCESS;
	}

	return MON_ERROR_CMD_FAILED;
}

unsigned char hostSelectDevice(unsigned char device)
{
	// select a device from the device table
	if ((device < DEV_LIST_SIZE) || (device == 'S'))
	{
		curDevice = device;
		return MON_SUCCESS;
	}

	return MON_ERROR_CMD_FAILED;
}

