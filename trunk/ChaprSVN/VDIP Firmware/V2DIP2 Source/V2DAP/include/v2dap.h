/*
** v2dap.h
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Header file for Vinculum II V2DAP Configuration and State Machine
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
#ifndef __v2dap_h__
#define __v2dap_h__

#define SIZEOF_FIRMWARE_TASK_MEMORY 0x1200

#define NUMBER_OF_DEVICES			4

#define VOS_DEV_MON					0  // Handle to the monitor, either UART, SPI or FIFO.
#define VOS_DEV_USB1				1
#define VOS_DEV_USB2				2
#define VOS_DEV_BOMS				3

#undef __VOS_MESSAGE_PASSING__

#define __VOS_DEVICES__

#endif                                 // __v2dap_h__

