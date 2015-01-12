/*
** host.h
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Header file for Vinculum II V2DAP USB Host Services
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

#ifndef __host_h__
#define __host_h__

#define DEV_LIST_SIZE 16

typedef struct _usbDevList
{
	usbhost_device_handle_ex ifDev;
	VOS_HANDLE				 host;
	unsigned char			 type;
} usbDevList;

extern unsigned char curDevice;
extern usbDevList deviceList[DEV_LIST_SIZE];

unsigned char hostConnected(VOS_HANDLE hUsbHost);
unsigned char hostAddToDeviceList(VOS_HANDLE hUsbHost);
unsigned char hostRemoveFromDeviceList(VOS_HANDLE hUsbHost);
unsigned char hostGetDevType(VOS_HANDLE hUsbHost, usbhost_device_handle_ex ifDev);
unsigned char hostGetPortDevType(VOS_HANDLE hUsbHost);
unsigned char hostSetDeviceFtdi(unsigned char device);
unsigned char hostSelectDevice(unsigned char device);
unsigned char hostSuspend(VOS_HANDLE hUSB);
unsigned char hostWake(VOS_HANDLE hUSB);

#define hostHasFTDI		 0x01          // 00000001b
#define hostHasPrnClass	 0x04          // 00000100b
#define hostHasHIDClass	 0x08          // 00001000b
#define hostHasCDCClass	 0x10          // 00010000b
#define hostHasBOMSClass 0x20          // 00100000b
#define hostHasUnknown	 0x40          // 01000000b
#define hostHasHub		 0x80          // 10000000b

#endif                                 // __host_h__

