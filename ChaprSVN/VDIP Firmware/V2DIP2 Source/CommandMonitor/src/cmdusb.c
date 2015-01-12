/*
** cmdusb.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP USB Commands
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
#include "slave.h"
#include "ioctl.h"

typedef unsigned char RESERVED;

// Structure to store data returned from QD.
typedef struct _stQD
{
	unsigned char  USBaddress;
	unsigned char  CtrlEndPoint;
	unsigned char  PipeInNo;
	unsigned char  PipeInSize;
	unsigned char  PipeOutNo;
	unsigned char  PipeOutSize;
	RESERVED	   resv0;
	unsigned char  DeviceType;
	RESERVED	   resv1;
	unsigned char  Location;
	unsigned char  MIindex;
	unsigned char  DeviceClass;
	unsigned char  DeviceSubClass;
	unsigned char  DeviceProtocol;
	unsigned short VID;
	unsigned short PID;
	unsigned short BCD;
	unsigned char  DeviceSpeed;
	RESERVED	   resv2[11];
} stQD;

/*
** cmd_qp1
**
** Displays a bitmask of device types on USB Port 1
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_qp1()
{
#ifndef V2MSC_FIRMWARE
#ifndef V2DPS_FIRMWARE
	unsigned char qp;
	char cr;

	// no- parameters purge input until CR
	monReadCr();

	qp = hostGetPortDevType(hUsb1);
	monAddNumberToConsole(&qp, 1);
	cr = 0x00;
	monAddNumberToConsole((unsigned)&cr, 1);
	cr = 0x0d;
	monWrite(&cr, 1);
#endif // V2DPS_FIRMWARE
#endif // V2MSC_FIRMWARE	
	return MON_SUCCESS;
}

/*
** cmd_qp2
**
** Displays a bitmask of device types on USB Port 2
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_qp2()
{
	unsigned char qp;
	char cr;

	// no- parameters purge input until CR
	monReadCr();

	qp = hostGetPortDevType(hUsb2);
	monAddNumberToConsole(&qp, 1);
	cr = 0x00;
	monAddNumberToConsole((unsigned)&cr, 1);
	cr = 0x0d;
	monWrite(&cr, 1);

	return MON_SUCCESS;
}

/*
** cmd_qd
**
** Shows stored information about a device.
**
** Parameters: param1: device number (0 to 15)
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED offset out of range or no device at offset
** Comments:
*/
unsigned char cmd_qd()
{
	char cr = 0x0d;

	stQD qdStruct;                       // Struture to hold data returned from the device...

	usbhost_ioctl_cb_t host_ioctl_cb;    // IOCTL block for our requests.
	usbhost_ioctl_cb_vid_pid_t cbVIDPID; // VID / PID structure
	usbhost_ioctl_cb_class_t cbClass;
	usbhost_ioctl_cb_dev_info_t ifInfo;
	usbhost_ioctl_cb_ep_info_t epInfo;   // Structure to store our endpoint data.

	usbhost_ep_handle_ex epHandle = 0;       // Handle to our endpoint.
	usbhost_device_handle_ex ifDev;
	VOS_HANDLE hDev;

	unsigned char status = MON_SUCCESS;
	unsigned char *pclr;
	unsigned char i;

	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 1);

	vos_memset(&qdStruct, 0, sizeof(qdStruct));

	if (param1 < 16)
	{
		ifDev = deviceList[param1].ifDev;
		hDev = deviceList[param1].host;

		if (ifDev != NULL)
		{
			// use this host
			host_ioctl_cb.handle.dif = ifDev;
			qdStruct.DeviceType = deviceList[param1].type;

			host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_DEV_INFO;
			host_ioctl_cb.get = &ifInfo;
			if (vos_dev_ioctl(hDev, &host_ioctl_cb) == USBHOST_OK)
			{
				qdStruct.USBaddress = ifInfo.addr;
				qdStruct.MIindex = ifInfo.interface_number;
				qdStruct.DeviceSpeed = ifInfo.speed + 1; // 1 for Full Speed, 2 for Low Speed
				qdStruct.Location = ifInfo.port_number;
			
				/* VID / PID */
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_VID_PID;
				host_ioctl_cb.get = &cbVIDPID;
				if (vos_dev_ioctl(hDev, &host_ioctl_cb) == USBHOST_OK)
				{
					qdStruct.VID = cbVIDPID.vid;
					qdStruct.PID = cbVIDPID.pid;
				}
				
				/* USB Class */
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CLASS_INFO;
				host_ioctl_cb.get = &cbClass;
				if (vos_dev_ioctl(hDev, &host_ioctl_cb) == USBHOST_OK)
				{
					qdStruct.DeviceClass = cbClass.dev_class;
					qdStruct.DeviceSubClass = cbClass.dev_subclass;
					qdStruct.DeviceProtocol = cbClass.dev_protocol;
				}
				/* Endpoints */
				// Control Endpoint
				epHandle = NULL;
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CONTROL_ENDPOINT_HANDLE;
				host_ioctl_cb.handle.dif = ifDev;
				host_ioctl_cb.get = &epHandle;

				vos_dev_ioctl(hDev, &host_ioctl_cb);

				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
				host_ioctl_cb.handle.ep = epHandle;
				host_ioctl_cb.get = &epInfo;

				vos_dev_ioctl(hDev, &host_ioctl_cb);

				qdStruct.CtrlEndPoint = epInfo.max_size;

				// Pipe In Endpoint
				epHandle = NULL;
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_IN_ENDPOINT_HANDLE;
				host_ioctl_cb.handle.dif = ifDev;
				host_ioctl_cb.get = &epHandle;

				vos_dev_ioctl(hDev, &host_ioctl_cb);

				if (!epHandle)             // We have no handle to a bulk endpoint so try an interrupt
				{
					// Acquire Handle to interrupt endpoint
					// epHandle = NULL;
					host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_INT_IN_ENDPOINT_HANDLE;
					// host_ioctl_cb.handle.dif = ifDev;
					// host_ioctl_cb.get = &epHandle;

					vos_dev_ioctl(hDev, &host_ioctl_cb);
				}

				if (epHandle)
				{
					host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
					host_ioctl_cb.handle.ep = epHandle;
					host_ioctl_cb.get = &epInfo;

					vos_dev_ioctl(hDev, &host_ioctl_cb);

					qdStruct.PipeInNo = epInfo.number;
					qdStruct.PipeInSize = epInfo.max_size;
				}

				// Aquire the handle for the Out endpoint.
				epHandle = NULL;

				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_OUT_ENDPOINT_HANDLE;
				host_ioctl_cb.handle.dif = ifDev;
				host_ioctl_cb.get = &epHandle;

				vos_dev_ioctl(hDev, &host_ioctl_cb);

				if (epHandle)
				{
					// Out Endpoint info.
					host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
					host_ioctl_cb.handle.ep = epHandle;
					host_ioctl_cb.get = &epInfo;

					vos_dev_ioctl(hDev, &host_ioctl_cb);

					qdStruct.PipeOutNo = epInfo.number;
					qdStruct.PipeOutSize = epInfo.max_size;
				}
			}
		}
		else
		{
			status = MON_ERROR_CMD_FAILED;
		}
	}
	else if ((param1 == 'S') || (param1 == 's'))
	{
		// send 32 zeros to monitor (qdStruct is cleared)
	}
	else
	{
		status = MON_ERROR_CMD_FAILED;
	}

	monAddNumberToConsole((unsigned char *) &qdStruct, 32);
	monWrite(&cr, 1);

	return status;
}

/*
** cmd_sc
**
** Sets the current device to use.
**
** Parameters: param1: Current device (0 to 15)
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED is no device at offset
** Comments:
*/
unsigned char cmd_sc()
{
	unsigned char status;

	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 1);

	if ((param1 == 'S') || (param1 == 's'))
	{
		status = slaveSelectDevice();
	}
	else
	{
		// do not set current to a hub device
		if (deviceList[param1].type & hostHasHub)
		{
			status = MON_ERROR_BAD_CMD;
		}
		else
		{
			status = hostSelectDevice((unsigned char) param1);
		}
	}

	return status;
}

/*
** cmd_sf
**
** Sets a device to be FTDI compatible.
**
** Parameters: none
** Parameters: param1: Device to set to FTDI device (0 to 15)
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED is no device at offset
** Comments:
*/
unsigned char cmd_sf()
{
	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 1);

	return hostSetDeviceFtdi((unsigned char) param1);
}

/*
** cmd_dsd
**
** Send data to a USB device bulk endpoint.
**
** Parameters: param1: number of bytes to send
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if no bulk out endpoint
** Comments: Uses current device (SC command).
**  Maximum 64 bytes to be sent in one operation.
*/
unsigned char cmd_dsd()
{
	usbhost_xfer_t xfer;
	unsigned char i;
	vos_semaphore_t s;
	usbhost_ep_handle_ex epHandle = 0;     // Handle to our endpoint.
	usbhost_device_handle_ex ifDev;
	VOS_HANDLE hDev;
	usbhost_ioctl_cb_t host_ioctl_cb;  // IOCTL block for our requests.
	unsigned char status = MON_ERROR_CMD_FAILED;
	// maximum allowed data
	unsigned char buf[64];
	unsigned char bulkOutEp = USBHOST_XFER_FLAG_START_BULK_ENDPOINT_LIST;

	monReadNumber(1, 1);
	// purge until CR
	monReadCr();

	monValidateNumber(1, 1);

	// read in data from the monitor
	monRead((unsigned char *) buf, (unsigned short) param1);

	if (curDevice < 16)
	{
		ifDev = deviceList[curDevice].ifDev;
		hDev = deviceList[curDevice].host;

		if (ifDev)
		{
			host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_OUT_ENDPOINT_HANDLE;
			host_ioctl_cb.handle.dif = ifDev;
			host_ioctl_cb.get = &epHandle;
			vos_dev_ioctl(hDev, &host_ioctl_cb);

			if (!epHandle)             // We have no handle to a bulk endpoint so try an interrupt
			{
				// Acquire Handle to interrupt endpoint
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_INT_OUT_ENDPOINT_HANDLE;
				vos_dev_ioctl(hDev, &host_ioctl_cb);
				bulkOutEp = 0;
			}

			if (epHandle)
			{
				vos_memset(&xfer, 0, sizeof(usbhost_xfer_t));
				vos_init_semaphore(&s, 0);

				xfer.buf = buf;
				xfer.len = param1;
				xfer.ep = epHandle;
				xfer.s = &s;
				xfer.cond_code = USBHOST_CC_NOTACCESSED;
				xfer.flags = bulkOutEp;
				status = vos_dev_write(hDev, (unsigned char *) &xfer, sizeof(usbhost_xfer_t), NULL);

				if (status == USBHOST_OK)
				{
					status = MON_SUCCESS;
				}
			}
		}
	}
	else
	{
		vos_dev_write(hFT232, buf, (unsigned short) param1, NULL);
		status = MON_SUCCESS;
	}

	return status;
}

/*
** cmd_drd
**
** Read data from a USB device bulk endpoint.
**
** Parameters: param1: number of bytes to read
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if no bulk in endpoint
** Comments: Uses current device (SC command).
**  Maximum 64 bytes to be received in one operation.
**  Size of received data, followed by data dumped to monitor.
*/
unsigned char cmd_drd()
{
	usbhost_xfer_t xfer;
	unsigned char i;
	vos_semaphore_t s;
	usbhost_ep_handle_ex epHandle = 0;     // Handle to our endpoint.
	usbhost_device_handle_ex ifDev;
	VOS_HANDLE hDev;
	usbhost_ioctl_cb_t host_ioctl_cb;  // IOCTL block for our requests.
	usbhost_ioctl_cb_ep_info_t epInfo; // Structure to store our endpoint data.
	common_ioctl_cb_t uart_iocb;       // UART iocb for getting bytes available.
	unsigned char status = MON_ERROR_CMD_FAILED;
	// maximum allowed data
	unsigned char buf[64];
	unsigned char *pbuf;
	char cr = 0x0d;
	unsigned char bulkInEp = USBHOST_XFER_FLAG_START_BULK_ENDPOINT_LIST;

	// purge until CR
	monReadCr();

	xfer.len = 0;

	if (curDevice < 16)
	{
		ifDev = deviceList[curDevice].ifDev;
		hDev = deviceList[curDevice].host;

		if (ifDev)
		{
			// Pipe In Endpoint
			host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_IN_ENDPOINT_HANDLE;
			host_ioctl_cb.handle.dif = ifDev;
			host_ioctl_cb.get = &epHandle;
			vos_dev_ioctl(hDev, &host_ioctl_cb);

			if (!epHandle)             // We have no handle to a bulk endpoint so try an interrupt
			{
				// Acquire Handle to interrupt endpoint
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_INT_IN_ENDPOINT_HANDLE;
				vos_dev_ioctl(hDev, &host_ioctl_cb);
				bulkInEp = 0;
			}

			if (epHandle)
			{
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
				host_ioctl_cb.handle.ep = epHandle;
				host_ioctl_cb.get = &epInfo;

				vos_dev_ioctl(hDev, &host_ioctl_cb);

				vos_memset(&xfer, 0, sizeof(usbhost_xfer_t));
				vos_init_semaphore(&s, 0);

				xfer.buf = buf;
				xfer.len = epInfo.max_size;
				xfer.ep = epHandle;
				xfer.s = &s;
				xfer.cond_code = USBHOST_CC_NOTACCESSED;
				xfer.flags = bulkInEp | USBHOST_XFER_FLAG_ROUNDING | USBHOST_XFER_FLAG_NONBLOCKING;
				status = vos_dev_read(hDev, (unsigned char *) &xfer, sizeof(usbhost_xfer_t), NULL);

				if (status == USBHOST_OK)
				{
					host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_CLEAR_ENDPOINT_TRANSFER;
					host_ioctl_cb.handle.ep = epHandle;

					status = MON_SUCCESS;

					// allow a timeout for the read operation
					// VNC1L would timeout after one NAK but does not manage interrupt endpoints
					// VNC2 manages interrupt endpoints so we have to try and force a timeout to cover
					//  at least one polling interval
					for (i = 0; i < 250; i++)
					{
						vos_delay_msecs(1);

						if (xfer.cond_code != USBHOST_CC_NOTACCESSED)
						{
							break;
						}
					}

					if (xfer.cond_code == USBHOST_CC_NOTACCESSED)
					{
						// no response so remove transfer request
						vos_dev_ioctl(hDev, &host_ioctl_cb);
						xfer.len = 0;
						status = MON_ERROR_CMD_FAILED;
					}

					vos_wait_semaphore(&s);
				}
				else
				{
					xfer.len = 0;
					status = MON_ERROR_CMD_FAILED;
				}
			}

			pbuf = buf;

			if (xfer.len)
			{
				if (deviceList[curDevice].type == hostHasFTDI)
				{
					xfer.len -= 2;
					pbuf = &buf[2];
				}
			}
		}
	}
	else
	{
		uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
		vos_dev_ioctl(hFT232, &uart_iocb);

		if (uart_iocb.get.queue_stat)
		{
			vos_dev_read(hFT232, buf, uart_iocb.get.queue_stat, NULL);

			xfer.len = uart_iocb.get.queue_stat;
			pbuf = buf;
			status = MON_SUCCESS;
		}
	}

	monAddNumberToConsole(&xfer.len, 1);
	monWrite(&cr, 1);
	monWrite((char *) pbuf, xfer.len);

	return status;
}

/*
** cmd_ssu
**
** Send Setup command to a USB device.
**
** Parameters: param1: 8 byte SETUP command
** Returns: MON_SUCCESS
** Comments: Uses current device (SC command).
**  If it is Host to Device then length of setup command used to send data from monitor.
**  If it is Device to Host then length of setup command used to receive data from device.
*/
unsigned char cmd_ssu()
{
	char cr = 0x0d;

	usbhost_xfer_t xfer;
	usbhost_ep_handle_ex epHandle = 0;     // Handle to our endpoint.
	usbhost_ioctl_cb_t host_ioctl_cb;  // IOCTL block for our requests.
	usb_deviceRequest_t *desc_dev;
	usbhost_device_handle_ex ifDev;
	VOS_HANDLE hDev;
	unsigned short len;
	unsigned char status = MON_ERROR_CMD_FAILED;
	unsigned char i;
	// maximum allowed data
	unsigned char buf[128];

	monReadNumber(1, 8);
	// purge until CR
	monReadCr();

	for (i = 0; i < 8; i++)
	{
		param2Data[8 - 1 - i] = param1Data[i];
	}

	desc_dev = param2Data;
	len = desc_dev->wLength & 0x7f;

	if ((desc_dev->bmRequestType & 0x80) == USB_BMREQUESTTYPE_HOST_TO_DEV)
	{
		// read in data from the monitor
		monRead((char *) buf, len);
	}

	// Aquire the handle for the Out endpoint.
	epHandle = NULL;

	if (curDevice < 16)
	{
		ifDev = deviceList[curDevice].ifDev;
		hDev = deviceList[curDevice].host;

		if (ifDev)
		{
			// Control Endpoint
			epHandle = NULL;
			host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_CONTROL_ENDPOINT_HANDLE;
			host_ioctl_cb.handle.dif = ifDev;
			host_ioctl_cb.get = &epHandle;

			vos_dev_ioctl(hDev, &host_ioctl_cb);

			if (epHandle)
			{
				host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_SETUP_TRANSFER;
				host_ioctl_cb.handle.ep = epHandle;
				host_ioctl_cb.set = param2Data;
				host_ioctl_cb.get = buf;

				if (vos_dev_ioctl(hDev, &host_ioctl_cb) == USBHOST_OK)
				{
					status = MON_SUCCESS;
				}
			}

			if ((desc_dev->bmRequestType & 0x80) == USB_BMREQUESTTYPE_DEV_TO_HOST)
			{
				if (len)
				{
					monWrite((char *) buf, len);
				}
			}
		}
	}

	return status;
}

