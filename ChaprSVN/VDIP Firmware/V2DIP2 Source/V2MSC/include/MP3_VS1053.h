/*
** MP3_VS1053.h
**
** Copyright © 2009-2011 Future Technology Devices International Limited
**
** Company Confidential
**
** Header file containing definitions for VLSI VS1053 MP3 codec.
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

#ifndef __MP3_VS1053_H__
#define __MP3_VS1053_H__

#include "vos.h"
#define MP3_VS1053_VERSION_STRING "2.0.0"

// VS1053 IOCTL definitions
#define VOS_IOCTL_MP3_VS1053_ATTACH						1 // Attach (SPI Master driver) and (XRESET & DREQ pin information) to the MP3 driver
#define VOS_IOCTL_MP3_VS1053_DETACH						2 // Detach SPI Master driver from the MP3 driver
#define VOS_IOCTL_MP3_VS1053_HARD_RESET    				3 // hardware reset
#define VOS_IOCTL_MP3_VS1053_SET_VOLUME 				4 // set volume
#define VOS_IOCTL_MP3_VS1053_SET_BASS   				5 // set bass
#define VOS_IOCTL_MP3_VS1053_SET_TREBLE 				6 // set treble
#define VOS_IOCTL_MP3_VS1053_SOFT_RESET 				7 // software reset (between songs)
#define VOS_IOCTL_MP3_VS1053_SETUP						8 // initialize the MP3 decoder chip
#define VOS_IOCTL_MP3_VS1053_FILE_SENT					9 // signal the driver to do post processing after the whole file has been sent
#define VOS_IOCTL_MP3_VS1053_PLAYBACK_CANCELLED			10 // signal the driver to perform necessary steps to cancel the playback process
#define VOS_IOCTL_MP3_VS1053_PLAYBACK_CANCELED			VOS_IOCTL_MP3_VS1053_PLAYBACK_CANCELLED // deprecated
#define VOS_IOCTL_MP3_VS1053_PREPARE_FORWARD_REWIND		11 // prepare the chip for forwarding / rewinding in a file
#define VOS_IOCTL_MP3_VS1053_GET_BYTE_RATE				12 // get the average byterate of a song
#define VOS_IOCTL_MP3_VS1053_READ_REGISTER				13 // read a VS1053's register
#define VOS_IOCTL_MP3_VS1053_WRITE_REGISTER				14 // write to a VS1053's register

// GPIO port definitions
#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4

// Chip select indentifiers for use in attch IOCTL
#define SPI_CHIP_SELECT_0								0 // Use SPI Master slave select 0
#define SPI_CHIP_SELECT_1								1 // Use SPI Master slave select 1

enum MP3_VS1053_STATUS
{
	MP3_VS1053_OK = 0,
	MP3_VS1053_INVALID_PARAMETER,
	MP3_VS1053_UNSUPPORTED_IOCTL,
	MP3_VS1053_DO_NOT_JUMP,
	MP3_VS1053_ERROR
};

typedef struct _mp3_VS1053_vol {
	unsigned char left;
	unsigned char right;
} mp3_VS1053_vol;

typedef struct _mp3_VS1053_bas {
	unsigned char amplitude;
	unsigned char freqLimit;
} mp3_VS1053_bas;

typedef struct _mp3_VS1053_tre {
	signed char   amplitude;
	unsigned char freqLimit;
} mp3_VS1053_tre;

// VS1053 control block for use with IOCTL function
typedef struct _mp3_VS1053_ioctl_cb
{
	unsigned char ioctl_code;
	VOS_HANDLE spi_master_handle;
	unsigned char XRESET_GPIO_port;
	unsigned char XRESET_GPIO_pin;
	unsigned char DREQ_GPIO_port;
	unsigned char DREQ_GPIO_pin;
	unsigned char SDI_CS_identifier;
	unsigned char SCI_CS_identifier;
	unsigned char reg_addr;
	union
	{
		mp3_VS1053_vol               volume;
		mp3_VS1053_bas               bass;
		mp3_VS1053_tre               treble;
		unsigned short 				 reg_val;
	} set;
	unsigned short get;
} mp3_VS1053_ioctl_cb;

// Context for VS1053
typedef struct _mp3_VS1053_context_t
{
	unsigned char buffer_size;
} mp3_VS1053_context_t;

void mp3_VS1053_init(unsigned char mp3VS1053DevNum, mp3_VS1053_context_t *mp3Context);

#endif /*__MP3_VS1053_H__*/
