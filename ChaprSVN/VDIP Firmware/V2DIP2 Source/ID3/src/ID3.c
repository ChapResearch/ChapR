/*
** Filename: ID3.c
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

#include "ID3.h"

//==========================================================================	
// FUNCTION DECLARATIONS	
//==========================================================================
unsigned char ParseID3_v1(FILE *, ID3_Tag *);
unsigned char ParseID3_v2_2(FILE *, ID3_Tag *);
unsigned char ParseID3_v2_3(FILE *, ID3_Tag *);
unsigned long GetFrameHeader(unsigned char *, unsigned char);
unsigned long CalculateTagSize(unsigned char *frame);
unsigned char BytesToString(FILE *, unsigned long, unsigned char *, unsigned char);

//==========================================================================
// FUNCTIONS DEFINITIONS
//==========================================================================		
unsigned char ReadID3Tag(FILE *file, ID3_Tag *ID3)
{		
	// returns the ID3 version of the file....
	// if the version is valid it will fill ID3 with data collected from the file...
	unsigned char status = 0;
	unsigned char version[4];
	vos_memset(ID3, 0x00, sizeof(ID3_Tag));
	fat_fileSeek(file, 0, FAT_SEEK_SET);
	fat_fileRead(file, 3, (char *)version, NULL, NULL);
	version[3] = 0;
	// fileName
	//strcpy(ID3->fileName, file->
	if (strcmp((const)version, "ID3") == 0) {
		unsigned short v2_vers;
		fat_fileRead(file, 2, (char *)&v2_vers, NULL, NULL);			
		
		if (v2_vers == 0x02) 
			status = ParseID3_v2_2(file, ID3);
		else if (v2_vers == 0x03 || v2_vers == 0x04)
			status = ParseID3_v2_3(file, ID3);
		else
			goto ERROR;
		
		if (status != 0x00)
			goto ERROR;
			
		return status;
	}
	else {
		// check to see if it's a version 1 ID3 tag...
		return ParseID3_v1(file, ID3);
	}
	
ERROR:		
	ID3 = NULL;
	return NOT_SUPPORTED;
}	
char *itoa(int value, char *string, int radix)
{
	int len = 0, i;
	char minus = 0;
	int temp;

	if (value < 0) {
		value = 0 - value;
		minus = 1;
		string[0] = '-';
	}

	temp = value;

	while (temp >= radix) {
		len++;
		temp = temp / radix;
	}

	for (i = len + minus; i >= minus; i--) {
		temp = value % radix;

		if (temp <= 9)
			temp = temp + '0';
		else
			temp = '?';

		*(string + i) = temp;
		value = value / radix;
	}
	return string;
}

unsigned char ParseID3_v1(FILE *file, ID3_Tag *ID3)
{	
	// Extended v1 tags are not supported...
	/*if (data == '+') {
		unsigned char data[3];
		fat_fileRead(file, 60, ID3->trackName, NULL, NULL);
		fat_fileRead(file, 60, ID3->artistName, NULL, NULL);
		fat_fileRead(file, 60, ID3->albumName, NULL, NULL);
		fat_fileSeek(file, 43, FAT_SEEK_CUR); // Skip speed, genre, start-time and end-time
		fat_fileRead(file, 3, (char *)&data, NULL, NULL);
		if (strcmp((const)data, "TAG") == 0) {
			fat_fileSeek(file, 125, FAT_SEEK_CUR);
			fat_fileRead(file, 1, ID3->trackNumber, NULL, NULL);
			ID3->trackNumber[1] = 0x00;
		}
	}
	else*/ 
	{
		unsigned char tag[4];		
		unsigned long filesize = fat_dirEntrySize(file);		
		
		tag[3] = 0x00;		
		fat_fileSeek(file, -128, FAT_SEEK_END); // Move back 128 bytes from the end of the file...
		fat_fileRead(file, 3, tag, NULL, NULL);
		if (strcmp(tag, "TAG\0") == 0) {
			unsigned char trkNo;
			fat_fileRead(file, 30, ID3->trackName, NULL, NULL);
			fat_fileRead(file, 30, ID3->artistName, NULL, NULL);
			fat_fileRead(file, 30, ID3->albumName, NULL, NULL);
			fat_fileSeek(file, -2, FAT_SEEK_END);
			fat_fileRead(file, 1, &trkNo, NULL, NULL);
			itoa(trkNo, ID3->trackNumber, 10);
			fat_fileSeek(file, 0, FAT_SEEK_SET);
		}
		else {
			fat_fileSeek(file, 0, FAT_SEEK_SET);
			return NOT_SUPPORTED;
		}
	}
	return 0;
}	

// 3 Bytes are used to represent the frame headers...
unsigned char ParseID3_v2_2(FILE *file, ID3_Tag *ID3)
{
	unsigned long ptr = 0;
	unsigned char flags;
	unsigned char header[4];
	unsigned long tagsize = 0;
	
	vos_memset(ID3, 0x00, sizeof(ID3_Tag));
	
	// Read the rest of the stuff from the ID3 header...
	fat_fileRead(file, 1, (char *)&flags, NULL, NULL);		
	fat_fileRead(file, 4, (char *)header, NULL, NULL);	
	tagsize = CalculateTagSize(header);
	fat_fileTell(file, &ptr); // how far through the file are we?
	
	do {
		unsigned long size = 0;
		unsigned char frameHeader[3];
		unsigned char frameSize[3];
		
		fat_fileRead(file, 3, (char *)frameHeader, NULL, NULL);
		fat_fileRead(file, 3, (char *)frameSize, NULL, NULL);
		size = (frameSize[0] << 16) | (frameSize[1] << 8) | (frameSize[2]);
		
		switch(GetFrameHeader(frameHeader, sizeof(frameHeader))) {		
		case ID3_FRAME(0, 'T', 'T', '2'): // Track name
			BytesToString(file, size, ID3->trackName, sizeof(ID3->trackName));
			break;
		case ID3_FRAME(0, 'T', 'P', '1'): // Artist name
			 BytesToString(file, size, ID3->artistName, sizeof(ID3->artistName));
			break;
		case ID3_FRAME(0, 'T', 'C', 'M'): // Composer
			BytesToString(file, size, ID3->composer, sizeof(ID3->composer));
			break;
		case ID3_FRAME(0, 'T', 'A', 'L'): // Album name
			BytesToString(file, size, ID3->albumName, sizeof(ID3->albumName));
			break;
		case ID3_FRAME(0, 'T', 'R', 'K'): // Track Number
			BytesToString(file, size, ID3->trackNumber, sizeof(ID3->trackNumber));
			break;
		default: // skip over these bytes as we don't care what they are...
			if (size == 0) // Can't have a frame of size zero so break...
				return 0;					
			fat_fileSeek(file, (int)size, FAT_SEEK_CUR);
			break;				
		} 
		ptr += (6 + size);  // Move the file ptr on...
	} while(ptr < tagsize);
	return 0;
}

// 4 Bytes are used to represent the frame headers...
unsigned char ParseID3_v2_3(FILE *file, ID3_Tag *ID3)
{
	unsigned long ptr = 0;
	unsigned char flags;
	unsigned char header[4];
	unsigned long tagsize = 0;
	
	vos_memset(ID3, 0x00, sizeof(ID3_Tag));
	
	// Read the rest of the stuff from the ID3 header...
	fat_fileRead(file, 1, (char *)&flags, NULL, NULL);		
	fat_fileRead(file, 4, (char *)header, NULL, NULL);	
	tagsize = CalculateTagSize(header);				
	if (flags & EXT_HEADER) { // TODO: check extended header support....
		unsigned long size = 0;
		unsigned char frameSize[4];
		fat_fileRead(file, 4, (char *)frameSize, NULL, NULL);	
		size = (frameSize[0] << 24) | (frameSize[1] << 16) | (frameSize[2] << 8) | (frameSize[3]);
		fat_fileSeek(file, (int)size, FAT_SEEK_CUR); // Skip over all this stuff, not interested...
	}
	fat_fileTell(file, &ptr); // how far through the file are we?
	
	do {			
		unsigned long size = 0;
		unsigned char frameHeader[4];
		unsigned char frameSize[4];
		unsigned char frameFlags[2];
		
		fat_fileRead(file, 4, (char *)frameHeader, NULL, NULL);
		fat_fileRead(file, 4, (char *)frameSize, NULL, NULL);
		size = (frameSize[0] << 24) | (frameSize[1] << 16) | (frameSize[2] << 8) | (frameSize[3]);
		fat_fileRead(file, 2, (char *)frameFlags, NULL, NULL);						
		ptr += (10 + size); // 10 == frameHeader[4] + frameSize[4] + frameFlags[2];			
		
		switch(GetFrameHeader(frameHeader, sizeof(frameHeader))) {
		case ID3_FRAME('T', 'I', 'T', '2'): // Song name
			BytesToString(file, size, ID3->trackName, sizeof(ID3->trackName));
			break;
		case ID3_FRAME('T', 'P', 'E', '1'): // Artist
			BytesToString(file, size, ID3->artistName, sizeof(ID3->artistName));
			break;
		case ID3_FRAME('T', 'C', 'O', 'M'): // Composer
			BytesToString(file, size, ID3->composer, sizeof(ID3->composer));
			break;
		case ID3_FRAME('T', 'A', 'L', 'B'): // Album
			BytesToString(file, size, ID3->albumName, sizeof(ID3->albumName));
			break;
		case ID3_FRAME('T', 'R', 'C', 'K'): // Track Number
			BytesToString(file, size, ID3->trackNumber, sizeof(ID3->trackNumber));
			break;
		default: // Read the size and skip over these bytes...				
			if (size == 0) // Can't have a frame of size zero so break...
				return 0;				
			fat_fileSeek(file, (int)ptr, FAT_SEEK_SET);
			break;			
		};
		
	} while (ptr < tagsize);
	
	return 0;
}

unsigned long GetFrameHeader(unsigned char *frame, unsigned char length)
{
	// Takes the frame header e.g. "TIT2" and converts it into an unsigned long...
	// Note: the length of the frame headers varies from v2.2 to v2.3 from 3 to 4...
	if (length == 4)
		return ID3_FRAME(frame[0], frame[1], frame[2], frame[3]);
	else if (length == 3)
		return ID3_FRAME(0, frame[0], frame[1], frame[2]);
}

unsigned long CalculateTagSize(unsigned char *frame)
{	
	return (frame[0] << 23) | (frame[1] << 15) | (frame[2] << 7) | (frame[3]);
}

unsigned char BytesToString(FILE *file, unsigned long size, unsigned char *string, unsigned char bufsize)
{
	unsigned char encoding, i, j = 0;
	
	fat_fileRead(file, 1, (char *)&encoding, NULL, NULL);
	if (encoding == 0x00 || encoding == 0x03) { // This is a normal c-style string.
		int mallocsize = size - 1;			
		unsigned char bytesToRead = size - 1;			
		
		if (bufsize < mallocsize)
			bytesToRead = bufsize - 1;
			
		fat_fileRead(file, (int)bytesToRead, (char *)string, NULL, NULL);
		string[bufsize - 1] = 0x00;
	} 
	else if (encoding == 0x01) { // Unicode!!!
		unsigned char byteOrder[2];
		unsigned char bytesToRead  = size - 3;
		unsigned char bytesToMalloc = size - 2;			
		unsigned char unicode[200];
					
		fat_fileRead(file, 2, (char *)byteOrder, NULL, NULL); // TODO: Byte order...
		
		if (sizeof(unicode) < bytesToMalloc)
			bytesToRead = bufsize - 1;
		fat_fileRead(file, (int)bytesToRead, (char *)unicode, NULL, NULL);			
					
		for (i = 0; i < bytesToRead; i += 2)
			string[j++] = unicode[i]; // TODO: Byte order...
			
		string[j] = 0x00; // Null terminate the string...
	}
	return string;
}

