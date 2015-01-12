/*
** cfgslaveft232.h
**
** Copyright © 2011 Future Technology Devices International Limited
**
** Company Confidential
**
** Header file containing definitions for V2DPS FT232 configuration
**
** Author: FTDI
** Project: Vinculum II
** Module: Vinculum II Firmware
** Requires:
** Comments: Edit this file with descriptor values for custom devices.
**
** History:
**  1 – Initial version
**
*/

#ifndef __cfgslaveft232_h__
#define __cfgslaveft232_h__

#include "USB.h"
#include "USBSlave.h"
#include "USBSlaveFT232.h"

/*
 * Device descriptor - change these #defines for custom device.
 */
#define CFGSLAVEFT232_VID				  USB_VID_FTDI
#define CFGSLAVEFT232_PID				  USB_PID_FTDI_FT232
#define CFGSLAVEFT232_SI_MANUFACTURER	  FT232_STRING_INDEX_MANUFACTURER
#define CFGSLAVEFT232_SI_PRODUCT		  FT232_STRING_INDEX_PRODUCT
#define CFGSLAVEFT232_SI_SERIAL_NUMBER	  FT232_STRING_INDEX_SERIAL_NUMBER

/*
 * Configuration descriptor - change these #defines for custom device.
 */
#define CFGSLAVEFT232_CONFIG_BMATTRIBUTES 0x80
#define CFGSLAVEFT232_CONFIG_MAX_POWER	  45

/*
 * Device strings - #define to use strings for custom device (CFGSLAVEFT232.c).
 */
#undef CFGSLAVEFT232_DEVICE_STRINGS

void cfgslaveft232_init(usbslaveft232_ioctl_cb_descriptors_t *cb);

#endif                                 /* __cfgslaveft232_h__ */

