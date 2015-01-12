/*
** VS1053_defs.h
**
** Copyright © 2009-2011 Future Technology Devices International Limited
**
** Company Confidential
**
** Header file containing definitions for VLSI VS1053b MP3 codec.
**
** Author: FTDI
** Project: Vinculum II
** Module: VS1053b MP3 Codec Driver
** Requires: VOS, SPIMaster, GPIO
** Comments:
**
** History:
**  1 – Initial version
**
*/

#ifndef __MP3_VS1053b_DEFS_H__
#define __MP3_VS1053b_DEFS_H__

// VS1053b	
int mp3_VS1053_read_register(unsigned char reg_address, unsigned short *data);
int mp3_VS1053_write_reg(unsigned char reg_address, unsigned short data);
unsigned short mp3_VS1053_read_ram(unsigned short ram_address);
unsigned char mp3_VS1053_write(unsigned char *buf, unsigned short num_to_write, unsigned short *num_written);
int mp3_VS1053_setup(void);
int	mp3_VS1053_select_data();
int mp3_VS1053_select_control();
int	mp3_VS1053_deselect_data();
int mp3_VS1053_deselect_control();
int	mp3_VS1053_wait_for_dreq();
int mp3_VS1053_soft_reset(void);
int mp3_VS1053_prepare_skip();

// SCI_MODE bits
#define BIT_SM_DIFF            	0           // Differential audio
#define MASK_SM_DIFF           	(1 << 0)    // 0: normal in-phase audio / 1: left channel inverted
#define BIT_SM_LAYER12	       	1           // Allow MPEG layers I & II
#define MASK_SM_LAYER12   		(1 << 1)    // 0: no / 1: yes
#define BIT_SM_RESET       		2           // Soft reset
#define MASK_SM_RESET          	(1 << 2)    // 0: no reset / 1: reset
#define BIT_SM_CANCEL        	3           // Cancel decoding current file
#define MASK_SM_CANCEL       	(1 << 3)    // 0: no / 1: yes
#define BIT_SM_EARSPEAKER_LO   	4           // EarSpeaker low setting
#define MASK_SM_EARSPEAKER_LO 	(1 << 4)    // 0: off / 1: active
#define BIT_SM_TESTS           	5           // Allow SDI tests
#define MASK_SM_TESTS          	(1 << 5)    // 0: not allowed / 1: allowed
#define BIT_SM_STREAM          	6           // Stream mode
#define MASK_SM_STREAM         	(1 << 6)    // 0: no / 1: yes
#define BIT_SM_EARSPEAKER_HI   	7           // EarSpeaker high setting
#define MASK_SM_EARSPEAKER_HI  	(1 << 7)    // 0: off / 1: active
#define BIT_SM_DACT_DCLK       	8           // DCLK active edge
#define MASK_SM_DACT_DCLK      	(1 << 8)    // 0: rising / 1: falling
#define BIT_SM_SDIORD_SDI      	9           // SDI bit order
#define MASK_SM_SDIORD_SDI     	(1 << 9)    // 0: MSb first / 1: MSb last
#define BIT_SM_SDISHARE        	10          // Share SPI chip select
#define MASK_SM_SDISHARE       	(1 << 10)   // 0: no / 1: yes
#define BIT_SM_SDINEW_VS1002   	11          // VS1002 native SPI modes
#define MASK_SM_SDINEW_VS1002  	(1 << 11)   // 0: no / 1: yes
#define BIT_SM_ADPCM_ADPCM     	12          // ADPCM recording active
#define MASK_SM_ADPCM_ADPCM    	(1 << 12)   // 0: no / 1: yes
#define BIT_SM_UNKNOWN		  	13          // -
#define MASK_SM_UNKNOWN		 	(1 << 13)   // 0 right / 1: wrong
#define BIT_SM_LINE1   			14          // MIC / LINE1 selector
#define MASK_SM_LINE1  			(1 << 14)   // 0: MICP / 1: LINE1
#define BIT_SM_CLK_RANGE		15          // Input clock range
#define MASK_SM_CLK_RANGE		(1 << 15)   // 0: 12..13MHz / 1: 24..26MHz

// VS1053 control register addresses
#define SCI_MODE               	0           // Mode control
#define SCI_STATUS             	1           // Status of VS1053b
#define SCI_BASS               	2           // Built-in bass/treble enhancer
#define SCI_CLOCKF             	3           // Clock freq + multiplier
#define SCI_DECODE_TIME        	4           // Decode time in seconds
#define SCI_AUDATA             	5           // Misc. audio data
#define SCI_WRAM	           	6           // RAM write/read
#define SCI_WRAMADDR           	7           // Base address for RAM write/read
#define SCI_HDAT0              	8           // Stream header data 0
#define SCI_HDAT1              	9           // Stream header data 1
#define SCI_AIADDR             	10          // Start address of application
#define SCI_VOL                	11          // Volume control
#define SCI_AICTRL0            	12          // Application control register 0
#define SCI_AICTRL1            	13          // Application control register 1
#define SCI_AICTRL2         	14          // Application control register 2
#define SCI_AICTRL3            	15          // Application control register 3

// SCI_STATUS bits
#define BIT_SS_DO_NOT_JUMP		15			// Header in decode, do not fast forward/rewind
#define MASK_SS_DO_NOT_JUMP		(1 << 15)
#define BIT_SS_SWING_HI			14			// Set swing to +0 dB, +0.5 dB, ..., or +3.5 dB
#define BIT_SS_SWING_LO			12			// Set swing to +0 dB, +0.5 dB, ..., or +3.5 dB
#define MASK_SS_SWING			(7 << 12)
#define BIT_SS_VCM_OVERLOAD		11			// GBUF overload indicator, '1' = overload
#define MASK_SS_VCM_OVERLOAD	(1 << 11)
#define BIT_SS_VCM_DISABLE		10			// GBUF overload detection, '1' = disable
#define MASK_SS_VCM_DISABLE		(1 << 10)
// Bit 8, 9 are resevered
#define BIT_SS_VER_HI          	7           // Version
#define BIT_SS_VER_LO          	4           // Version
#define MASK_SS_VER            	(15 << 4)
#define BIT_SS_APDOWN2         	3           // Analog driver powerdown
#define MASK_SS_APDOWN2        	(1 << 3)
#define BIT_SS_APDOWN1         	2           // Analog internal powerdown
#define MASK_SS_APDOWN1        	(1 << 2)
#define BIT_SS_AD_CLOCK        	1           // AD clock reset, '0' = 6 MHz, '1' = 3 MHz
#define MASK_SS_AD_CLOCK        (1 << 1)
#define BIT_SS_REFERENCE_SEL   	0           // Reference voltage selection, '0' = 1.23 V, '1' = 1.65 V
#define MASK_SS_REFERENCE_SEL 	(1 << 0)

// SCI_BASS bits
#define BIT_ST_AMPLITUDE_HI    15          // Treble Control in 1.5 dB steps (-8..7, 0 = off)
#define BIT_ST_AMPLITUDE_LO    12          // Treble Control in 1.5 dB steps (-8..7, 0 = off)
#define MASK_ST_AMPLITUDE      (15 << 12)  // Treble Control in 1.5 dB steps (-8..7, 0 = off)
#define BIT_ST_FREQLIMIT_HI    11          // Lower limit frequency in 1000 Hz steps (0..15)
#define BIT_ST_FREQLIMIT_LO    8           // Lower limit frequency in 1000 Hz steps (0..15)
#define MASK_ST_FREQLIMIT      (15 << 8)   // Lower limit frequency in 1000 Hz steps (0..15)
#define BIT_SB_AMPLITUDE_HI    7           // Bass Enhancement in 1 dB steps (0..15, 0 = off)
#define BIT_SB_AMPLITUDE_LO    4           // Bass Enhancement in 1 dB steps (0..15, 0 = off)
#define MASK_SB_AMPLITUDE      (15 << 4)   // Bass Enhancement in 1 dB steps (0..15, 0 = off)
#define BIT_SB_FREQLIMIT_HI    3           // Lower limit frequency in 10 Hz steps (2..15)
#define BIT_SB_FREQLIMIT_LO    0           // Lower limit frequency in 10 Hz steps (2..15)
#define MASK_SB_FREQLIMIT      (15 << 0)   // Lower limit frequency in 10 Hz steps (2..15)

// SCI_CLOCKF bits
#define BIT_SC_MULT_HI         15          // Clock multiplier
#define BIT_SC_MULT_LO         13          // Clock multiplier
#define MASK_SC_MULT           (7 << 13)   // Clock multiplier
#define BIT_SC_ADD_HI          12          // Allowed multiplier addition
#define BIT_SC_ADD_LO          11          // Allowed multiplier addition
#define MASK_SC_ADD            (3 << 11)   // Allowed multiplier addition
#define BIT_SC_FREQ_HI         10          // Clock frequency
#define BIT_SC_FREQ_LO         0           // Clock frequency
#define MASK_SC_FREQ           (2047 << 0) // Clock frequency

// Extra Parameter in X memory (refer to section 9.11 of the datasheet)
#define param_playSpeed                              0x1E04
#define param_byteRate                               0x1E05
#define param_endFillByte                            0x1E06

/*
SC MULT MASK CLKI
0 0x0000 XTALI
1 0x2000 XTALIx1:5
2 0x4000 XTALIx2:0
3 0x6000 XTALIx2:5
4 0x8000 XTALIx3:0
5 0xa000 XTALIx3:5
6 0xc000 XTALIx4:0
7 0xe000 XTALIx4:5


SC ADD MASK Multiplier addition
0 0x0000 No modification is allowed
1 0x0800 0.5x
2 0x1000 1.0x
3 0x1800 1.5x
*/

#define MASK_VS1053_RESET      0x01
#define MASK_VS1053_DREQ       0x02

typedef struct _mp3_prvt_context
{
	VOS_HANDLE 	hSpiMaster;
	unsigned char XRESET_GPIO_port;
	unsigned char XRESET_GPIO_pin;
	unsigned char DREQ_GPIO_port;
	unsigned char DREQ_GPIO_pin;
	unsigned char SDI_CS_identifier;
	unsigned char SCI_CS_identifier;
} mp3_VS1053_prvt_context;

// VS1053 read function
unsigned char mp3_VS1053_read(
	unsigned char *buf,
	unsigned short num_to_read,
	unsigned short *num_read
	);

// VS1053 write function
unsigned char mp3_VS1053_write(
	unsigned char *buf,
	unsigned short num_to_write,
	unsigned short *num_written
	);

// VS1053 IOCTL function
unsigned char mp3_VS1053_ioctl(
	void *ioctl_cb
	);

#endif /* __MP3_VS1053b_DEFS_H__ */

