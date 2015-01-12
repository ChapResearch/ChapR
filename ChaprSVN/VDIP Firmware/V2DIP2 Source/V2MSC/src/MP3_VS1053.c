/*
** VS1053.c
**
** Copyright © 2009-2011 Future Technology Devices International Limited
**
** Company Confidential
**
** C Source file containing definitions for VLSI VS1053 MP3 codec.
**
** Author: FTDI
** Project: Vinculum II
** Module: VS1053 MP3 Codec Driver
** Requires: VOS, SPIMaster, GPIO
** Comments:
**
** History:
**  1 – Initial version
**
*/

#include "SPIMaster.h"
#include "GPIO.h"
#include "MP3_VS1053.h"
#include "MP3_VS1053_defs.h"

#include "devman.h"

#define MP3_VS1053_BUFFER_SIZE 128

#define MASK_GPIO_EN (1 << 1)

/* Port registers */
#define VII_GPIO_SYS_CNTRL_1   0x180   /* GPIO Control Register */
#define VII_GPIO_CNTRL_PORTA_1 0x181
#define VII_GPIO_CNTRL_PORTB_1 0x182
#define VII_GPIO_CNTRL_PORTC_1 0x183
#define VII_GPIO_CNTRL_PORTD_1 0x184
#define VII_GPIO_CNTRL_PORTE_1 0x185
#define VII_GPIO_DATA_TX_PA_1  0x186
#define VII_GPIO_DATA_TX_PB_1  0x187
#define VII_GPIO_DATA_TX_PC_1  0x188
#define VII_GPIO_DATA_TX_PD_1  0x189
#define VII_GPIO_DATA_TX_PE_1  0x18a
#define VII_GPIO_DATA_RX_PA_1  0x18b
#define VII_GPIO_DATA_RX_PB_1  0x18c
#define VII_GPIO_DATA_RX_PC_1  0x18d
#define VII_GPIO_DATA_RX_PD_1  0x18e
#define VII_GPIO_DATA_RX_PE_1  0x18f
#define VII_GPIO_INT_REG_EN_1  0x19a   /* GPIO Interrupt Register */

port vII_gpio_sys_cntrl_1 @ VII_GPIO_SYS_CNTRL_1;
port vII_gpio_cntrl_porta_1 @ VII_GPIO_CNTRL_PORTA_1;
port vII_gpio_cntrl_portb_1 @ VII_GPIO_CNTRL_PORTB_1;
port vII_gpio_cntrl_portc_1 @ VII_GPIO_CNTRL_PORTC_1;
port vII_gpio_cntrl_portd_1 @ VII_GPIO_CNTRL_PORTD_1;
port vII_gpio_cntrl_porte_1 @ VII_GPIO_CNTRL_PORTE_1;
port vII_gpio_data_tx_pa_1 @ VII_GPIO_DATA_TX_PA_1;
port vII_gpio_data_tx_pb_1 @ VII_GPIO_DATA_TX_PB_1;
port vII_gpio_data_tx_pc_1 @ VII_GPIO_DATA_TX_PC_1;
port vII_gpio_data_tx_pd_1 @ VII_GPIO_DATA_TX_PD_1;
port vII_gpio_data_tx_pe_1 @ VII_GPIO_DATA_TX_PE_1;
port vII_gpio_data_rx_pa_1 @ VII_GPIO_DATA_RX_PA_1;
port vII_gpio_data_rx_pb_1 @ VII_GPIO_DATA_RX_PB_1;
port vII_gpio_data_rx_pc_1 @ VII_GPIO_DATA_RX_PC_1;
port vII_gpio_data_rx_pd_1 @ VII_GPIO_DATA_RX_PD_1;
port vII_gpio_data_rx_pe_1 @ VII_GPIO_DATA_RX_PE_1;
port vII_gpio_int_reg_en_1 @ VII_GPIO_INT_REG_EN_1;

/* Global variables */
vos_driver_t      			mp3_VS1053_cb;
mp3_VS1053_prvt_context 	mp3_VS1053_prvt_ctx;
common_ioctl_cb_t  			spim_iocb;

extern unsigned long dataAvailable;

/* Function prototypes */
unsigned char mp3_VS1053_attach(VOS_HANDLE spi_master_handle, unsigned char XRESET_GPIO_port, unsigned char XRESET_GPIO_pin, unsigned char DREQ_GPIO_port, unsigned char DREQ_GPIO_pin, unsigned char SDI_CS_identifier, unsigned char SCI_CS_identifier);
int	mp3_VS1053_detach();
int mp3_VS1053_select_control(void);
int mp3_VS1053_deselect_control(void);
int mp3_VS1053_select_data(void);
int mp3_VS1053_deselect_data(void);
int mp3_VS1053_hard_reset(void);
int mp3_VS1053_soft_reset(void);
int mp3_VS1053_setup(void);
int mp3_VS1053_wait_for_dreq(void);
int mp3_VS1053_write_reg(unsigned char reg_address, unsigned short data);
int mp3_VS1053_read_reg(unsigned char reg_address, unsigned short *data);
unsigned short mp3_VS1053_read_ram(unsigned short ram_address);
unsigned char mp3_VS1053_write(unsigned char *buf, unsigned short num_to_write, unsigned short *num_written);
unsigned char mp3_VS1053_ioctl(void *cb);

void mp3_VS1053_init(unsigned char mp3VS1053DevNum, mp3_VS1053_context_t *mp3Context)
{
	// Set up function pointers for our driver
	mp3_VS1053_cb.flags     = 0;
	mp3_VS1053_cb.read      = (PF_IO) NULL;
	mp3_VS1053_cb.write     = mp3_VS1053_write;
	mp3_VS1053_cb.ioctl     = mp3_VS1053_ioctl;
	mp3_VS1053_cb.interrupt = (PF_INT) NULL;
	mp3_VS1053_cb.open      = (PF_OPEN) NULL;
	mp3_VS1053_cb.close     = (PF_CLOSE) NULL;
	// OK - register with device manager
	vos_dev_init(mp3VS1053DevNum, &mp3_VS1053_cb, NULL);
	// Enable GPIO control
	vII_gpio_sys_cntrl_1 = MASK_GPIO_EN;
}

unsigned char mp3_VS1053_attach(VOS_HANDLE spi_master_handle, unsigned char XRESET_GPIO_port, unsigned char XRESET_GPIO_pin, unsigned char DREQ_GPIO_port, unsigned char DREQ_GPIO_pin, unsigned char SDI_CS_identifier, unsigned char SCI_CS_identifier)
{
	if ((XRESET_GPIO_port > PORT_E) || (DREQ_GPIO_port > PORT_E))
		return MP3_VS1053_INVALID_PARAMETER;
	if ((XRESET_GPIO_pin > 7) || (DREQ_GPIO_pin > 7))
		return MP3_VS1053_INVALID_PARAMETER;

	mp3_VS1053_prvt_ctx.hSpiMaster = spi_master_handle;
	mp3_VS1053_prvt_ctx.XRESET_GPIO_port = XRESET_GPIO_port;
	mp3_VS1053_prvt_ctx.XRESET_GPIO_pin = XRESET_GPIO_pin;
	mp3_VS1053_prvt_ctx.DREQ_GPIO_port = DREQ_GPIO_port;
	mp3_VS1053_prvt_ctx.DREQ_GPIO_pin = DREQ_GPIO_pin;
	mp3_VS1053_prvt_ctx.SDI_CS_identifier = SDI_CS_identifier;
	mp3_VS1053_prvt_ctx.SCI_CS_identifier = SCI_CS_identifier;

	return MP3_VS1053_OK;
}

int	mp3_VS1053_detach()
{
	mp3_VS1053_prvt_ctx.hSpiMaster = NULL;

	return MP3_VS1053_OK;
}

int mp3_VS1053_hard_reset(void)
{
	unsigned char mask;
	mask = 0x01 << mp3_VS1053_prvt_ctx.XRESET_GPIO_pin;

	switch (mp3_VS1053_prvt_ctx.XRESET_GPIO_port)
	{
		case PORT_A:
			vII_gpio_data_tx_pa_1 &= (~mask); // set the pin LOW
			vos_delay_msecs(3); // wait for some time
			vII_gpio_data_tx_pa_1 |= mask; // set the pin HIGH
			break;
		case PORT_B:
			vII_gpio_data_tx_pb_1 &= (~mask); // set the pin LOW
			vos_delay_msecs(3); // wait for some time
			vII_gpio_data_tx_pb_1 |= mask; // set the pin HIGH
			break;
		case PORT_C:
			vII_gpio_data_tx_pc_1 &= (~mask); // set the pin LOW
			vos_delay_msecs(3); // wait for some time
			vII_gpio_data_tx_pc_1 |= mask; // set the pin HIGH
			break;
		case PORT_D:
			vII_gpio_data_tx_pd_1 &= (~mask); // set the pin LOW
			vos_delay_msecs(3); // wait for some time
			vII_gpio_data_tx_pd_1 |= mask; // set the pin HIGH
			break;
		case PORT_E:
			vII_gpio_data_tx_pe_1 &= (~mask); // set the pin LOW
			vos_delay_msecs(3); // wait for some time
			vII_gpio_data_tx_pe_1 |= mask; // set the pin HIGH
			break;
		default:
			return MP3_VS1053_INVALID_PARAMETER;
	}
	return MP3_VS1053_OK;
}
	
int mp3_VS1053_soft_reset(void)
{
	mp3_VS1053_write_reg(SCI_MODE, MASK_SM_RESET); // Set Reset bit in mode register
	vos_delay_msecs(2); // At least 2 millisecond delay
	mp3_VS1053_wait_for_dreq(); // wait for startup
	// Then set mode register and clock register again
	mp3_VS1053_write_reg(SCI_MODE, MASK_SM_SDINEW_VS1002);
	mp3_VS1053_write_reg(SCI_CLOCKF, 0x8800);
	vos_delay_msecs(1); // 1 millisecond delay
	mp3_VS1053_wait_for_dreq(); // wait for the commands to complete

	return MP3_VS1053_OK;
}	

int mp3_VS1053_setup(void)
{
	unsigned short regVal;
	common_ioctl_cb_t spim_iocb;

	// Initialize SPI pins
	// Reset VS1053 codec hardware
	mp3_VS1053_hard_reset();
	// Configure VS1053 defaults
	mp3_VS1053_write_reg(SCI_MODE, MASK_SM_SDINEW_VS1002);	
	mp3_VS1053_write_reg(SCI_CLOCKF, 0xe000);
	return 0;
}

// Waits for the DREQ line to go high...
int mp3_VS1053_wait_for_dreq(void)
{
	unsigned char port_value = 0, mask;
	int timeout = 0;
	
	mask = 0x01 << mp3_VS1053_prvt_ctx.DREQ_GPIO_pin;
	
	switch (mp3_VS1053_prvt_ctx.DREQ_GPIO_port)
	{
		case PORT_A:
			// Read the pin. If the pin is LOW, continue reading it until it is HIGH
			do
			{
				port_value = vII_gpio_data_rx_pa_1;
			}
			while ((port_value & mask) == 0);
			break;
		case PORT_B:
			// Read the pin. If the pin is LOW, continue reading it until it is HIGH
			do {
				port_value = vII_gpio_data_rx_pb_1;
				//vos_delay_msecs(1);
			} while ((port_value & mask) == 0);
			break;
		case PORT_C:
			// Read the pin. If the pin is LOW, continue reading it until it is HIGH
			do
			{
				port_value = vII_gpio_data_rx_pc_1;
			}
			while ((port_value & mask) == 0);
			break;
		case PORT_D:
			// Read the pin. If the pin is LOW, continue reading it until it is HIGH
			do
			{
				port_value = vII_gpio_data_rx_pd_1;
			}
			while ((port_value & mask) == 0);
			break;
		case PORT_E:
			// Read the pin. If the pin is LOW, continue reading it until it is HIGH
			do
			{
				port_value = vII_gpio_data_rx_pe_1;
			}
			while ((port_value & mask) == 0);
			break;
		default:
			return MP3_VS1053_INVALID_PARAMETER;
	}		
}

int mp3_VS1053_select_control(void)	
{
	if (mp3_VS1053_prvt_ctx.SCI_CS_identifier == SPI_CHIP_SELECT_0)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_0;
	else if (mp3_VS1053_prvt_ctx.SCI_CS_identifier == SPI_CHIP_SELECT_1)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_1;
	spim_iocb.set.param = SPI_MASTER_SS_ENABLE;
	vos_dev_ioctl(mp3_VS1053_prvt_ctx.hSpiMaster, &spim_iocb);
	
	return MP3_VS1053_OK;
}

int mp3_VS1053_deselect_control(void)
{
	if (mp3_VS1053_prvt_ctx.SCI_CS_identifier == SPI_CHIP_SELECT_0)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_0;
	else if (mp3_VS1053_prvt_ctx.SCI_CS_identifier == SPI_CHIP_SELECT_1)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_1;
	spim_iocb.set.param = SPI_MASTER_SS_DISABLE;
	vos_dev_ioctl(mp3_VS1053_prvt_ctx.hSpiMaster, &spim_iocb);

	return MP3_VS1053_OK;
}

int mp3_VS1053_select_data(void)
{
	if (mp3_VS1053_prvt_ctx.SDI_CS_identifier == SPI_CHIP_SELECT_0)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_0;
	else if (mp3_VS1053_prvt_ctx.SDI_CS_identifier == SPI_CHIP_SELECT_1)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_1;
	spim_iocb.set.param = SPI_MASTER_SS_ENABLE;
	vos_dev_ioctl(mp3_VS1053_prvt_ctx.hSpiMaster, &spim_iocb);

	return MP3_VS1053_OK;
}

int mp3_VS1053_deselect_data(void)
{
	if (mp3_VS1053_prvt_ctx.SDI_CS_identifier == SPI_CHIP_SELECT_0)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_0;
	else if (mp3_VS1053_prvt_ctx.SDI_CS_identifier == SPI_CHIP_SELECT_1)
		spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_SS_1;
	spim_iocb.set.param = SPI_MASTER_SS_DISABLE;
	vos_dev_ioctl(mp3_VS1053_prvt_ctx.hSpiMaster, &spim_iocb);

	return MP3_VS1053_OK;
}
	
int mp3_VS1053_set_volume(unsigned char leftChannel, unsigned char rightChannel)
{
	unsigned short volume;
	unsigned short volume_set;
	
	volume = (leftChannel << 8) | rightChannel;
	
	mp3_VS1053_write_reg(SCI_VOL, volume);
	mp3_VS1053_read_reg(SCI_VOL, &volume_set);
	
	if (volume == volume_set)	
		return MP3_VS1053_OK;
	else
		return MP3_VS1053_ERROR;
}

int mp3_VS1053_write_reg(unsigned char reg_address, unsigned short data)
{
	unsigned char SCIData[4];
	unsigned short num_written;
	unsigned short num_read;

	SCIData[0] = 0x02;                   // opcode for writing
	SCIData[1] = reg_address;            // register's address
	SCIData[2] = ((data >> 8) & 0x00FF); // MSB of data
	SCIData[3] = (data & 0x00FF);        // LSB of data

	// Wair for DREQ to be HIGH, select SCI, send data, then deselect SCI
	mp3_VS1053_wait_for_dreq();
	mp3_VS1053_deselect_data();
	mp3_VS1053_select_control();
	//vos_gpio_write_port(GPIO_PORT_A, 0x01);
	vos_dev_write(mp3_VS1053_prvt_ctx.hSpiMaster, SCIData, 4, &num_written);
	mp3_VS1053_wait_for_dreq(); // it's better to wait for DREQ to be HIGH again before deselecting SCI
	//mp3_VS1053_deselect_control();
	//mp3_VS1053_select_data();
	vos_dev_read(mp3_VS1053_prvt_ctx.hSpiMaster, SCIData, num_written, &num_read);	
	mp3_VS1053_deselect_control();
	mp3_VS1053_select_data(); // Turn data back on!!
	
	return MP3_VS1053_OK;
}

int mp3_VS1053_read_reg(unsigned char reg_address, unsigned short *data)
{
	// Because Wiznet 5100 permanently drives the MISO line when its SEN pin is pulled HIGH, this operation is currently N.A.
	// For more information, refer to http://www.wiznet.co.kr/UpLoad_Files/ReferenceFiles/W5100_AN_SPI.pdf
	// Currently VS1053b's registers can only be read from the modified board
	unsigned char  SCIData[4];
	unsigned short num_written;
	unsigned short num_read;

	SCIData[0] = 0x03;                 // opcode for reading
	SCIData[1] = reg_address;          // register's address
	SCIData[2] = 0x00;                 // dummy data MSB
	SCIData[3] = 0x00;                 // dummy data LSB

	// Wait for DREQ to be HIGH, select SCI, send dummy data, then deselect SCI
	mp3_VS1053_wait_for_dreq();
	//mp3_VS1053_deselect_data();
	mp3_VS1053_select_control();
	//vos_gpio_write_port(GPIO_PORT_A, 0x01);
	vos_dev_write(mp3_VS1053_prvt_ctx.hSpiMaster, SCIData, 4, &num_written);
	mp3_VS1053_wait_for_dreq(); // it's better to wait for DREQ to be HIGH again before deselecting SCI
	mp3_VS1053_deselect_control();
	//mp3_VS1053_select_data();
	vos_dev_read(mp3_VS1053_prvt_ctx.hSpiMaster, SCIData, 4, &num_read);
	*data = (((SCIData[2] << 8) & 0xFF00) | SCIData[3]);
	//vos_gpio_write_port(GPIO_PORT_A, 0x08);

	return MP3_VS1053_OK;
}

unsigned short mp3_VS1053_read_ram(unsigned short ram_address)
{
	unsigned short ram_data;

    mp3_VS1053_write_reg(SCI_WRAMADDR, ram_address); 	// First set the offset	
    mp3_VS1053_read_reg(SCI_WRAM, &ram_data); 			// Then read the ram

	return ram_data;
}

// Write data from a file to the codec (playback)
unsigned char mp3_VS1053_write(unsigned char *buf, unsigned short num_to_write, unsigned short *num_written)
{	
	vos_dev_write(mp3_VS1053_prvt_ctx.hSpiMaster, buf, num_to_write, num_written);

	return MP3_VS1053_OK;
}		
	
int mp3_VS1053_read_register(unsigned char reg_address, unsigned short *data)
{
	unsigned char  SCIData[4];
	unsigned short num_written;
	unsigned short num_read;
	common_ioctl_cb_t spim_iocb;

	SCIData[0] = 0x03;                 // opcode for reading
	SCIData[1] = reg_address;          // register's address
	SCIData[2] = 0x00;                 // dummy data MSB
	SCIData[3] = 0x00;                 // dummy data LSB
	
	mp3_VS1053_wait_for_dreq();
	mp3_VS1053_deselect_data();
	mp3_VS1053_select_control();
	
	// Added to purge the FIFO buffer before a read...
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_RESET_FIFO_BUFFER;	
	vos_dev_ioctl(mp3_VS1053_prvt_ctx.hSpiMaster, &spim_iocb);	
	
	mp3_VS1053_write(SCIData, 4, &num_written);
	mp3_VS1053_wait_for_dreq(); // it's better to wait for DREQ to be HIGH again before deselecting SCI	
	vos_dev_read(mp3_VS1053_prvt_ctx.hSpiMaster, SCIData, 4, &num_read);
	*data = (((SCIData[2] << 8) & 0xFF00) | SCIData[3]);
	mp3_VS1053_deselect_control();
	mp3_VS1053_select_data();

	return MP3_VS1053_OK;
}

int mp3_VS1053_prepare_skip()
{
	unsigned short mp3Regval, num_bytes_written;
	unsigned char i;	
	unsigned char buffer[32], endFillByte7_0;;
		
	dataAvailable = 0;
	mp3_VS1053_wait_for_dreq();
	mp3_VS1053_read_register(SCI_STATUS, &mp3Regval);
	// If the bit is set, random access cannot be performed. Inform users to send a portion of the file and try again
	if (mp3Regval & MASK_SS_DO_NOT_JUMP) {
		return 0; // return here as we can't skip currently....
	}
	else {
		// Read extra parameter endFillByte (only the last 7 bits actually are needed)
		endFillByte7_0 = (unsigned char) mp3_VS1053_read_ram(param_endFillByte);
		vos_memset(buffer, (unsigned short)endFillByte7_0, 32);
		// Send at least 2048 bytes of endFillByte[7:0]
		for (i = 0; i < 64; i++) {
			mp3_VS1053_wait_for_dreq();
			mp3_VS1053_deselect_control();
			mp3_VS1053_select_data();
			mp3_VS1053_write(buffer, 32, &num_bytes_written);
			mp3_VS1053_deselect_data();
		}
	}
	mp3_VS1053_deselect_control();
	mp3_VS1053_select_data();
	return 1;
}

unsigned char mp3_VS1053_ioctl(void *cb)
{
	unsigned char retcode = MP3_VS1053_OK;
	unsigned short mp3Regval;
	unsigned char endFillByte7_0;
	unsigned short byteRate;
	unsigned short num_bytes_written;
	unsigned char times = 0;
	int i;

	// Cast our control block to our MP3 codec specific one
	mp3_VS1053_ioctl_cb *mp3_cb = (mp3_VS1053_ioctl_cb *) cb;
	// Determine the required IOCTL operation
	switch (mp3_cb->ioctl_code)
	{
		case VOS_IOCTL_MP3_VS1053_SETUP:
			mp3_VS1053_setup();
			break;
		case VOS_IOCTL_MP3_VS1053_ATTACH:
			retcode = mp3_VS1053_attach(mp3_cb->spi_master_handle, mp3_cb->XRESET_GPIO_port, mp3_cb->XRESET_GPIO_pin, mp3_cb->DREQ_GPIO_port, mp3_cb->DREQ_GPIO_pin, mp3_cb->SDI_CS_identifier, mp3_cb->SCI_CS_identifier);
			break;
		case VOS_IOCTL_MP3_VS1053_DETACH:
			mp3_VS1053_detach();
			break;
		case VOS_IOCTL_MP3_VS1053_HARD_RESET:
			// Toggle the xRST line to reset the hardware
			mp3_VS1053_hard_reset();
			break;
		case VOS_IOCTL_MP3_VS1053_SOFT_RESET:
			mp3_VS1053_soft_reset();
			break;
		case VOS_IOCTL_MP3_VS1053_SET_VOLUME:
			// Left volume in upper byte, right volume in lower byte
			mp3_VS1053_set_volume(mp3_cb->set.volume.left, mp3_cb->set.volume.right);
			break;
		case VOS_IOCTL_MP3_VS1053_SET_BASS:
			if (mp3_cb->set.bass.amplitude > 0x0F)
			{
				// Valid range for bass amplitude is 0 -> 15
				return MP3_VS1053_INVALID_PARAMETER;
			}
			else if ((mp3_cb->set.bass.freqLimit < 0x02) || (mp3_cb->set.bass.freqLimit > 0x0F))
			{
				// Valid range for bass lower frequency limit is 2 -> 15
				return MP3_VS1053_INVALID_PARAMETER;
			}
			else
			{
				mp3_VS1053_read_reg(SCI_BASS, &mp3Regval);
				mp3Regval &= 0xFF00; // reset bass bits
				mp3Regval |= (((mp3_cb->set.bass.amplitude << 4) & 0xF0) | mp3_cb->set.bass.freqLimit);
				mp3_VS1053_write_reg(SCI_BASS, mp3Regval);
			}
			break;
		case VOS_IOCTL_MP3_VS1053_SET_TREBLE:
			if ((mp3_cb->set.treble.amplitude < -0x08) || (mp3_cb->set.treble.amplitude > 0x07))
			{
				// Valid range for treble amplitude is -8 -> 7
				return MP3_VS1053_INVALID_PARAMETER;
			}
			else if (mp3_cb->set.treble.freqLimit > 0x0F)
			{
				// Valid range for treble lower frequency limit is 0 -> 15
				return MP3_VS1053_INVALID_PARAMETER;
			}
			else
			{
				mp3_VS1053_read_reg(SCI_BASS, &mp3Regval);
				mp3Regval &= 0x00FF; // reset treble bits
				mp3Regval |= (((mp3_cb->set.treble.amplitude << 12) & 0xF000) | (mp3_cb->set.treble.freqLimit << 8));
				mp3_VS1053_write_reg(SCI_BASS, mp3Regval);
			}
			break;
		case VOS_IOCTL_MP3_VS1053_FILE_SENT:
			// Read extra parameter endFillByte (only the last 7 bits actually are needed)
			endFillByte7_0 = (unsigned char) mp3_VS1053_read_ram(param_endFillByte);
			// Send at least 2052 bytes of endFillByte[7:0]
			for (i = 0; i < 65; i++)
			{
				mp3_VS1053_write(&endFillByte7_0, 32, &num_bytes_written);
			}
			// Set SCI_MODE bit SM_CANCEL
			mp3_VS1053_read_reg(SCI_MODE, &mp3Regval);
			mp3_VS1053_write_reg(SCI_MODE, mp3Regval | MASK_SM_CANCEL);
			do
			{
				// Send at least 32 bytes of endFillByte[7:0]
				mp3_VS1053_write(&endFillByte7_0, 32, &num_bytes_written);
				// Read SCI_MODE
				mp3_VS1053_read_reg(SCI_MODE, &mp3Regval);
				mp3Regval &= MASK_SM_CANCEL;
				times++; // increment the number of times the loop has run
				if (times < 64) // If less than 2048 bytes have been sent,
				{
					if (mp3Regval == MASK_SM_CANCEL)
						continue; // and SM_CANCEL is still set, loop again
					else
						break;
				}
				else // do a software reset, extremely rare case
				{
					mp3_VS1053_soft_reset();
					break;
				}
			} while (1);
			times = 0;
			break;
		case VOS_IOCTL_MP3_VS1053_PLAYBACK_CANCELLED:
			// Set SCI_MODE bit SM_CANCEL
			mp3_VS1053_read_reg(SCI_MODE, &mp3Regval);
			mp3_VS1053_write_reg(SCI_MODE, mp3Regval | MASK_SM_CANCEL);
			mp3_VS1053_read_reg(SCI_MODE, &mp3Regval);
			// Wait for one second, if SM_CANCEL hasn't cleared, do a software reset
			//vos_delay_msecs(1000);
			mp3_VS1053_read_reg(SCI_MODE, &mp3Regval);
			mp3Regval &= MASK_SM_CANCEL;
			if (mp3Regval == MASK_SM_CANCEL)
			{
				//mp3_VS1053_soft_reset();
			}
			else
			{
				// Read extra parameter endFillByte (only the last 7 bits actually are needed)
				endFillByte7_0 = (unsigned char) mp3_VS1053_read_ram(param_endFillByte);
				// Send 2052 bytes of endFillByte[7:0]
				for (i = 0; i < 64; i++)
				{
					mp3_VS1053_write(&endFillByte7_0, 32, &num_bytes_written);
				}
				mp3_VS1053_write(&endFillByte7_0, 4, &num_bytes_written);
			}
			break;
		case VOS_IOCTL_MP3_VS1053_PREPARE_FORWARD_REWIND:
			// Read SCI_STATUS bit SS_DO_NOT_JUMP
			mp3_VS1053_read_reg(SCI_STATUS, &mp3Regval);
			// If the bit is set, random access cannot be performed. Inform users to send a portion of the file and try again
			if (mp3Regval & MASK_SS_DO_NOT_JUMP)
			{
				retcode = (unsigned char) MP3_VS1053_DO_NOT_JUMP;
				break;
			}
			else
			{
				// Read extra parameter endFillByte (only the last 7 bits actually are needed)
				endFillByte7_0 = (unsigned char) mp3_VS1053_read_ram(param_endFillByte);
				// Send at least 2048 bytes of endFillByte[7:0]
				for (i = 0; i < 64; i++)
				{
					mp3_VS1053_write(&endFillByte7_0, 32, &num_bytes_written);
				}
			}
			// Now jump forward or backward in the file and continue sending the file.
			// This should be done in the application.
			break;
		case VOS_IOCTL_MP3_VS1053_GET_BYTE_RATE:
			// Read extra parameter byteRate (only the last 7 bits actually are needed)
			mp3_cb->get = mp3_VS1053_read_ram(param_byteRate);//param_byteRate);
			break;
		case VOS_IOCTL_MP3_VS1053_READ_REGISTER:
			mp3_VS1053_read_reg(mp3_cb->reg_addr, &(mp3_cb->get));
			break;
		case VOS_IOCTL_MP3_VS1053_WRITE_REGISTER:
			mp3_VS1053_write_reg(mp3_cb->reg_addr, mp3_cb->set.reg_val);
			break;
		default:
			retcode = (unsigned char) MP3_VS1053_UNSUPPORTED_IOCTL;
			break;
	}

	return retcode;
}

