/*
** cmdft.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP FTDI Device Commands
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

#define FT_SYSTEM_CLOCK_FREQ 3000000

#define USBHOST_BAUD_2000000 2000000
#define USBHOST_BAUD_3000000 3000000

/* Non-Standard bRequest codes */
#define USBHOST_MODEMCTRL	 0x01
#define USBHOST_SETFLOWCTRL	 0x02
#define USBHOST_SETBAUDRATE	 0x03
#define USBHOST_SETDATA		 0x04
#define USBHOST_GETMODEMSTAT 0x05
#define USBHOST_SETLATTIMER	 0x09
#define USBHOST_SETBITMODE	 0x0b
#define USBHOST_GETBITMODE	 0x0c

/*
** ftGetPortInfo
**
** Shared function required by all functions
**
*/
void ftGetPortInfo(unsigned short *portIndex, usbhost_ep_handle_ex *ep)
{
	usbhost_device_handle_ex ifDev;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ioctl_cb_dev_info_t ifInfo;
	unsigned short index;

	ifDev = deviceList[curDevice].ifDev;

	// Get the control endpoint for our operation.
	host_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CONTROL_ENDPOINT_HANDLE;
	host_iocb.handle.dif = ifDev;
	host_iocb.get = ep;

	vos_dev_ioctl(deviceList[curDevice].host, &host_iocb); // Got a handle to our control endpoint 'ep'

	host_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_DEV_INFO;
	host_iocb.get = &ifInfo;

	vos_dev_ioctl(deviceList[curDevice].host, &host_iocb);

	// Address the correct port on the FT device
	index = ifInfo.interface_number + 1;

	if (index == 1)
		index = 0;

	*portIndex = index;
}

unsigned char ftSendSetup(usb_deviceRequest_t *desc_dev, char *buf, usbhost_ep_handle_ex ep)
{
	unsigned char result;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...

	host_iocb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_SETUP_TRANSFER;
	host_iocb.handle.ep = ep;
	host_iocb.set = desc_dev;
	host_iocb.get = buf;

	result = vos_dev_ioctl(deviceList[curDevice].host, &host_iocb);

	if (result == USBHOST_OK)
		result = MON_SUCCESS;

	return result;
}

/*
** cmd_fbd
**
** Set the baud rate of an FT device
**
** Parameters: param1: baud rate (3 bytes)
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fbd(void)
{
	// setup transfer descriptor
	usb_deviceRequest_t desc_dev;
	unsigned char result = 0;
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned short index;

	monReadNumber(1, 3);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 3);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_VENDOR |
			USB_BMREQUESTTYPE_DEVICE;
		desc_dev.bRequest = USBHOST_SETBAUDRATE;

		// Set register values
		desc_dev.wValue = param1Data[2] | (param1Data[1] << 8);
		desc_dev.wIndex = index | (param1Data[0] << 8);
		desc_dev.wLength = 0;          // No Data phase.

		result = ftSendSetup(&desc_dev, NULL, ep);
	}

	return result;
}

/*
** cmd_fmc
**
** Set Modem Ctrl
**
** Parameters: param1: modem control word
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fmc(void)
{
	usb_deviceRequest_t desc_dev;
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned short index;

	monReadNumber(1, 2);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 2);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		// Set the flow control properties of a device.
		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_VENDOR |
			USB_BMREQUESTTYPE_DEVICE;

		desc_dev.bRequest = USBHOST_MODEMCTRL;
		desc_dev.wValue = param1Data[1] | (param1Data[0] << 8);
		desc_dev.wIndex = index;
		desc_dev.wLength = 0;          // No data phase.

		result = ftSendSetup(&desc_dev, NULL, ep);
	}

	return result;
}

/*
** cmd_fsd
**
** Set Data Characteristics
**
** Parameters: param1: data characteristics word
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fsd()
{
	usb_deviceRequest_t desc_dev;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned short index;

	monReadNumber(1, 2);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 2);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_VENDOR |
			USB_BMREQUESTTYPE_DEVICE;

		desc_dev.bRequest = USBHOST_SETDATA;
		desc_dev.wValue = param1Data[1] | (param1Data[0] << 8);
		desc_dev.wIndex = index;
		desc_dev.wLength = 0;          // No data phase.

		result = ftSendSetup(&desc_dev, NULL, ep);
	}

	return result;
}

/*
** cmd_ffc
**
** Set Flow Control
**
** Parameters: param1: flow control byte
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_ffc()
{
	usb_deviceRequest_t desc_dev;

	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned short index;

	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 1);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		// Set the flow control properties of a device.
		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV | // Data transfer direction
			USB_BMREQUESTTYPE_VENDOR |                           // Type
			USB_BMREQUESTTYPE_DEVICE;                            // Recipient
		desc_dev.bRequest = USBHOST_SETFLOWCTRL;
		desc_dev.wValue = (param1Data[0] << 8);
		desc_dev.wIndex = index;
		desc_dev.wLength = 0;          // No data phase.

		result = ftSendSetup(&desc_dev, NULL, ep);
	}

	return result;
}

/*
** cmd_fgm
**
** Get modem status
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Displays modem status word.
*/
unsigned char cmd_fgm()
{
	usb_deviceRequest_t desc_dev;
	char cr = 0x0D;
	unsigned short modemStatus = 0;
	unsigned char result = 0;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned short index;

	// no- parameters purge input until CR
	monReadCr();

	ftGetPortInfo(&index, &ep);

	desc_dev.bmRequestType = USB_BMREQUESTTYPE_DEV_TO_HOST |
		USB_BMREQUESTTYPE_VENDOR |
		USB_BMREQUESTTYPE_DEVICE;
	desc_dev.bRequest = USBHOST_GETMODEMSTAT;
	desc_dev.wValue = 0;
	desc_dev.wIndex = index;
	desc_dev.wLength = 2;              // 2 bytes returned from the call in the data phase.

	result = ftSendSetup(&desc_dev, (char *) &modemStatus, ep);
	monAddNumberToConsole((unsigned char *) &modemStatus, 2);
	monWrite(&cr, 1);

	return result;
}

/*
** cmd_fsl
**
** Set latency timer
**
** Parameters: param1: latency timer setting byte
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fsl(void)
{
	usb_deviceRequest_t desc_dev;
	unsigned short modemStatus = 0;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned short index;

	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 1);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_VENDOR |
			USB_BMREQUESTTYPE_DEVICE;
		desc_dev.bRequest = USBHOST_SETLATTIMER;
		desc_dev.wValue = (param1Data[0] << 8);
		desc_dev.wIndex = index;
		desc_dev.wLength = 0;

		result = ftSendSetup(&desc_dev, (char *) &modemStatus, ep);
	}

	return result;
}

/*
** cmd_fgb
**
** Get Bit Mode
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Displays bit mode byte.
*/
unsigned char cmd_fgb(void)
{
	// Get bit mode, returns a char
	usb_deviceRequest_t desc_dev;
	char cr = 0x0D;
	unsigned char bitMode = 0;
	unsigned char result = 0;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned short index;

	// no- parameters purge input until CR
	monReadCr();

	ftGetPortInfo(&index, &ep);

	desc_dev.bmRequestType = USB_BMREQUESTTYPE_DEV_TO_HOST |
		USB_BMREQUESTTYPE_VENDOR |
		USB_BMREQUESTTYPE_DEVICE;
	desc_dev.bRequest = USBHOST_GETBITMODE;
	desc_dev.wValue = 0;
	desc_dev.wIndex = index;
	desc_dev.wLength = 1;              // Get Bit Mode returns a char...

	result = ftSendSetup(&desc_dev, (char *) &bitMode, ep);
	monAddNumberToConsole(&bitMode, 1);
	monWrite(&cr, 1);

	return result;
}

/*
** cmd_fsb
**
** Set bit mode.
**
** Parameters: param1: bit mode word
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fsb(void)
{
	usb_deviceRequest_t desc_dev;
	unsigned char bitMode = 0;
	usbhost_ioctl_cb_t host_iocb;      // Ioctl block for our request...
	usbhost_ep_handle_ex ep;           // Handle to the control endpoint
	unsigned char result = MON_ERROR_CMD_FAILED;
	unsigned short index;

	monReadNumber(1, 2);
	// purge until CR
	monReadCr();

	result = monValidateNumber(1, 2);

	if (result == MON_SUCCESS)
	{
		ftGetPortInfo(&index, &ep);

		desc_dev.bmRequestType = USB_BMREQUESTTYPE_HOST_TO_DEV |
			USB_BMREQUESTTYPE_VENDOR |
			USB_BMREQUESTTYPE_DEVICE;
		desc_dev.bRequest = USBHOST_SETBITMODE;
		desc_dev.wValue = param1Data[1] | (param1Data[0] << 8);
		desc_dev.wIndex = index;
		desc_dev.wLength = 0;          // No data phase.

		result = ftSendSetup(&desc_dev, NULL, ep);
	}

	return result;
}

