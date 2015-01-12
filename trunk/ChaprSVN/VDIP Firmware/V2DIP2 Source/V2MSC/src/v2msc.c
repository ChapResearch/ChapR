/*
** v2dap.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2MSC Configuration and State Machine
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

#include "vos.h"

#include "stdlib.h"

#include "SPIMaster.h"
#include "SPISlave.h"
#include "FIFO.h"
#include "UART.h"
#include "USBHost.h"
#include "USB.h"
#include "msi.h"
#include "BOMS.h"
#include "FAT.h"

#include "host.h"
#include "v2msc.h"
#include "monitor.h"
#include "cfgmonitor.h"
#include "disk.h"
#include "MP3_VS1053.h"


VOS_HANDLE hMonitor;
VOS_HANDLE hBoms;
VOS_HANDLE hUsb2;
VOS_HANDLE hSPIMaster;
VOS_HANDLE hMP3;

vos_mutex_t mPlayState;
vos_mutex_t mPlaying;
vos_mutex_t mMP3File;
vos_semaphore_t sPaused;
vos_semaphore_t sPlayAll;
vos_semaphore_t sPlaying;

unsigned char monInterface; // Interface to the monitor.
#define UART 0
#define SPI	 1

// driver context structure
fat_context fatContext;

// thread control block for the monitor
vos_tcb_t *tcbMonitor;

unsigned char curState = STATE_NONE;

// forward declarations
void firmware(void);
/*unsigned char checkDataMode(void);
void ackDataMode(void);
void ackCmdMode(void);*/
void msgOnline(void);

extern void MP3();
extern void Play(void);
extern void PlayAllTrackMGMT(void);
extern unsigned char playState;

void SPIMaster_Setup()
{
	common_ioctl_cb_t spim_iocb;

	// Setup SPI Master	
	// enable DMA
	spim_iocb.ioctl_code = VOS_IOCTL_COMMON_ENABLE_DMA;
	spim_iocb.set.param = DMA_ACQUIRE_AS_REQUIRED;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set clock phase
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SCK_CPHA;
	spim_iocb.set.param = SPI_MASTER_SCK_CPHA_0;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set clock polarity
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SCK_CPOL;
	spim_iocb.set.param = SPI_MASTER_SCK_CPOL_0;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set data order
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_DATA_ORDER;
	spim_iocb.set.param = SPI_MASTER_DATA_ORDER_MSB; // MSB first
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set clock rate
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SET_SCK_FREQUENCY;
	spim_iocb.set.spi_master_sck_freq = 3000000;	
	//spim_iocb.set.spi_master_sck_freq = 4000000;	
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// Set data delay
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SET_DATA_DELAY;
	spim_iocb.set.param = 0;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set initial state of chip select 0 pin
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_0;
	spim_iocb.set.param = SPI_MASTER_SS_DISABLE;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);

	// set initial state of chip select 1 pin
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_1;
	spim_iocb.set.param = SPI_MASTER_SS_DISABLE;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);
}

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
	spimaster_context_t spim_ctx;
	fifo_context_t fifo_ctx;
	spislave_context_t spislave_ctx;
	uart_context_t uart_ctx;
	usbhost_context_t usb_ctx;
	unsigned char dataBuf;
	unsigned short numRead;
	unsigned char monInt;

	// coinfigure UART, FIFO and SPISlave context structures
	uart_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	fifo_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;
	spislave_ctx.slavenumber = SPI_SLAVE_0;
	spislave_ctx.buffer_size = VOS_BUFFER_SIZE_128_BYTES;

	// call VOS initialisation routines
	vos_init(10, VOS_TICK_INTERVAL, NUMBER_OF_DEVICES);
	vos_set_idle_thread_tcb_size(500);
	vos_set_clock_frequency(VOS_48MHZ_CLOCK_FREQUENCY);
	
	// initialise USB Host device driver
	usb_ctx.if_count = 16;
	usb_ctx.ep_count = 32;
	usb_ctx.xfer_count = 2;
	usb_ctx.iso_xfer_count = 1;
	usbhost_init(-1, VOS_DEV_USB2, &usb_ctx);
	// initialise BOMS device driver
	boms_init(VOS_DEV_BOMS);	
	// initialise SPI Master driver
	spim_ctx.buffer_size = VOS_BUFFER_SIZE_512_BYTES;
	spimaster_init(VOS_DEV_SPIM, &spim_ctx);
	// initialise SPI Master driver
	mp3_VS1053_init(VOS_DEV_MP3, NULL);
	
	vos_init_mutex(&mPlayState, 0);
	vos_init_mutex(&mPlaying, 0);
	vos_init_mutex(&mMP3File, 0);
	vos_init_semaphore(&sPaused, 0);
	vos_init_semaphore(&sPlayAll, 0);
	vos_init_semaphore(&sPlaying, 0);
		
	/* Monitor Interface Selection
	   Pin 46       Monitor Mode
	   Pull-Up      UART
	   Pull-Down    SPI	   
	*/
	// determine what interface the monitor is going to be using
	if (vos_get_package_type() == VINCULUM_II_48_PIN) {
		dataBuf = 0;
		// ACBUS6 pin 46
		vos_iomux_define_input(46, IOMUX_IN_GPIO_PORT_B_1);				
		vos_gpio_read_port(GPIO_PORT_B, &dataBuf); // Read the data on the GPIO pins.
	}
	else {
		// default interface is UART for 64 pin package
		dataBuf = 0;
	}

	// High for UART, low for SPI Slave...
	if (dataBuf & 0x02) { // UART
		uart_init(VOS_DEV_MON, &uart_ctx);
		monInterface = UART;
	}
	else { // SPI
		spislave_init(VOS_DEV_MON, &spislave_ctx);
		monInterface = SPI;
	}
	
	// This firmware is only available on a 48pin package
	if (vos_get_package_type() == VINCULUM_II_48_PIN) {	
		
		// Route signals for MP3 decoder
		// DCS# -> Pin 15
		// VS1053b's XRESET# pin is connected to VNC2's pin 32.
		// VS1053b's DREQ pin is connected to VNC2's pin 33.
		vos_iomux_define_output(15, IOMUX_OUT_SPI_MASTER_CS_1);
		vos_iomux_define_output(16, IOMUX_OUT_GPIO_PORT_B_5);
		vos_iomux_define_input(18, IOMUX_IN_GPIO_PORT_B_6);
		
		// Route Signals for LEDs
		// LED1 -> 35
		// LED2 -> 36
		vos_iomux_define_bidi(35, IOMUX_IN_GPIO_PORT_A_4, IOMUX_OUT_GPIO_PORT_A_4);
		vos_iomux_define_bidi(36, IOMUX_IN_GPIO_PORT_A_5, IOMUX_OUT_GPIO_PORT_A_5);
		
		// Route signals for the SPI Master
		// SCLK -> Pin 20
		// MOSI -> Pin 21
		// MISO -> Pin 22
		// SS xCS# -> Pin 23
		vos_iomux_define_output(20, IOMUX_OUT_SPI_MASTER_CLK);
		vos_iomux_define_output(21, IOMUX_OUT_SPI_MASTER_MOSI);
		vos_iomux_define_input(22, IOMUX_IN_SPI_MASTER_MISO);
		vos_iomux_define_output(23, IOMUX_OUT_SPI_MASTER_CS_0);
		
		// configure the IOMUX depending on the monitor interface selected
		if (monInterface == UART)
		{
			// RI# -> Pin 14
			// TXD -> Pin 31
			// RXD -> Pin 32
			// RTS# -> Pin 33
			// CTS# -> Pin 34
			vos_iomux_define_input(14, IOMUX_IN_UART_RI);
			vos_iomux_define_output(31, IOMUX_OUT_UART_TXD);
			vos_iomux_define_input(32, IOMUX_IN_UART_RXD);
			vos_iomux_define_output(33, IOMUX_OUT_UART_RTS_N);
			vos_iomux_define_input(34, IOMUX_IN_UART_CTS_N);
		}
		else if (monInterface == SPI)
		{
			// SCLK -> Pin 31
			// SDI -> Pin 32
			// SD0 -> Pin 33
			// CS -> Pin 34
			vos_iomux_define_input(31, IOMUX_IN_SPI_SLAVE_0_CLK);
			vos_iomux_define_input(32, IOMUX_IN_SPI_SLAVE_0_MOSI);
			vos_iomux_define_output(33, IOMUX_OUT_SPI_SLAVE_0_MISO);
			vos_iomux_define_input(34, IOMUX_IN_SPI_SLAVE_0_CS);
		}
	}
	
	// create threads for firmware application
	tcbMonitor = vos_create_thread_ex(20, 1200, firmware, "Application\0", 0);	

	// start VOS scheduler
	vos_start_scheduler();

main_loop:
	goto main_loop;
}
		
unsigned char CheckUSB2Enumerated(unsigned char *curState)
{
	if (hostConnected(hUsb2) == PORT_STATE_ENUMERATED)
	{
		monDeviceDetected(USBHOST_PORT_2);
		hostAddToDeviceList(hUsb2);
		*curState = STATE_DEVICE;
		return TRUE;
	}
	return FALSE;
}
	
unsigned char CheckUSB2Disconnected(unsigned char *curState)
{
	if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
	{
		monDeviceRemoved(USBHOST_PORT_2);
		hostRemoveFromDeviceList(hUsb2);
		monLedsOff();		
		*curState = STATE_NONE;
		return TRUE;
	}
	return FALSE;
}

unsigned char CheckUSB2DiskDisconnected(unsigned char *curState)
{
	if (hostConnected(hUsb2) == PORT_STATE_DISCONNECTED)
	{
		if (playState == PLAYING) {
			*curState = STATE_NONE;
			monWriteString("Stopped\r");
			monWriteString("Command Failed\r");
		}
		
		diskRemoveDisk();
		monDeviceRemoved(USBHOST_PORT_2);
		hostRemoveFromDeviceList(hUsb2);
		monLedsOff();		
		*curState = STATE_NONE;
		vos_dev_close(hBoms);
		return TRUE;
	}
	return FALSE;
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
	common_ioctl_cb_t monitor_iocb, spim_iocb;
	mp3_VS1053_ioctl_cb mp3_iocb;
	unsigned char current_volume = 0x10; // initial volume is 0x15

	// general purpose variables
	unsigned char status;
	unsigned char count;

	// current device type bitmap for USB ports
	//unsigned char host1DevType = 0;
	unsigned char host2DevType = 0;	
	// initialise monitor
	monInit(CFGMONITOR_MON_INIT);

	vos_gpio_set_port_mode(GPIO_PORT_A, 0xff);  // set all as output

	if (monInterface == UART)
	{
		// set DATAACK as output DATAREQ as input
		vos_gpio_set_port_mode(GPIO_PORT_C, 0x10);
	}
	else
	{
		vos_gpio_set_port_mode(GPIO_PORT_C, 0x00);  // set all as input
	}

	if (monInterface == UART)
	{
		vos_gpio_set_port_mode(GPIO_PORT_D, 0x00);  // set all as input
	}
	else
	{
		// set DATAACK as output DATAREQ as input
		vos_gpio_set_port_mode(GPIO_PORT_D, 0x10);
	}

	/* set LEDs to all off */
	monLedsOff();

	/* find and open USB drivers */	
	hUsb2 = vos_dev_open(VOS_DEV_USB2);
	hMonitor = vos_dev_open(VOS_DEV_MON);
	hSPIMaster = vos_dev_open(VOS_DEV_SPIM);
	hMP3 = vos_dev_open(VOS_DEV_MP3);

	/* UART setup */
	/* set baud rate to 9600 baud */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_BAUD_RATE;
	monitor_iocb.set.uart_baud_rate = CFGMONITOR_BAUD;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set flow control */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_FLOW_CONTROL;
	monitor_iocb.set.param = CFGMONITOR_FLOW_CONTROL;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set data bits */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_DATA_BITS;
	monitor_iocb.set.param = CFGMONITOR_DATA_BITS;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set stop bits */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_STOP_BITS;
	monitor_iocb.set.param = CFGMONITOR_STOP_BITS;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	/* set parity */
	monitor_iocb.ioctl_code = VOS_IOCTL_UART_SET_PARITY;
	monitor_iocb.set.param = CFGMONITOR_PARITY;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	/* SPI Slave Setup */
	// set mode for SPI
	monitor_iocb.ioctl_code = VOS_IOCTL_SPI_SLAVE_SET_MODE;
	monitor_iocb.set.param = SPI_SLAVE_MODE_VI_COMPATIBLE;
	vos_dev_ioctl(hMonitor, &monitor_iocb);

	// Enable DMA for monitor
	monitor_iocb.ioctl_code = VOS_IOCTL_COMMON_ENABLE_DMA;
	monitor_iocb.set.param = DMA_ACQUIRE_AS_REQUIRED;
	vos_dev_ioctl(hMonitor, &monitor_iocb);
	
	vos_gpio_set_pin_mode(GPIO_B_6, 0);
	vos_gpio_set_pin_mode(GPIO_B_5, 1);
	
	SPIMaster_Setup();

	mp3_iocb.ioctl_code = VOS_IOCTL_MP3_VS1053_ATTACH;
	mp3_iocb.spi_master_handle = hSPIMaster;
	// Internally, attach GPIO information of XRESET and DREQ as well
	mp3_iocb.XRESET_GPIO_port = PORT_B; // GPIO port used for XRESET pin
	mp3_iocb.XRESET_GPIO_pin = 5; // GPIO pin used for XRESET pin
	mp3_iocb.DREQ_GPIO_port = PORT_B; // GPIO port used for DREQ pin
	mp3_iocb.DREQ_GPIO_pin = 6; // GPIO pin used for DREQ pin
	mp3_iocb.SDI_CS_identifier = SPI_CHIP_SELECT_1;
	mp3_iocb.SCI_CS_identifier = SPI_CHIP_SELECT_0;	
	vos_dev_ioctl(hMP3, &mp3_iocb);	
	
	mp3_iocb.ioctl_code = VOS_IOCTL_MP3_VS1053_SETUP;
	vos_dev_ioctl(hMP3, &mp3_iocb);
	
	do
	{
		// loop on flashing LEDS
#ifdef CFGMONITOR_LEDS_FLASH
		//monLedsGreen();
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
#endif                                 // CFGMONITOR_LEDS_FLASH
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
#endif                                 // CFGMONITOR_SHOW_VERSION
	
	// initialise monitor
	hostSelectDevice(0);	
	
	// Create our thread that will poll for data to send to the MP3 decoder...	
	vos_create_thread_ex(23, 512, &MP3, "MP3\0", 0);
	vos_create_thread_ex(22, 1400, &Play, "Play\0", 0);
	vos_create_thread_ex(20, 1400, &PlayAllTrackMGMT, "Play MGMT\0", 0);	
	
	while (1)
	{
		switch (curState) {
		// State:	STATE_NONE
		// Next:	STATE_DEVICE
		case STATE_NONE:
			CheckUSB2Enumerated(&curState);
			break;
		// State:	STATE_DEVICE
		// Next:	STATE_NONE
		//			STATE_DISK
		case STATE_DEVICE:
			if (!CheckUSB2Disconnected(&curState)) // Check the device is still connected...
			{
				// device is detected and enumerated
				host2DevType = hostGetPortDevType(hUsb2);

				if (host2DevType & hostHasBOMSClass)
				{
					monLedsRed();
					hBoms = vos_dev_open(VOS_DEV_BOMS);
					monLedsGreen();

					if (diskCheckDisk(hUsb2) == MON_SUCCESS)
					{
						monLedsGreen();
						curState = STATE_DISK;
						monPrompt();
					}
					else
					{
						monLedsOff();
						vos_dev_close(hBoms);
					}
				}
			}
			else
				host2DevType = 0;
			break;
		// State:	STATE_DISK
		// Next:	STATE_NONE
		case STATE_DISK:
			if (CheckUSB2DiskDisconnected(&curState)) // Check the disk is still connected...
			{
				host2DevType = 0;
			}
			break;
		}

		// Process any commands...		
		monCheck(curState == STATE_DISK);
	}
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
	char *onlineStr1 = "\rVer V2MSC";
	char *onlineStr2 = " On-Line:\r";

	monWriteString(onlineStr1);
	monVersion();
	monWriteString(onlineStr2);
}

