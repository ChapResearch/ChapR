/*
** cfgslaveft232.c
**
** Copyright © 2011 Future Technology Devices International Limited
**
** Company Confidential
**
** Source file for V2DPS FT232 configuration
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

#include "vos.h"
#include "USBSlave.h"
#include "USBSlaveFT232.h"
#include "cfgslaveft232.h"

#ifdef CFGSLAVEFT232_DEVICE_STRINGS

/*
 * Device strings - change these declarations for custom device.
 */
uint8 cfgslaveft232_str0_descriptor[4] = {
	0x04, 0x03, 0x09, 0x04
};

uint8 cfgslaveft232_str1_descriptor[10] = {
	0x0a, 0x03, 0x46, 0x00, 0x54, 0x00, 0x44, 0x00, 0x49, 0x00
};

uint8 cfgslaveft232_str2_descriptor[40] = {
	0x28, 0x03, 0x56, 0x00, 0x4E, 0x00, 0x43, 0x00, 0x32, 0x00,
	0x20, 0x00, 0x55, 0x00, 0x53, 0x00, 0x42, 0x00, 0x20, 0x00,
	0x3c, 0x00, 0x2d, 0x00, 0x3e, 0x00, 0x20, 0x00, 0x53, 0x00,
	0x65, 0x00, 0x72, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6c, 0x00
};

uint8 cfgslaveft232_str3_descriptor[18] = {
	0x12, 0x03, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00,
	0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00
};

#endif

void cfgslaveft232_init(usbslaveft232_ioctl_cb_descriptors_t *cb)
{
	cb->device_descriptor.idVendor = CFGSLAVEFT232_VID;
	cb->device_descriptor.idProduct = CFGSLAVEFT232_PID;
	cb->device_descriptor.iManufacturer = CFGSLAVEFT232_SI_MANUFACTURER;
	cb->device_descriptor.iProduct = CFGSLAVEFT232_SI_PRODUCT;
	cb->device_descriptor.iSerialNumber = CFGSLAVEFT232_SI_SERIAL_NUMBER;
	cb->device_descriptor.use = 1;

	cb->config_descriptor.bmAttributes = CFGSLAVEFT232_CONFIG_BMATTRIBUTES;
	cb->config_descriptor.bMaxPower = CFGSLAVEFT232_CONFIG_MAX_POWER;
	cb->config_descriptor.use = 1;

#ifdef CFGSLAVEFT232_DEVICE_STRINGS
	cb->zero_string = (usb_deviceStringDescriptorZero_t *) cfgslaveft232_str0_descriptor;
	cb->manufacturer_string = (usb_deviceStringDescriptor_t *) cfgslaveft232_str1_descriptor;
	cb->product_string = (usb_deviceStringDescriptor_t *) cfgslaveft232_str2_descriptor;
	cb->serial_number_string = (usb_deviceStringDescriptor_t *) cfgslaveft232_str3_descriptor;
#else
	cb->zero_string = (usb_deviceStringDescriptorZero_t *) 0;
	cb->manufacturer_string = (usb_deviceStringDescriptor_t *) 0;
	cb->product_string = (usb_deviceStringDescriptor_t *) 0;
	cb->serial_number_string = (usb_deviceStringDescriptor_t *) 0;
#endif
}

