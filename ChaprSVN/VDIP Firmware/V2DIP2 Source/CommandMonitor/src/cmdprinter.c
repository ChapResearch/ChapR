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

#include "host.h"
#include "monitor.h"
#include "cmdmon.h"

/* Printer Class */
#define GET_PORT_STATUS 1
#define SOFT_RESET		2

/*
** cmd_pgs
**
** Printer get status.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Displays printer status byte.
**  Uses current device (SC command).
*/
unsigned char cmd_pgs()
{
	usb_deviceRequest_t desc_dev;
	char cr = 0x0D;
	unsigned char bitMode = 0;
	usbhost_ioctl_cb_t hc_ioctl;       // Ioctl block for our request...
	usbhost_ep_handle_ex ep = 0;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned char printerStatus = 0;
	usbhost_device_handle_ex ifDev;

	// no- parameters purge input until CR
	monReadCr();

	if (deviceList[curDevice].type == hostHasPrnClass)
	{
		ifDev = deviceList[curDevice].ifDev;

		// Get the control endpoint for our operation.
		hc_ioctl.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CONTROL_ENDPOINT_HANDLE;
		hc_ioctl.handle.dif = ifDev;
		hc_ioctl.get = &ep;

		vos_dev_ioctl(deviceList[curDevice].host, &hc_ioctl);

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_DEV_TO_HOST |
			USB_BMREQUESTTYPE_CLASS |
			USB_BMREQUESTTYPE_INTERFACE;
		desc_dev.bRequest = GET_PORT_STATUS;
		desc_dev.wValue = 0;
		desc_dev.wIndex = 0;
		desc_dev.wLength = 1;          // Sends back 1 byte in the data phase.

		hc_ioctl.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_SETUP_TRANSFER;
		hc_ioctl.handle.ep = ep;
		hc_ioctl.set = &desc_dev;
		hc_ioctl.get = &printerStatus; // Status returned from the printer

		result = vos_dev_ioctl(deviceList[curDevice].host, &hc_ioctl);
	}

	monAddNumberToConsole(&printerStatus, 1);
	monWrite(&cr, 1);

	return result;
}

/*
** cmd_psr
**
** Printer reset.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Uses current device (SC command).
*/
unsigned char cmd_psr()
{
	usb_deviceRequest_t desc_dev;
	unsigned char bitMode = 0;
	usbhost_ioctl_cb_t hc_ioctl;       // Ioctl block for our request...
	usbhost_ep_handle_ex ep = 0;           // Handle to the control endpoint
	unsigned char result = MON_SUCCESS;
	usbhost_device_handle_ex ifDev;

	// no- parameters purge input until CR
	monReadCr();

	if (deviceList[curDevice].type == hostHasPrnClass)
	{
		ifDev = deviceList[curDevice].ifDev;

		// Get the control endpoint for our operation.
		hc_ioctl.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CONTROL_ENDPOINT_HANDLE;
		hc_ioctl.handle.dif = ifDev;
		hc_ioctl.get = &ep;

		vos_dev_ioctl(deviceList[curDevice].host, &hc_ioctl); // Endpoint handle

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_CLASS |
			USB_BMREQUESTTYPE_INTERFACE;
		desc_dev.bRequest = SOFT_RESET;
		desc_dev.wValue = 0;
		desc_dev.wIndex = 0;           // TODO: change this to the interface number.
		desc_dev.wLength = 0;

		hc_ioctl.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_SETUP_TRANSFER;
		hc_ioctl.handle.ep = ep;
		hc_ioctl.set = &desc_dev;
		hc_ioctl.get = NULL;           // No data phase.

		result = vos_dev_ioctl(deviceList[curDevice].host, &hc_ioctl);

		if (result != USBHOST_OK)
			result = MON_ERROR_CMD_FAILED;
	}

	return result;
}

