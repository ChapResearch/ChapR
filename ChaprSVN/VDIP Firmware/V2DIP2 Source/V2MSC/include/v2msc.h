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

#define VMSC_FIRMWARE

#define SIZEOF_FIRMWARE_TASK_MEMORY 0x1200

enum VOS_DEVICES {
	VOS_DEV_MON,
	VOS_DEV_USB2,
	VOS_DEV_BOMS,
	VOS_DEV_SPIM,
	VOS_DEV_MP3,
	NUMBER_OF_DEVICES,
};


#undef __VOS_MESSAGE_PASSING__

#define __VOS_DEVICES__

enum DEVICE_STATES {
	STATE_NONE 	 = 0b00000001, // No device connected.
	STATE_DEVICE = 0b00000010, // A USB device connected.
	STATE_DISK 	 = 0b00000110, // A USB BOMS disk connected.
};
		
// Note: A song can be stopped by playing another song or skipping a track.
// In this situation we do not want to display the "STOPPED" string.  STOP_VST
// indicates that playback has been stopped by the user.
enum PLAY_STATES
{
	PLAYING = 0, 				// Track is playing
	STOP_VST, 					// User generated stop command
	STOPPED,					// Track is stopped playing
	PAUSED,						// User generated pause command
	PAUSED_E,					// Generated from 'e' or 'E'
	STOP_DISK_REMOVED,			// Flash disc has been pulled from the Host
};	

unsigned char cmd_pause(char on);

#endif                                 // __v2dap_h__

