/*
** v2dps.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Configuration and State Machine
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
**  2 - Fix for Data Mode
*/

#include "vos.h"

#include "stdlib.h"

#include "UART.h"
#include "FIFO.h"
#include "SPISlave.h"
#include "USBSlave.h"
#include "USBSlaveFT232.h"
#include "cfgslaveft232.h"
#include "USBHost.h"
#include "USB.h"
#include "msi.h"
#include "BOMS.h"
#include "FAT.h"

#include "host.h"
#include "slave.h"
#include "v2dps.h"
#include "monitor.h"
#include "cfgmonitor.h"
#include "disk.h"

unsigned char monInterface;            // Interface to the monitor.
#define UART 0
#define SPI	 1
#define FIFO 2

VOS_HANDLE      hMonitor;
VOS_HANDLE      hBoms;
VOS_HANDLE      hUsb1;
VOS_HANDLE      hUsb2;
VOS_HANDLE      hFT232;

// driver context structure
fat_context     fatContext;

// thread control block for the monitor
vos_tcb_t       *tcbMonitor;

// forward declarations
void            firmware(void);
unsigned char   checkDataMode(void);
void            ackDataMode(void);
void            ackCmdMode(void);
void            msgOnline(void);

void monDataModeHost();
void monDataModeSlave();

/*
** main
**
** Entry point for application
**
** Parameters: none
** Returns: none
**    never returns
** Requires: VOS kernel
** Comments: Intialise firmware before calling scheduler
*/
void main(void)
{
	common_ioctl_cb_t monitor_iocb;
	fifo_context_t fifo_ctx;
	spislave_context_t spislave_ctx;
	uart_context_t    uart_ctx;
	usbhost_context_t usb_ctx;
	unsigned char     dataBuf;
	unsigned short    numRead;
	unsigned char     monInt;

	// coinfigure UART context structures
	uart_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	fifo_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	spislave_ctx.slavenumber = SPI_SLAVE_0;
	spislave_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;

	// call VOS initialisation routines
	vos_init(10, VOS_TICK_INTERVAL, NUMBER_OF_DEVICES);
	vos_set_clock_frequency(VOS_48MHZ_CLOCK_FREQUENCY);

	// initialise USB Slave device driver
	usbslave_init(USBSLAVE_PORT_A, VOS_DEV_USB1);
	usbslaveft232_init(VOS_DEV_FT232, NULL);

	// initialise USB Host device driver
	usb_ctx.if_count       = 16;
	usb_ctx.ep_count       = 32;
	usb_ctx.xfer_count     = 2;
	usb_ctx.iso_xfer_count = 1;
	usbhost_init(-1, VOS_DEV_USB2, &usb_ctx);

	// initialise BOMS device driver
	boms_init(VOS_DEV_BOMS);

	/* Monitor Interface Selection
	   Pin 47       Pin 46       Monitor Mode
	   Pull-Up      Pull-Up      UART
	   Pull-Up      Pull-Down    SPI
	   Pull-Down    Pull-Up      FIFO
	   Pull-Down    Pull-Down    UART
	 */

	// determine what interface the monitor is going to be using
	// selection pins are only available on 48 pins
	if (vos_get_package_type() == VINCULUM_II_48_PIN)
	{
		// ACBUS6 pin 46
		vos_iomux_define_input(46, IOMUX_IN_GPIO_PORT_D_5);
		// ACBUS6 pin 47
		vos_iomux_define_input(47, IOMUX_IN_GPIO_PORT_D_6);

		dataBuf = 0;
		vos_gpio_read_port(GPIO_PORT_D, &dataBuf); // Read the data on the GPIO pins.
	}
	else
	{
		// default interface is UART for 64 pin package
		dataBuf = 0;
	}

	// We are only interested in the two pins (46 and 47) so mask the rest off.
	if (((dataBuf & 0x60) == 0x60) || ((dataBuf & 0x60) == 0))
	{
		// UART
		uart_init(VOS_DEV_MON, &uart_ctx);
		monInterface = UART;
	}
	else
	{
		if (dataBuf & 0x20)
		{
			// FIFO
			fifo_init(VOS_DEV_MON, &fifo_ctx);
			monInterface = FIFO;
		}
		else
		{
			// SPI
			spislave_init(VOS_DEV_MON, &spislave_ctx);
			monInterface = SPI;
		}
	}

	// configure the IOMUX depending on the monitor interface selected

	/* GPIO Configuration
	   VNC1L Port  VNC2 GPIO Port
	    AD          C
	    AC          D
	    BD          A
	    BC          B
	 */
	if (vos_get_package_type() == VINCULUM_II_48_PIN)
	{
		// BD0 -> Pin 11
		vos_iomux_define_bidi(11, IOMUX_IN_GPIO_PORT_A_0, IOMUX_OUT_GPIO_PORT_A_0);
		// BD1 -> Pin 12
		vos_iomux_define_bidi(12, IOMUX_IN_GPIO_PORT_A_1, IOMUX_OUT_GPIO_PORT_A_1);
		// BD2 -> Pin 13
		vos_iomux_define_bidi(13, IOMUX_IN_GPIO_PORT_A_2, IOMUX_OUT_GPIO_PORT_A_2);
		// BD3 -> Pin 14
		vos_iomux_define_bidi(14, IOMUX_IN_GPIO_PORT_A_3, IOMUX_OUT_GPIO_PORT_A_3);
		// BD4 -> Pin 15
		vos_iomux_define_bidi(15, IOMUX_IN_GPIO_PORT_A_4, IOMUX_OUT_GPIO_PORT_A_4);
		// BD5 -> Pin 16
		vos_iomux_define_bidi(16, IOMUX_IN_GPIO_PORT_A_5, IOMUX_OUT_GPIO_PORT_A_5);
		// BD6 -> Pin 18
		vos_iomux_define_bidi(18, IOMUX_IN_GPIO_PORT_A_6, IOMUX_OUT_GPIO_PORT_A_6);
		// BD7 -> Pin 19
		vos_iomux_define_bidi(19, IOMUX_IN_GPIO_PORT_A_7, IOMUX_OUT_GPIO_PORT_A_7);

		// BC0 -> Pin 20
		vos_iomux_define_bidi(20, IOMUX_IN_GPIO_PORT_B_0, IOMUX_OUT_GPIO_PORT_B_0);
		// BC1 -> Pin 21
		vos_iomux_define_bidi(21, IOMUX_IN_GPIO_PORT_B_1, IOMUX_OUT_GPIO_PORT_B_1);
		// BC2 -> Pin 22
		vos_iomux_define_bidi(22, IOMUX_IN_GPIO_PORT_B_2, IOMUX_OUT_GPIO_PORT_B_2);
		// BC3 -> Pin 23
		vos_iomux_define_bidi(23, IOMUX_IN_GPIO_PORT_B_3, IOMUX_OUT_GPIO_PORT_B_3);

		if (monInterface == UART)
		{
			// TXD -> Pin 31
			// RXD -> Pin 32
			// RTS# -> Pin 33
			// CTS# -> Pin 34
			// DTR# (DATAACK#) -> Pin 35
			// AD4 -> Pin 35
			vos_iomux_define_output(35, IOMUX_OUT_GPIO_PORT_C_4);
			// DSR# (DATAREQ#) -> Pin 36
			// AD5 -> Pin 36
			vos_iomux_define_input(36, IOMUX_IN_GPIO_PORT_C_5);
			// DCD# -> Pin 37
			// RI# -> Pin 38
			// TXDEN# -> Pin 41

			// AC1 -> Pin 42
			vos_iomux_define_bidi(42, IOMUX_IN_GPIO_PORT_D_1, IOMUX_OUT_GPIO_PORT_D_1);
			// AC2 -> Pin 43
			vos_iomux_define_bidi(43, IOMUX_IN_GPIO_PORT_D_2, IOMUX_OUT_GPIO_PORT_D_2);
			// AC3 -> Pin 44
			vos_iomux_define_bidi(44, IOMUX_IN_GPIO_PORT_D_3, IOMUX_OUT_GPIO_PORT_D_3);
			// AC4 -> Pin 45
			vos_iomux_define_bidi(45, IOMUX_IN_GPIO_PORT_D_4, IOMUX_OUT_GPIO_PORT_D_4);
			// AC5 -> Pin 46
			vos_iomux_define_bidi(46, IOMUX_IN_GPIO_PORT_D_5, IOMUX_OUT_GPIO_PORT_D_5);
		}

		if (monInterface == SPI)
		{
			// SCLK -> Pin 31
			vos_iomux_define_input(31, IOMUX_IN_SPI_SLAVE_0_CLK);
			// SDI -> Pin 32
			vos_iomux_define_input(32, IOMUX_IN_SPI_SLAVE_0_MOSI);
			// SD0 -> Pin 33
			vos_iomux_define_output(33, IOMUX_OUT_SPI_SLAVE_0_MISO);
			// CS -> Pin 34
			vos_iomux_define_input(34, IOMUX_IN_SPI_SLAVE_0_CS);

			// AD5 -> Pin 36
			vos_iomux_define_bidi(36, IOMUX_IN_GPIO_PORT_C_5, IOMUX_OUT_GPIO_PORT_C_5);
			// AD6 -> Pin 37
			vos_iomux_define_bidi(37, IOMUX_IN_GPIO_PORT_C_6, IOMUX_OUT_GPIO_PORT_C_6);
			// AD7 -> Pin 38
			vos_iomux_define_bidi(38, IOMUX_IN_GPIO_PORT_C_7, IOMUX_OUT_GPIO_PORT_C_7);

			// AC0 -> Pin 41
			vos_iomux_define_bidi(41, IOMUX_IN_GPIO_PORT_D_0, IOMUX_OUT_GPIO_PORT_D_0);
			// AC1 -> Pin 42
			vos_iomux_define_bidi(42, IOMUX_IN_GPIO_PORT_D_1, IOMUX_OUT_GPIO_PORT_D_1);
			// AC2 -> Pin 43
			vos_iomux_define_bidi(43, IOMUX_IN_GPIO_PORT_D_2, IOMUX_OUT_GPIO_PORT_D_2);
			// AC3 -> Pin 44
			vos_iomux_define_bidi(44, IOMUX_IN_GPIO_PORT_D_3, IOMUX_OUT_GPIO_PORT_D_3);

			// DATAREQ# -> Pin 45
			vos_iomux_define_input(45, IOMUX_IN_GPIO_PORT_D_4);
			// DATAACK# -> Pin 46
			vos_iomux_define_output(46, IOMUX_OUT_GPIO_PORT_D_5);
		}

		if (monInterface == FIFO)      // Route the FIFO
		{   // D0 41
			vos_iomux_define_bidi(31, IOMUX_IN_FIFO_DATA_0, IOMUX_OUT_FIFO_DATA_0);
			// D0 41
			vos_iomux_define_bidi(32, IOMUX_IN_FIFO_DATA_1, IOMUX_OUT_FIFO_DATA_1);
			// D0 41
			vos_iomux_define_bidi(33, IOMUX_IN_FIFO_DATA_2, IOMUX_OUT_FIFO_DATA_2);
			// D0 41
			vos_iomux_define_bidi(34, IOMUX_IN_FIFO_DATA_3, IOMUX_OUT_FIFO_DATA_3);
			// D0 41
			vos_iomux_define_bidi(35, IOMUX_IN_FIFO_DATA_4, IOMUX_OUT_FIFO_DATA_4);
			// D0 41
			vos_iomux_define_bidi(36, IOMUX_IN_FIFO_DATA_5, IOMUX_OUT_FIFO_DATA_5);
			// D0 41
			vos_iomux_define_bidi(37, IOMUX_IN_FIFO_DATA_6, IOMUX_OUT_FIFO_DATA_6);
			// D0 41
			vos_iomux_define_bidi(38, IOMUX_IN_FIFO_DATA_7, IOMUX_OUT_FIFO_DATA_7);

			// RXF# -> Pin 41
			vos_iomux_define_output(41, IOMUX_OUT_FIFO_RXF_N);
			// TXE# -> Pin 42
			vos_iomux_define_output(42, IOMUX_OUT_FIFO_TXE_N);
			// RD# -> Pin 43
			vos_iomux_define_input(43, IOMUX_IN_FIFO_RD_N);
			// WR -> Pin 44
			vos_iomux_define_input(44, IOMUX_IN_FIFO_WR_N);

			// DATAREQ# -> Pin 45
			vos_iomux_define_input(45, IOMUX_IN_GPIO_PORT_D_4);
			// DATAACK# -> Pin 46
			vos_iomux_define_output(46, IOMUX_OUT_GPIO_PORT_D_5);
		}

		// ACBUS6
		vos_iomux_define_bidi(47, IOMUX_IN_GPIO_PORT_D_6, IOMUX_OUT_GPIO_PORT_D_6);
		// ACBUS7
		vos_iomux_define_bidi(48, IOMUX_IN_GPIO_PORT_D_7, IOMUX_OUT_GPIO_PORT_D_7);
	}

	// setup for 64 pin package is to only allow UART interface
	if (vos_get_package_type() == VINCULUM_II_64_PIN)
	{
		// BD1 -> Pin 12 -> V2EVAL Board LED 3
		vos_iomux_define_output(12, IOMUX_OUT_GPIO_PORT_A_1);
		// BD2 -> Pin 13 -> V2EVAL Board LED 4
		vos_iomux_define_output(13, IOMUX_OUT_GPIO_PORT_A_2);
		// BD5 -> Pin 29 -> V2EVAL Board LED 5
		vos_iomux_define_output(29, IOMUX_OUT_GPIO_PORT_A_5);
		// BD6 -> Pin 31 -> V2EVAL Board LED 6
		vos_iomux_define_output(31, IOMUX_OUT_GPIO_PORT_A_6);

		// UART Defaults
		// TXD -> Pin 39
		vos_iomux_define_output(39, IOMUX_OUT_UART_TXD);
		// RXD -> Pin 40
		vos_iomux_define_input(40, IOMUX_IN_UART_RXD);
		// RTS# -> Pin 41
		vos_iomux_define_output(41, IOMUX_OUT_UART_RTS_N);
		// CTS# -> Pin 42
		vos_iomux_define_input(42, IOMUX_IN_UART_CTS_N);
		// DTR# (DATAACK#) -> Pin 43
		vos_iomux_define_output(43, IOMUX_OUT_GPIO_PORT_C_4);
		// DSR# (DATAREQ#) -> Pin 44
		vos_iomux_define_input(44, IOMUX_IN_GPIO_PORT_C_5);
		// DCD# -> Pin 45
		// RI# -> Pin 46
		// TXDEN# -> Pin 47
	}

	if (vos_get_package_type() == VINCULUM_II_32_PIN)
	{
		// UART Defaults
		// TXD -> Pin 23
		vos_iomux_define_output(23, IOMUX_OUT_UART_TXD);
		// RXD -> Pin 24
		vos_iomux_define_input(24, IOMUX_IN_UART_RXD);
		// RTS# -> Pin 25
		vos_iomux_define_output(25, IOMUX_OUT_UART_RTS_N);
		// CTS# -> Pin 26
		vos_iomux_define_input(26, IOMUX_IN_UART_CTS_N);
#if 1
		// Use pins 29 and 30 as GPIO on 32 pin modules
		// BC0 -> Pin 29
		vos_iomux_define_output(29, IOMUX_OUT_GPIO_PORT_B_0);
		// BC1 -> Pin 30
		vos_iomux_define_input(30, IOMUX_IN_GPIO_PORT_B_1);
#else // 1
		// Use pins 29 and 30 as DTR/DSR for DATAMODE selection on 32 pin modules
		// DTR# (DATAACK#) -> Pin 29
		vos_iomux_define_output(29, IOMUX_OUT_GPIO_PORT_C_4);
		// DSR# (DATAREQ#) -> Pin 30
		vos_iomux_define_input(30, IOMUX_IN_GPIO_PORT_C_5);
#endif // 1
	}

	// create threads for firmware application
	tcbMonitor    = vos_create_thread_ex(20, 1200, firmware, "Application", 0);

	// start VOS scheduler
	vos_start_scheduler();

main_loop:
	goto main_loop;
}

/*
** firmware
**
** State machine and control for V2DAP application
**
** Parameters: none
** Returns: none
**    never returns
** Requires: VOS kernel
** Comments: State machine progresses depending on devices connected and
**    removed from USB ports. Command monitor is called whenever the state
**    machine does not change.
*/
void firmware(void)
{
	// UART IOCTL request block
	common_ioctl_cb_t monitor_iocb;
	// USBSlave IOCTL request block
	common_ioctl_cb_t slave_iocb;
	// USBSlave descriptors request block
	usbslaveft232_ioctl_cb_descriptors_t slave_desc;

	// general purpose variables
	unsigned char     status;
	unsigned char     count;

	// current device type bitmap for USB ports
	unsigned char     host2DevType = 0;

	// state machine progression
	enum state_e {
		STATE_NONE_NONE,
		STATE_NONE_DEVICE,
		STATE_NONE_DISK,
		STATE_SLAVE_NONE,
		STATE_SLAVE_DEVICE,
		STATE_SLAVE_DISK,
	} curState, nextState;

	curState  = STATE_NONE_NONE;
	nextState = curState;

	// initialise monitor
	monInit(CFGMONITOR_MON_INIT);

	vos_gpio_set_port_mode(GPIO_PORT_A, 0xff); // set all as output
	vos_gpio_set_port_mode(GPIO_PORT_B, 0x00); // set all as input

	if (monInterface == UART)
	{
		// set DATAACK as output DATAREQ as input
		vos_gpio_set_port_mode(GPIO_PORT_C, 0x10); 
	}
	else
	{
		vos_gpio_set_port_mode(GPIO_PORT_C, 0x00); 
	}

	if (monInterface == UART)
	{
		vos_gpio_set_port_mode(GPIO_PORT_D, 0x00); 
	}
	else
	{
		// set DATAACK as output DATAREQ as input
		vos_gpio_set_port_mode(GPIO_PORT_D, 0x20); 
	}

	/* start with data mode DATAACK# not asserted */
	ackCmdMode();

	/* set LEDs to all off */
	monLedsOff();

	/* find and open USB drivers */

	hUsb2 = vos_dev_open(VOS_DEV_USB2);

	//vos_enable_interrupts(VOS_USB_0_DEV_INT_IEN);

	hMonitor = vos_dev_open(VOS_DEV_MON);

	/* UART setup */
	/* set baud rate to 9600 baud */
	monitor_iocb.ioctl_code         = VOS_IOCTL_UART_SET_BAUD_RATE;
	monitor_iocb.set.uart_baud_rate = CFGMONITOR_BAUD;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set flow control */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_FLOW_CONTROL;
	monitor_iocb.set.param  = CFGMONITOR_FLOW_CONTROL;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set data bits */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_DATA_BITS;
	monitor_iocb.set.param  = CFGMONITOR_DATA_BITS;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set stop bits */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_STOP_BITS;
	monitor_iocb.set.param  = CFGMONITOR_STOP_BITS;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set parity */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_PARITY;
	monitor_iocb.set.param  = CFGMONITOR_PARITY;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	// set mode for FIFO
	monitor_iocb.ioctl_code = VOS_IOCTL_FIFO_SET_MODE;
	monitor_iocb.set.param = FIFO_MODE_ASYNCHRONOUS;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	// set mode for SPI
	monitor_iocb.ioctl_code = VOS_IOCTL_SPI_SLAVE_SET_MODE;
	monitor_iocb.set.param = SPI_SLAVE_MODE_VI_COMPATIBLE;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	// Enable DMA for monitor
	monitor_iocb.ioctl_code = VOS_IOCTL_COMMON_ENABLE_DMA;
	monitor_iocb.set.param = DMA_ACQUIRE_AS_REQUIRED;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	do
	{
#ifdef CFGMONITOR_LEDS_FLASH
		// loop on flashing LEDS
		monLedsGreen();
		vos_delay_msecs(250);
		monLedsRed();
		vos_delay_msecs(250);
		monLedsGreen();
		vos_delay_msecs(250);
		monLedsRed();
		vos_delay_msecs(250);
		monLedsGreen();
		vos_delay_msecs(250);
		monLedsRed();
		vos_delay_msecs(250);
		monLedsOff();
		vos_delay_msecs(250);
#endif //CFGMONITOR_LEDS_FLASH
		vos_delay_msecs(250);

		// if we are on the UART interface then we will loop until the
		// CTS (or DSR) pin goes active (i.e. a terminal application on
		// a PC or an application processor is activated)
		if (monInterface == UART)
		{
			monitor_iocb.ioctl_code = VOS_IOCTL_UART_GET_MODEM_STATUS;
			monitor_iocb.get.param = 0;
			vos_dev_ioctl(hMonitor, &monitor_iocb);

			monitor_iocb.get.param &= (UART_MODEM_STATUS_CTS | UART_MODEM_STATUS_DSR);

			if (monitor_iocb.get.param != (UART_MODEM_STATUS_CTS | UART_MODEM_STATUS_DSR))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	while (1);

	// report that the firmware is ready to start
#ifdef CFGMONITOR_SHOW_VERSION
	msgOnline();
#endif // CFGMONITOR_SHOW_VERSION

	hUsb1                 = vos_dev_open(VOS_DEV_USB1);
	hFT232                = vos_dev_open(VOS_DEV_FT232);

	// configure FT232BM
	cfgslaveft232_init(&slave_desc);
	slave_iocb.ioctl_code = VOS_IOCTL_USBSLAVEFT232_SET_DESCRIPTORS;
	slave_iocb.set.data   = &slave_desc;
	vos_dev_ioctl(hFT232, &slave_iocb);

	// attach FT232BM to USB Slave port A
	slave_iocb.ioctl_code = VOS_IOCTL_USBSLAVEFT232_ATTACH;
	slave_iocb.set.data   = (void *) hUsb1;
	vos_dev_ioctl(hFT232, &slave_iocb);

	// initialise monitor
	hostSelectDevice(0);

	while (1)
	{
		// STATE: USB1 no device USB2 no device
		// NEXT:  USB1 device USB2 no device
		//        USB1 no device USB2 device
		if (curState == STATE_NONE_NONE)
		{
			// check for connect of USB device on port 2
			if (slaveConnected(hUsb1) == usbsStateConfigured)
			{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
				monSlaveEnabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
				nextState         = STATE_SLAVE_NONE;
			}
			// check for connect of USB device on port 2
			else if (hostConnected(hUsb2) == PORT_STATE_ENUMERATED)
			{
				monDeviceDetected(USBHOST_PORT_2);
				hostAddToDeviceList(hUsb2);
				// if not a disk then go here
				nextState = STATE_NONE_DEVICE;
			}
		}

		// STATE: USB1 device USB2 no device
		// NEXT:  USB1 device USB2 device
		//        USB1 no device USB2 no device
		if (curState == STATE_SLAVE_NONE)
		{
			if (slaveConnected(hUsb1) != usbsStateConfigured)
			{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
				monSlaveDisabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
				nextState = STATE_NONE_NONE;
			}
			else if (hostConnected(hUsb2) == PORT_STATE_ENUMERATED)
			{
				monDeviceDetected(USBHOST_PORT_2);
				hostAddToDeviceList(hUsb2);
				nextState = STATE_SLAVE_DEVICE;
			}
			else
			{
				if (checkDataMode())
				{
					ackDataMode();
					// slave is only one connected
					monDataModeSlave();
					ackCmdMode();
				}
			}
		}

		// STATE: USB1 no device USB2 device
		// NEXT:  USB1 no device USB2 no device
		//        USB1 no device USB2 disk
		//        USB1 device USB2 device
		if (curState == STATE_NONE_DEVICE)
		{
			if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
			{
				monDeviceRemoved(USBHOST_PORT_2);
				hostRemoveFromDeviceList(hUsb2);

				monLedsOff();

				host2DevType = 0;
				nextState    = STATE_NONE_NONE;
			}
			else if (hostConnected(hUsb2) == PORT_STATE_ENUMERATED)
			{
				// device is detected and enumerated
				if (host2DevType == 0)
				{
					host2DevType = hostGetPortDevType(hUsb2);

					if (host2DevType & hostHasBOMSClass)
					{
						monLedsRed();
						hBoms = vos_dev_open(VOS_DEV_BOMS);

						if (diskCheckDisk(hUsb2) == MON_SUCCESS)
						{
							monLedsGreen();
							nextState = STATE_NONE_DISK;
							monPrompt();
						}
						else
						{
							monLedsOff();
							vos_dev_close(hBoms);
						}
					}
				}
			}

			if (curState == nextState)
			{
				if (slaveConnected(hUsb1) == usbsStateConfigured)
				{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
					monSlaveEnabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
					nextState = STATE_SLAVE_DEVICE;
				}
				// This allows V2DPS to detect Data or Command mode
				if (checkDataMode())
				{
					ackDataMode();
					// no slave connected
					monDataModeHost();
					ackCmdMode();
				}
			}
		}

		// STATE: USB1 no device USB2 disk
		// NEXT:  USB1 no device USB2 no device
		//        USB1 device USB2 disk
		if (curState == STATE_NONE_DISK)
		{
			if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
			{
				diskRemoveDisk();

				monDeviceRemoved(USBHOST_PORT_2);
				hostRemoveFromDeviceList(hUsb2);

				monLedsOff();

				host2DevType = 0;
				nextState    = STATE_NONE_NONE;

				vos_dev_close(hBoms);
			}

			if (curState == nextState)
			{
				if (slaveConnected(hUsb1) == usbsStateConfigured)
				{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
					monSlaveEnabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
					nextState = STATE_SLAVE_DISK;
				}
			}
		}

		// STATE: USB1 device USB2 device
		// NEXT:  USB1 no device USB2 device
		//        USB1 device USB2 no device
		//        USB1 device USB2 disk
		if (curState == STATE_SLAVE_DEVICE)
		{
			if (slaveConnected(hUsb1) < 3)
			{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
				monSlaveDisabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
				nextState = STATE_NONE_DEVICE;
			}

			if (curState == nextState)
			{
				if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
				{
					monDeviceRemoved(USBHOST_PORT_2);
					hostRemoveFromDeviceList(hUsb2);

					monLedsOff();

					host2DevType = 0;
					nextState    = STATE_SLAVE_NONE;
				}
				else if (hostConnected(hUsb2) == PORT_STATE_ENUMERATED)
				{
					// device is detected and enumerated
					if (host2DevType == 0)
					{
						host2DevType = hostGetPortDevType(hUsb2);

						if (host2DevType & hostHasBOMSClass)
						{
							monLedsRed();
							hBoms = vos_dev_open(VOS_DEV_BOMS);

							// leds = green flash
							if (diskCheckDisk(hUsb2) == MON_SUCCESS)
							{
								// leds = green on
								monLedsGreen();
								nextState = STATE_SLAVE_DISK;
								monPrompt();
							}
							else
							{
								monLedsOff();
								vos_dev_close(hBoms);
							}
						}
					}
				}
			}

			if (curState == nextState)
			{
				if (checkDataMode())
				{
					ackDataMode();
					monDataMode();
					ackCmdMode();
				}
			}
		}

		// STATE: USB1 no device USB2 disk
		// NEXT:  USB1 no device USB2 disk
		//        USB1 device USB2 no device
		if (curState == STATE_SLAVE_DISK)
		{
			if (slaveConnected(hUsb1) != usbsStateConfigured)
			{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
				monSlaveDisabled();
#endif // CFGMONITOR_SHOW_CONNECTS_REMOVALS
				nextState = STATE_NONE_DISK;
			}
			else if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
			{
				diskRemoveDisk();

				monDeviceRemoved(USBHOST_PORT_2);
				hostRemoveFromDeviceList(hUsb2);

				monLedsOff();

				host2DevType = 0;
				nextState    = STATE_SLAVE_NONE;

				vos_dev_close(hBoms);
			}

			if (curState == nextState)
			{
				if (checkDataMode())
				{
					ackDataMode();
					// connect to slave in preference to disk
					monDataModeSlave();
					ackCmdMode();
				}
			}
		}

		if (curState == nextState)
		{
			// check and process commands
			monCheck((curState == STATE_SLAVE_DISK) || (curState == STATE_NONE_DISK));
		}

		curState = nextState;
	}
}

/*
** checkDataMode
**
** Check if the DATAREQ pin is asserted to enter data mode
**
** Parameters: none
** Returns: TRUE if asserted, FALSE if not
** Comments: Must be available externally to monitor.c
*/
unsigned char checkDataMode(void)
{
	unsigned char bitVal;
	unsigned char status = FALSE;

#ifdef CFGMONITOR_DATA_MODE
	// do not move to data mode with hub devices
	if ((curDevice == 'S') || ((deviceList[curDevice].type & hostHasHub) == 0))
	{
		if (monInterface == UART)
		{
			vos_gpio_read_port(GPIO_PORT_C, &bitVal);

			if ((bitVal & 0x20) == 0)
			{
				status = TRUE;
			}
		}
		else
		{
			// SPI and FIFO DATAREQ pins
			vos_gpio_read_port(GPIO_PORT_D, &bitVal);

			if ((bitVal & 0x10) == 0)
			{
				status = TRUE;
			}
		}
	}
#endif                                 // CFGMONITOR_DATA_MODE

	return status;
}

/*
** ackDataMode
**
** Acknowledge data mode by asserting the DATAACK pin
**
** Parameters: none
** Returns: none
** Comments: Must be available externally to monitor.c
*/
void ackDataMode(void)
{
	unsigned char bitVal;

	if (monInterface == UART)
	{
		vos_gpio_read_port(GPIO_PORT_C, &bitVal);
		bitVal &= 0xef;
		vos_gpio_write_port(GPIO_PORT_C, bitVal);
	}
	else
	{
		// SPI and FIFO DATAREQ pins
		vos_gpio_read_port(GPIO_PORT_D, &bitVal);
		bitVal &= 0xdf;
		vos_gpio_write_port(GPIO_PORT_D, bitVal);
	}
}

/*
** ackCmdMode
**
** Return to command mode and deassert DATAACK pin
**
** Parameters: none
** Returns: none
** Comments: Must be available externally to monitor.c
*/
void ackCmdMode(void)
{
	unsigned char bitVal;

	if (monInterface == UART)
	{
		vos_gpio_read_port(GPIO_PORT_C, &bitVal);
		bitVal |= 0x10;
		vos_gpio_write_port(GPIO_PORT_C, bitVal);
	}
	else
	{
		// SPI and FIFO DATAREQ pins
		vos_gpio_read_port(GPIO_PORT_D, &bitVal);
		bitVal |= 0x20;
		vos_gpio_write_port(GPIO_PORT_D, bitVal);
	}
}

/*
** monDataModeSlave
**
** Perform input/output operations required in data mode for slave.
**
** Parameters: none
** Returns: none
** Comments: Uses current device (SC command).
**  Needs access to checkDataMode() function.
*/
void monDataModeSlaveIn(vos_semaphore_t *inDone);

void monDataModeSlave()
{
	vos_tcb_t *tcbDm = NULL;
	char strDm[8] = {'D', 'A', 'T', 'A', 'I', 'N', 'S', 0};  // "DATAINS"
	common_ioctl_cb_t     uart_iocb;   // UART iocb for getting bytes available.
	unsigned short        dataLength, dataRead;
	vos_semaphore_t		  inDone;
	// maximum allowed data
	unsigned char         buf[64];

	vos_init_semaphore(&inDone, 0);
	// make a new thread for the UART to USBSlave
	// this should be the same priority as the application
	tcbDm = vos_create_thread_ex(20, 512, monDataModeSlaveIn, strDm, 2, &inDone);
	if (tcbDm == NULL)
		return;
		
	// Data mode for Slave
	while (checkDataMode())
	{
		// read slave, send to monitor
		uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
		vos_dev_ioctl(hFT232, &uart_iocb);

		dataLength = uart_iocb.get.queue_stat;

		if (dataLength > 64)
			dataLength = 64;

		if (dataLength)
		{
			vos_dev_read(hFT232, buf, dataLength, NULL);
			vos_dev_write(hMonitor, buf, dataLength, &dataRead);
		}

		if (slaveConnected(hUsb1) != usbsStateConfigured)
			break;

		vos_delay_msecs(0);
	}
	vos_wait_semaphore(&inDone);
}

void monDataModeSlaveIn(vos_semaphore_t *inDone)
{
	common_ioctl_cb_t     uart_iocb;   // UART iocb for getting bytes available.
	unsigned short        dataLength, dataRead;
	// maximum allowed data
	unsigned char         buf[64];

	// Data mode for Slave
	while (checkDataMode())
	{
		// read monitor, send to slave
		uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
		vos_dev_ioctl(hMonitor, &uart_iocb);

		dataLength = uart_iocb.get.queue_stat;

		if (dataLength > 64)
			dataLength = 64;

		if (dataLength)
		{
			vos_dev_read(hMonitor, buf, dataLength, NULL);
			vos_dev_write(hFT232, buf, dataLength, &dataRead);
		}

		if (slaveConnected(hUsb1) != usbsStateConfigured)
			break;
			
		vos_delay_msecs(0);
	}
	
	vos_signal_semaphore(inDone);
}

/* MonDataModeHost
**
** Perform input/output operations required in data mode for host.
**
** Parameters: none
** Returns: none
** Comments: Uses current device (SC command).
**  Needs access to checkDataMode() function.
*/
void monDataModeHostIn(usbhost_ep_handle_ex epHandleIn, usbDevList *pUsbDevList,
	unsigned char bulkInEp, unsigned short maxSize);

void monDataModeHost()
{
	usbhost_xfer_t xfer;
	vos_tcb_t *tcbDm = NULL;
	char strDm[8] = {'D', 'A', 'T', 'A', 'I', 'N', 'H', 0};  // "DATAINH"
	vos_semaphore_t semOut;
	common_ioctl_cb_t uart_iocb;       // UART iocb for getting bytes available.
	unsigned short numRead, dataLength;
	usbhost_ep_handle_ex epHandleIn, epHandleOut;
	usbhost_ioctl_cb_ep_info_t epInfo; // Structure to store our endpoint data.
	usbhost_device_handle_ex ifDev;
	usbhost_ioctl_cb_t host_ioctl_cb;  // IOCTL block for our requests.
	// maximum allowed data (arbitary size)
	unsigned char buf[64];
	unsigned char status;
	unsigned char bulkInEp = USBHOST_XFER_FLAG_START_BULK_ENDPOINT_LIST;
	unsigned char bulkOutEp = USBHOST_XFER_FLAG_START_BULK_ENDPOINT_LIST;
	usbDevList *pUsbDevList = &deviceList[curDevice];

	// Data mode for Host
	ifDev = pUsbDevList->ifDev;
	epHandleIn = NULL;
	epHandleOut = NULL;

	// Pipe In Endpoint
	host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_IN_ENDPOINT_HANDLE;
	host_ioctl_cb.handle.dif = deviceList[curDevice].ifDev;
	host_ioctl_cb.get = &epHandleIn;
	vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);

	if (!epHandleIn)                   // We have no handle to a bulk endpoint so try an interrupt
	{
		// Acquire Handle to interrupt endpoint
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_INT_IN_ENDPOINT_HANDLE;
		vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);
		bulkInEp = 0;
	}

	if (epHandleIn)
	{
		// get max IN endpoint transfer size
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
		host_ioctl_cb.handle.ep = epHandleIn;
		host_ioctl_cb.get = &epInfo;

		vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);
	}

	// Pipe Out Endpoint
	host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_BULK_OUT_ENDPOINT_HANDLE;
	host_ioctl_cb.handle.dif = deviceList[curDevice].ifDev;
	host_ioctl_cb.get = &epHandleOut;
	vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);

	if (!epHandleOut)                  // We have no handle to a bulk endpoint so try an interrupt
	{
		// Acquire Handle to interrupt endpoint
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_INT_OUT_ENDPOINT_HANDLE;
		vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);
		bulkOutEp = 0;
	}

	vos_init_semaphore(&semOut, 0);

	// only enter data mode if there are in or out endpoints
	if (epHandleIn)
	{
		tcbDm = vos_create_thread_ex(24, 1200, monDataModeHostIn, strDm, 9,
					//sizeof(usbhost_ep_handle_ex) + sizeof(usbDevList*) +
					//sizeof(unsigned char) + sizeof(unsigned short),
					epHandleIn, pUsbDevList, bulkInEp, epInfo.max_size);
		if (tcbDm == NULL)
			return;

		// setup IOCTL for detecting disconnected IN endpoint
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_GET_ENDPOINT_INFO;
		host_ioctl_cb.handle.ep = epHandleIn;
		host_ioctl_cb.get = &epInfo;
	}

	if ((epHandleIn == NULL) && (epHandleOut == NULL))
	{
		// no device present
		return;
	}

	while(checkDataMode())
	{
		if (epHandleOut)
		{
			uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
			vos_dev_ioctl(hMonitor, &uart_iocb);
			dataLength = uart_iocb.get.queue_stat;

			if (dataLength)
			{
				if (dataLength > 64)
					dataLength = 64;

				vos_dev_read(hMonitor, buf, dataLength, &numRead);

				xfer.buf = buf;
				xfer.len = dataLength;
				xfer.ep = epHandleOut;
				xfer.s = &semOut;
				xfer.cond_code = USBHOST_CC_NOTACCESSED;
				xfer.flags = bulkOutEp;
				status = vos_dev_write(pUsbDevList->host, (unsigned char *) &xfer, sizeof(usbhost_xfer_t), NULL);

				// catch a disconnected device
				if (status == USBHOST_NOT_FOUND)
					break;
			}
		}
		if (epHandleIn && tcbDm)
		{
			// detect disconnected IN endpoint (DATAIN thread will have completed)
			status = vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);
			if (status == USBHOST_NOT_FOUND)
			{
				break;
			}
		}
	}

	if (tcbDm != NULL)
	{
		// remove transfer request (and stop thread)
		host_ioctl_cb.ioctl_code = VOS_IOCTL_USBHOST_DEVICE_CLEAR_ENDPOINT_TRANSFER;
		host_ioctl_cb.handle.ep = epHandleIn;
		vos_dev_ioctl(pUsbDevList->host, &host_ioctl_cb);
	}
}

void monDataModeHostIn(usbhost_ep_handle_ex epHandleIn, usbDevList *pUsbDevList,
				   unsigned char bulkInEp, unsigned short maxSize)
{
	usbhost_xfer_t xfer;
	unsigned char status;
	unsigned char *buf;
	unsigned char *pbuf;
	vos_semaphore_t semIn;

	vos_init_semaphore(&semIn, 0);

	buf = vos_malloc(maxSize);
	pbuf = buf;
	if (pUsbDevList->type == hostHasFTDI)
	{
		pbuf = &buf[2];
	}

	while (checkDataMode())
	{
		xfer.buf = buf;
		xfer.len = maxSize;
		xfer.ep = epHandleIn;
		xfer.s = &semIn;
		xfer.cond_code = USBHOST_CC_NOTACCESSED;
		xfer.flags = bulkInEp | USBHOST_XFER_FLAG_ROUNDING;
		status = vos_dev_read(pUsbDevList->host, (unsigned char *) &xfer, sizeof(usbhost_xfer_t), NULL);

		if (status == USBHOST_OK)
		{
			// only report data on a complete transfer
			if (xfer.cond_code == USBHOST_CC_NOERROR)
			{
				if (xfer.len)
				{
					if (pUsbDevList->type == hostHasFTDI)
					{
						xfer.len -= 2;
					}

					monWrite((char *) pbuf, xfer.len);
				}
			}
			else
			{
				break;
			}
		}
		// detect a disconnected device
		else if (status == USBHOST_NOT_FOUND)
		{
			break;
		}
	}
	vos_free(buf);
}


/*
** MonDataMode
**
** Perform data mode input/output
**
** Parameters: none
** Returns: none
** Comments: Selects either slave or host data mode depending on last SC command
*/
void monDataMode()

{
	// check if slave device selected
	if (curDevice == 'S')
		monDataModeSlave();
	else
		monDataModeHost();
}

/*
** msgOnline
**
** Initialise the monitor.
**
** Parameters: none
** Returns: none
** Comments: Must be availble externally from monitor.c
*/
void msgOnline()
{
	char *onlineStr1 = "\rVer V2DPS";
	char *onlineStr2 = " On-Line:\r";

	monWriteString(onlineStr1);
	monVersion();
	monWriteString(onlineStr2);
}

