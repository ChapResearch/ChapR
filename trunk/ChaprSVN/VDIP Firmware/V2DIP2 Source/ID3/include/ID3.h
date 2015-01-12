/*
** Filename: ID3.h
**
** Automatically created by Application Wizard 2.0.0
** 
** Part of solution VMusic3 in project ID3
**
** Comments: 
**
** Important: Sections between markers "FTDI:S*" and "FTDI:E*" will be overwritten by
** the Application Wizard
*/

#ifndef _ID3_H_
#define _ID3_H_


//==========================================================================	
// HEADERS
//==========================================================================
#include "vos.h"

#include "USB.h"
#include "USBHost.h"
#include "FAT.h"
#include "msi.h"
#include "BOMS.h"
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "string.h"

#define ID3_FRAME(A, B, C, D) ((A << 24) | (B << 16) | (C << 8) | D)
#define EXT_HEADER (1 << 6)

enum ID3Version
{
	ID3_V1,
	ID3_V2,
	NOT_SUPPORTED = 0xFF,
};

typedef struct _ID3_Tag
{
	unsigned char fileName[15];	// e.g. song2.mp3
	unsigned char trackName[100];
	unsigned char albumName[100];
	unsigned char artistName[100];
	unsigned char composer[100];
	unsigned char trackNumber[7];
} ID3_Tag;	
	
//==========================================================================	
// FUNCTION DECLARATIONS	
//==========================================================================
unsigned char ReadID3Tag(FILE *file, ID3_Tag *ID3);

#endif
