/*
** svcslave.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP USB Slave Services
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
#include "USBSlave.h"
#include "USBSlaveFT232.h"
#include "ioctl.h"

#include "monitor.h"
#include "slave.h"
#include "host.h"

unsigned char slaveConnected(VOS_HANDLE hUsbSlave)
{
	usbslave_ioctl_cb_t usbslave_iocb;
	unsigned char i;

	usbslave_iocb.ioctl_code = VOS_IOCTL_USBSLAVE_GET_STATE;
	usbslave_iocb.get = &i;
	vos_dev_ioctl(hUsbSlave, &usbslave_iocb);

	return i;
}

unsigned char slaveDataRx(VOS_HANDLE hUsbFT232)
{
	common_ioctl_cb_t usbslave_iocb;
	unsigned char data = 0;

	usbslave_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;

	if (vos_dev_ioctl(hUsbFT232, &usbslave_iocb) == USBSLAVEFT232_OK)
	{
		data = usbslave_iocb.get.queue_stat & 0xff;
	}

	return data;
}

unsigned char slaveDataTx(VOS_HANDLE hUsbFT232)
{
	common_ioctl_cb_t usbslave_iocb;
	unsigned char data = 0;

	usbslave_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_TX_QUEUE_STATUS;

	if (vos_dev_ioctl(hUsbFT232, &usbslave_iocb) == USBSLAVEFT232_OK)
	{
		data = usbslave_iocb.get.queue_stat & 0xff;
	}

	return data;
}

unsigned char slaveSelectDevice()
{
	return hostSelectDevice('S');
}

