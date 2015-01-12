/*
** cfgmonitor.h
**
** Copyright © 2011 Future Technology Devices International Limited
**
** Company Confidential
**
** Header file containing definitions for V2DPS/V2DAP configuration
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

#ifndef __cfgmonitor_h__
#define __cfgmonitor_h__

#include "uart.h"
#include "monitor.h"

// monitor confguration
#define CFGMONITOR_BAUD			UART_BAUD_9600
#define CFGMONITOR_FLOW_CONTROL UART_FLOW_RTS_CTS
#define CFGMONITOR_DATA_BITS	UART_DATA_BITS_8
#define CFGMONITOR_STOP_BITS	UART_STOP_BITS_1
#define CFGMONITOR_PARITY		UART_PARITY_NONE

// monitor initial mode
// bitmap of either MON_ECS or MON_SCS and either MON_IPH or MON_IPA
#define CFGMONITOR_MON_INIT		(MON_ECS | MON_IPH)

// monitor beahviour
// use #undef to disable
#define CFGMONITOR_LEDS_FLASH
#define CFGMONITOR_DATA_MODE
#define CFGMONITOR_FIRMWARE_UPDATE

// monitor display
// use #undef to disable
#define CFGMONITOR_SHOW_PROMPT
#define CFGMONITOR_SHOW_VERSION
#define CFGMONITOR_SHOW_CONNECTS_REMOVALS

// USB device connection behaviour (for set current)
#undef CFGMONITOR_SC_SET_FOR_INSERTED_DEVICES
#undef CFGMONITOR_SC_RESET_FOR_REMOVED_DEVICES

#endif                                 /* __cfgmonitor_h__ */

