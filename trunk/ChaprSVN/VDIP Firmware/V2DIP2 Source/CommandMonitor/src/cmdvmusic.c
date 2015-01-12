/*
** cmdvmusic.c
**
** Copyright © 2010-2012 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2MSC Commands
** in the Command Interpreter
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments:
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"
#include "FAT.h"
#include "stdlib.h"
#include "ID3.h"
#include "MP3_VS1053.h"
#include "MP3_VS1053_defs.h"
#include "v2msc.h"
#include "SPIMaster.h"

#include "monitor.h"

#define IDLE	 	0b00000000				// When no files are playing...
#define SINGLE	 	0b00000001				// When playing a single file...
#define REPEAT		0b00000010				// When repeating play of track(s)...
#define ALL			0b00000100				// When playing all tracks...
#define RANDOM		0b00001000				// When randomly playing all songs...

#define NO_SKIP		0b00000000				// No skip
#define SKIP_FWD	0b00000001				// Skips one track forward
#define SKIP_BWD	0b00000010				// Skips one track back
#define SKIP_START	0b00000100				// If duration is more than 1 second then skip to start
#define FFWD		0b00001000				// Skips the song forward 5 seconds
#define REWD		0b00010000				// Skips the song back 5 seconds
#define SKIP_DIR	0b00100000				// Skips the song forward one directory

#define FRAME_SYNC	0xFFF0					// At the start of each mp3 frame header...

#define BUFFER_SIZE 1024

// MP3 Frame Header
#define MPEG_V1		0b00011000
#define MPEG_V2		0b00010000
#define LAYER_III	0b00000010
#define LAYER_II	0b00000100
#define LAYER_I		0b00000110
#define PADDING_BIT	0b00000010				// If this is set then not protected...

unsigned char STEP_OUT[11] = "..         ";	// Steps out from the current directory...
unsigned char MON_TIME_STR[3] = "T \0";
unsigned char MP3_FILE[4] = "MP3\0";
unsigned char WMA_FILE[4] = "WMA\0";

long offset_file, meta_data_offset;
unsigned char endFillByte7_0;
unsigned short directoryLevel;

//=====================================================================
// EXTERNS
//=====================================================================
extern vos_mutex_t mPlayState;
extern vos_mutex_t mPlaying;
extern vos_mutex_t mMP3File;
extern vos_semaphore_t sPaused;
extern vos_semaphore_t sPlayAll;
extern vos_semaphore_t sPlaying;
extern fat_context fatContext;
extern VOS_HANDLE hMP3;
extern VOS_HANDLE hSPIMaster;
extern unsigned char curState;
extern unsigned char fileState;

// TODO: probably move this?
#define FILE_STATE_NOT_OPEN	  0
#define FILE_STATE_OPEN_READ  1
#define FILE_STATE_OPEN_WRITE 2

unsigned char VOLUME = 0xFE;
unsigned char STATE;
	
					   /*|    MPEG 1    | MPEG 2 |
						 |LI  LII  LIII |L1  LIII|*/
int BIT_RATES[16][5] = 	{{0,   0,   0,   0,   0  },
						 {32,  32,  32,  32,  8  },
						 {64,  48,  40,  48,  16 },
						 {96,  56,  48,  56,  24 },
						 {128, 64,  56,  64,  32 },
						 {160, 80,  64,  80,  40 },
						 {192, 96,  80,  96,  48 },
						 {224, 112, 96,  112, 56 },
						 {256, 128, 112, 128, 64 },
						 {288, 160, 128, 144, 80 },
						 {320, 192, 160, 160, 96 },
						 {352, 224, 192, 176, 112},
						 {384, 256, 224, 192, 128},
						 {416, 320, 256, 224, 144},
						 {448, 384, 320, 256, 160},
						 {0,   0,   0,   0,   0  }};
						
int SAMPLE_RATES[4][3] = {{44100, 22050, 11025},
						  {48000, 24000, 12000},
						  {32000, 16000, 8000 },
						  {0,     0,     0,   }};


//=====================================================================
// ENUMS
//=====================================================================
	
//=====================================================================
// FUNCTION DECLARATIONS
//=====================================================================
void MP3();
void Play();
void PlayTrackMGMT(unsigned char);
void PlayAllTrackMGMT();	
unsigned char StopTrack();
void RecurseFileSystem();
unsigned char IsMusicFile(unsigned char *fileName);

//=====================================================================
// GLOBALS
//=====================================================================	
file_context_t file;
	
unsigned char playState = STOPPED;
unsigned char skipState = NO_SKIP;
	
// Determines how much data is in our buffer to send to the MP3 decoder
unsigned long dataAvailable;
unsigned char buffer[BUFFER_SIZE];
	
vos_semaphore_t sDataAvailable;

unsigned long counter; // used for the random function...
int elapsed;
	
unsigned long debug_seek;
//=====================================================================
// HELPER FUNCTIONS
//=====================================================================
// Calculates the number of frames 
unsigned int CalculateFrameSkip()
{
	unsigned long skip_time = 0;
	unsigned long file_pos = 0;
	
	int elapsed_local = elapsed;
	if (elapsed_local == 0)
		elapsed_local += 1;
		
	fat_fileTell(&file, &file_pos);
	skip_time = ((file_pos - meta_data_offset)/elapsed_local);
	skip_time *= 5;
	
	return skip_time;
}
	
void CancelPlayback()
{
	unsigned short mp3Regval;
	
	vos_lock_mutex(&mMP3File);
	mp3_VS1053_read_register(SCI_MODE, &mp3Regval);
	mp3_VS1053_write_reg(SCI_MODE, mp3Regval | MASK_SM_CANCEL);
	vos_unlock_mutex(&mMP3File);
}
	
unsigned char findFile(file_context_t *fileFind)
{
	// filename specified:
	monReadFileName(1);	
	monValidateFileName(1);
	if (IsMusicFile(param1Data)) {
		return fat_dirTableFind(fatContext, fileFind, (char *) param1Data);
	}
	else {
		return MON_ERROR_INVALID_FILENAME;
	}
}
	
unsigned char StopTrack()
{
	common_ioctl_cb_t spim_iocb;
	mp3_VS1053_ioctl_cb mp3_iocb;
	
	spim_iocb.ioctl_code = VOS_IOCTL_SPI_MASTER_RESET_FIFO_BUFFER;
	vos_dev_ioctl(hSPIMaster, &spim_iocb);
	
	vos_lock_mutex(&mPlayState);
	
	dataAvailable = 0;
	
	if (playState == PAUSED) {		
		playState = STOPPED;
		vos_unlock_mutex(&mPlayState);
		vos_signal_semaphore(&sPaused); // Return from the thread...
		return MON_SUCCESS;
	}
	else if (playState == PLAYING) {
		playState = STOPPED;
	}		
	skipState = NO_SKIP;
	vos_unlock_mutex(&mPlayState);	
	
	return MON_SUCCESS;
}

int strcmp(char *a, char *b)
{
	int i;

	for (i = 0; a[i] == b[i]; i++)
		if (a[i] == '\0')
			return 0;
	return a[i] - b[i];
}
	
unsigned char *FindPreviousFile(unsigned char *filename)
{
	unsigned char previousFile[12]; // terminate the strings so that we can do a strcmp...
	unsigned char currentFile[12];
	vos_memset(currentFile, 0x00, 12);
	vos_memset(previousFile, 0x00, 12);
	
	if (filename[0] == 0x00)
		return NULL;
	
	if (fat_dirTableFindFirst(fatContext, &file) != FAT_OK) {
		goto CLEANUP;
	}
	
	vos_memcpy(previousFile, file.dirEntry, 11);	
	do 
	{
		vos_memcpy(currentFile, file.dirEntry, 11);
		if (strcmp((const char *)currentFile, (const char *)filename) == 0) {
			vos_memcpy(param1Data, previousFile, 11); // vos_memcopy
			return previousFile;
		}
		else {
			vos_memcpy(previousFile, currentFile, 11);
		}		
	} while (fat_dirTableFindNext(fatContext, &file) == FAT_OK);
	
CLEANUP:	
	return NULL;
}
	
void Mon_Write_Time()
{	
	monWriteString((char *)MON_TIME_STR);
	monAddNumberToConsole((unsigned char *)&elapsed, 2);
	monWriteString("\r");
}

void Mon_Display_ID3()
{
	ID3_Tag ID3;
	
	if (ReadID3Tag(&file, &ID3) != NOT_SUPPORTED ) {
		monWriteString("Playing ");
		monWriteFileName((char *)&file);
		monWriteString("\r");
		monWriteString((char*)ID3.trackNumber);
		monWriteString("\r");
		monWriteString((char*)ID3.trackName);
		monWriteString("\r");
		monWriteString((char*)ID3.artistName);
		monWriteString("\r");
		monWriteString((char*)ID3.albumName);
		monWriteString("\r");
		monWriteString((char*)ID3.composer);
		monWriteString("\r");
	}
	else { // NOT_SUPPORTED
		monWriteString("Playing ");
		monWriteFileName((char *)&file);
		monWriteString("\r");
	}	
}
	
uint32 GenerateRandomNumber()
{
	long seed = vos_get_kernel_clock();
	seed = seed * 0x343fd + 0x269EC3;
	return (seed >> 0x10) & 0x2F;
}
	
unsigned char IsMusicFile(unsigned char *fileName)
{
	unsigned char extension[4];
	char i;
	
	for (i = 0; i < 4; extension[i] = fileName[8 + i], i++)
		;
	extension[3] = 0x00;
	
	// toupper
	for (i = 0; i < 4; i++) {
		if (extension[i] >= 'a' && extension[i] <= 'z') 
			extension[i] -= 0x20;
	}
	
	if ((strcmp((unsigned char *)extension, (unsigned char *)MP3_FILE) == 0) || 
		(strcmp((unsigned char *)extension, (unsigned char *)WMA_FILE) == 0)) {
		return TRUE;
	}
	return FALSE;
}
	
unsigned char DeviceConnected()
{
	if (curState != STATE_DISK) {
		monWriteString("No Disk\r");
		return FALSE;
	}
	return TRUE;
}
	
//=====================================================================
// VMUSIC COMMANDS
//=====================================================================	
/*
** cmd_pause
**
** Pauses/restarts the currently playing file.
**
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/		
unsigned char cmd_pause(char on)
{
	vos_lock_mutex(&mPlayState);
	if (on) 
	{
		if (playState == PLAYING)
		{
			playState = PAUSED_E;
		}
	}
	else
	{
		if (playState == PAUSED_E)
		{
			playState = PLAYING;
			vos_signal_semaphore(&sPaused);
		}
	}
	vos_unlock_mutex(&mPlayState);

	return MON_NO_CMD;
}
		
/*
** cmd_vpf
**
** Play file.
**
** Parameters: param1: file in the current directory to play
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/	
unsigned char cmd_vpf()
{
	file_context_t fileFind;
	if (!DeviceConnected())
		return MON_NO_CMD;
	if (findFile(&fileFind) == FAT_OK && IsMusicFile(fileFind.dirEntry)) {
		vos_memcpy(param1Data, fileFind.dirEntry, 11);
		StopTrack();				 // Stop the currently playing track...
		vos_lock_mutex(&mPlayState); // Wait until the last cmd has finished before issuing a new one...		
		STATE = SINGLE; 			 // Set the new state for this CMD...		
		vos_unlock_mutex(&mPlayState);
		return MON_NO_CMD;
	}
	else {
		monWriteString("Stopped\r");
		monWriteString("Filename Invalid\r");
		return MON_NO_CMD;
	}
}

/*
** cmd_vrf
**
** Play file repeatedly.
**
** Parameters: param1: file in the current directory to play repeatedly
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/
unsigned char cmd_vrf()
{
	file_context_t fileFind;
	if (!DeviceConnected())
		return MON_NO_CMD;
	if (findFile(&fileFind) == FAT_OK && IsMusicFile(fileFind.dirEntry)) {
		vos_memcpy(param1Data, fileFind.dirEntry, 11);
		StopTrack(); 				 // Stop the currently playing track...
		vos_lock_mutex(&mPlayState); // Wait until the last cmd has finished before issuing a new one...		
		STATE = REPEAT | SINGLE; 	 // Set the new state for this CMD...		
		vos_unlock_mutex(&mPlayState);
		return MON_NO_CMD;
	}
	else {
		monWriteString("Stopped\r");
		monWriteString("Filename Invalid\r");
		return MON_NO_CMD;
	}
}	

/*
** cmd_v3a
**
** Play all files in this directory and subsequent directories
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/
unsigned char cmd_v3a()
{
	if (!DeviceConnected())
		return MON_NO_CMD;
	StopTrack(); 					// Attempt to stop any previously playing track...
	vos_lock_mutex(&mPlayState); 	// Wait until the last cmd has finished before issuing a new one...
	STATE = ALL;
	vos_unlock_mutex(&mPlayState);
	return MON_NO_CMD;
}	

/*
** cmd_vra
**
** Play all files in this directory and subsequent directories repeatedly
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/	
unsigned char cmd_vra()
{
	if (!DeviceConnected())
		return MON_NO_CMD;
	StopTrack();
	vos_lock_mutex(&mPlayState); // Wait until the last cmd has finished before issuing a new one...
	STATE = REPEAT | ALL; // Set the new state for this CMD...	
	vos_unlock_mutex(&mPlayState);
	return MON_NO_CMD;
}

/*
** cmd_vrr
**
** Plays random files in all directories repeatedly
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if file not found
*/	
unsigned char cmd_vrr()
{
	if (!DeviceConnected())
		return MON_NO_CMD;
	StopTrack();
	vos_lock_mutex(&mPlayState); // Wait until the last cmd has finished before issuing a new one...	
	STATE = RANDOM | REPEAT;
	vos_unlock_mutex(&mPlayState);	
	return MON_NO_CMD;
}

/*
** cmd_vst
**
** Stops playback
**
** Parameters: none
** Returns: MON_SUCCESS
*/
unsigned char cmd_vst()
{		
	if (!DeviceConnected())
		return MON_NO_CMD;
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	STATE = IDLE;
	if (playState == PAUSED) {
		playState = STOP_VST;
		vos_unlock_mutex(&mPlayState);
		vos_signal_semaphore(&sPaused); // Return from the thread...
		return MON_NO_CMD;
	}
	else if (playState == PLAYING) {
		playState = STOP_VST;
	}		
	else if (playState == STOPPED 
		|| playState == STOP_VST) {
		monPrompt();
		playState = STOPPED;
	}
	skipState = NO_SKIP;
	vos_unlock_mutex(&mPlayState);	
	
	return MON_NO_CMD;	 // TODO: check this is correct!
}

/*
** cmd_vp
**
** Pauses playback
**
** Parameters: none
** Returns: MON_SUCCESS
*/
unsigned char cmd_vp()
{
	vos_lock_mutex(&mPlayState);
	if (playState == PAUSED) {
		if (curState != STATE_DISK) {
			monPrompt();
			monWriteString("Stopped\r");
			monWriteString("Command Failed\r");
			monWriteString("No Disk\r");
			return MON_NO_CMD;
		}
		else {			
			playState = PLAYING;
			vos_unlock_mutex(&mPlayState);
			vos_signal_semaphore(&sPaused);
		}
	}
	else if (playState == PLAYING) {
		playState = PAUSED;
		vos_unlock_mutex(&mPlayState);
	}
	else if (playState == STOPPED) {				
		if (!DeviceConnected())
			return MON_NO_CMD;			
		vos_unlock_mutex(&mPlayState);	
		monPrompt();
	}
	
	return MON_NO_CMD;
}

/*
** cmd_vsf
**
** Skips forward a track
**
** Parameters: none
** Returns: MON_SUCCESS
*/
unsigned char cmd_vsf()
{
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	skipState = SKIP_FWD;
	vos_unlock_mutex(&mPlayState);
	return MON_SUCCESS;
}

/*
** cmd_vsb
**
** Skips back a track
**
** Parameters: none
** Returns: MON_SUCCESS
*/
unsigned char cmd_vsb()
{
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	skipState = SKIP_BWD;
	vos_unlock_mutex(&mPlayState);
	return MON_SUCCESS;
}

/*
** cmd_vsd
**
** Skips forward a directory
**
** Parameters: none
** Returns: MON_SUCCESS
*/	
unsigned char cmd_vsd()
{
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	skipState = SKIP_DIR;
	vos_unlock_mutex(&mPlayState);
	return MON_SUCCESS;
}

/*
** cmd_vf
**
** Skips the track forward 5 secs
**
** Parameters: none
** Returns: MON_SUCCESS
*/		
unsigned char cmd_vf()
{	
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	skipState = FFWD;
	vos_unlock_mutex(&mPlayState);
	return MON_SUCCESS;
}

/*
** cmd_vb
**
** Skips the track back 5 secs
**
** Parameters: none
** Returns: MON_SUCCESS
*/			
unsigned char cmd_vb()
{	
	vos_lock_mutex(&mPlayState);
	dataAvailable = 0;
	skipState = REWD;
	vos_unlock_mutex(&mPlayState);
	return MON_SUCCESS;
}

/*
** cmd_vrd
**
** Reads a value from the specified decoder register
**
** Parameters: unsigned char register_address
** Returns: MON_SUCCESS
*/	
unsigned char cmd_vrd()
{
	unsigned short data;
	unsigned char regAddr = 0;
	
	monReadNumber(1, 1);
	monValidateNumber(1, 1);
	monReadCr();
	
	vos_lock_mutex(&mMP3File);
	mp3_VS1053_read_register(param1, &data);
	vos_unlock_mutex(&mMP3File);
	monAddNumberToConsole(&data, 2);
	
	return MON_SUCCESS;
}

/*
** cmd_vwr
**
** Writes a value to the specified decoder register
**
** Parameters: 	unsigned char register_address
** 				unsigned short register_value
** Returns: MON_SUCCESS
*/	
unsigned char cmd_vwr()
{
	unsigned char regAddr = 0;
	unsigned short regVal = 0;
	
	monReadNumber(1, 3);
	monValidateNumber(1, 3);
	monReadCr();
	
	vos_lock_mutex(&mMP3File);
	mp3_VS1053_write_reg((param1 >> 16) & 0xFF, param1 & 0xFFFF);
	vos_unlock_mutex(&mMP3File);
	
	return MON_SUCCESS;
}

/*
** cmd_vsv
**
** Sets playback volume
**
** Parameters: byte playback volume
** Returns: MON_SUCCESS
*/
unsigned char cmd_vsv()
{
	mp3_VS1053_ioctl_cb mp3_iocb;
	unsigned char charRead = 0;
	unsigned short data;
	
	monReadNumber(1, 1);
	monValidateNumber(1, 1);
	monReadCr();
	
	mp3_iocb.ioctl_code = VOS_IOCTL_MP3_VS1053_SET_VOLUME;
	mp3_iocb.set.volume.left = param1;
	mp3_iocb.set.volume.right = param1;
	vos_lock_mutex(&mMP3File);
	vos_dev_ioctl(hMP3, &mp3_iocb);
	vos_unlock_mutex(&mMP3File);
	
	return MON_SUCCESS;
}

//=====================================================================
// VMUSIC THREADS
//=====================================================================
unsigned char PlayRandomTrack(unsigned long randomNumber)
{
	file_context_t fileFind;
	
	if (fat_dirTableFindFirst(fatContext, &fileFind) != FAT_OK) {
		return;
	}
	
	while (1) {	
		if (fat_dirEntryIsValid(&fileFind)) {
			if (fat_dirEntryIsDirectory(&fileFind)) {
				unsigned char dirName[11];
				vos_memcpy(dirName, fileFind.dirEntry, 11);
				fat_dirChangeDir(fatContext, dirName);				
				PlayRandomTrack(randomNumber);				
				fat_dirChangeDir(fatContext, STEP_OUT);		
				fat_dirTableFind(fatContext, &fileFind, (char *)dirName);
			}
			else if (IsMusicFile(fileFind.dirEntry)) {
				if (counter++ == randomNumber) {
					vos_memcpy(param1Data, fileFind.dirEntry, 11);
					vos_signal_semaphore(&sPlaying); 	// Signal to play...
					vos_wait_semaphore(&sPlayAll);		// Wait till song finishes...					
					counter = 0;				
					return TRUE;
				}
			}
		}
		vos_lock_mutex(&mPlayState);
		if (playState == STOPPED || playState == STOP_VST) {
			skipState = NO_SKIP;
			vos_unlock_mutex(&mPlayState);
			return TRUE;
		}
		if (skipState == SKIP_START) { // If skip start then go to the beginning of this song...
			skipState = NO_SKIP;
			vos_unlock_mutex(&mPlayState);
			fat_dirTableFind(fatContext, &fileFind, (char *)param1Data);
			continue;
		}
		else if (skipState) { // Any other skip then just find a random song...
			skipState = NO_SKIP;
			vos_unlock_mutex(&mPlayState);
			return TRUE;
		}		
		vos_unlock_mutex(&mPlayState);
		
		if (fat_dirTableFindNext(fatContext, &fileFind) != FAT_OK) {
			break;
		}
	}
	return FALSE;
}
	
void RecurseFileSystem()
{	
	file_context_t fileFind;
	
	if (fat_dirTableFindFirst(fatContext, &fileFind) != FAT_OK) {
		goto CLEANUP;
	}
	
	while (1) {	
		if (fat_dirEntryIsValid(&fileFind)) {
			if (fat_dirEntryIsDirectory(&fileFind)) {
				unsigned char dirName[11];
				vos_memcpy(dirName, fileFind.dirEntry, 11);
				fat_dirChangeDir(fatContext, dirName);
				++directoryLevel;
				RecurseFileSystem();		
				if (directoryLevel > 0)
				fat_dirChangeDir(fatContext, STEP_OUT);		
				fat_dirTableFind(fatContext, &fileFind, (char *)dirName);
			}
			else if (IsMusicFile(fileFind.dirEntry)) {
				vos_memcpy(param1Data, fileFind.dirEntry, 11);
				vos_signal_semaphore(&sPlaying); 	// Signal to play...
				vos_wait_semaphore(&sPlayAll);		// Wait till song finishes...
			}
		}	
		// Check why the song finished...
		vos_lock_mutex(&mPlayState);
		if (playState == STOPPED || playState == STOP_VST) {
			vos_unlock_mutex(&mPlayState);
			break;
		}		
		// Check if the track has been skipped...
		if (skipState) {
			if (skipState == SKIP_FWD) {
				// Do nothing...
			}
			else if (skipState == SKIP_BWD) {
				unsigned char filename[12];
				unsigned char *previousFile;
				skipState = NO_SKIP;
				vos_memset(filename, 0x00, 12); // Null string...
				vos_memcpy(filename, fileFind.dirEntry, 11);						
				previousFile = FindPreviousFile(filename); // Find previous...
				vos_unlock_mutex(&mPlayState);
				if (previousFile) {
					fat_dirTableFind(fatContext, &fileFind, (char *)param1Data);
					if (!fat_dirEntryIsDirectory(&fileFind)) {						
						continue;
					}
				}
			}
			else if (skipState == SKIP_DIR) {
				skipState = NO_SKIP;
				vos_unlock_mutex(&mPlayState);
				break;
			}
			else if (skipState == SKIP_START) {
				skipState = NO_SKIP;
				vos_unlock_mutex(&mPlayState);
				fat_dirTableFind(fatContext, &fileFind, (char *)param1Data);
				continue;
			}
			skipState = NO_SKIP;
		}
		vos_unlock_mutex(&mPlayState);
		
		if (fat_dirTableFindNext(fatContext, &fileFind) != FAT_OK) {
			break;
		}
	}
	
CLEANUP:
	return;	
}	
	
void PlayAllTrackMGMT(void)
{	
	while (1) {			
		if (STATE == IDLE) {
			vos_delay_msecs(100);
			continue;
		}
	
		playState = PLAYING;
		counter = 0;
		do {			
			if (STATE & ALL) {
				directoryLevel = 0;
				RecurseFileSystem();
			}
			else if (STATE & RANDOM) {
				unsigned long random = GenerateRandomNumber();
				while (PlayRandomTrack(random) == FALSE)
					;				
			}
			else if (STATE & SINGLE) {				
				vos_signal_semaphore(&sPlaying); 	// Signal to play...
				vos_wait_semaphore(&sPlayAll);		// Wait till song finishes...
			}
			else { // Impossible Failure!
				break;
			}
			
			// Check the state returned from the play thread...
			vos_lock_mutex(&mPlayState);
			if (playState == STOP_VST) { // User issued stop...	
				vos_unlock_mutex(&mPlayState);
				break;
			}
			else if (playState == STOPPED) { // Song finished...
				vos_unlock_mutex(&mPlayState);
				goto CLEANUP;
			}
			else if (playState == STOP_DISK_REMOVED) { // Disk gone!...
				vos_unlock_mutex(&mPlayState);
				STATE = IDLE;
				goto CLEANUP;
			}
			vos_unlock_mutex(&mPlayState);
		} while (STATE & REPEAT);
		
		STATE = IDLE;
		playState = STOPPED;
		monWriteString("Stopped");
		monWriteString("\r");
		monPrompt();
		
CLEANUP:
		continue;		
	}
}

void Play(void)
{
	unsigned long actual = 0;
	unsigned char status = MON_ERROR_CMD_FAILED, endFillByte7_0;
	mp3_VS1053_ioctl_cb mp3_iocb;
	common_ioctl_cb_t spim_iocb;
	
	while (1)
	{		
		unsigned int frameCount = 0;
		unsigned char frameHeader[4];		
		unsigned int i = 0;		
		unsigned short data = 0, mp3Regval = 1;
		
		elapsed = -1;
		
		vos_wait_semaphore(&sPlaying); // Wait for a play to be received...		
		vos_lock_mutex(&mPlaying);
		
		skipState = 0x00;
		fileState = FILE_STATE_OPEN_WRITE;		
		status = fat_fileOpen(fatContext, &file, (char *) param1Data, FILE_MODE_READ);
		if (status != FAT_OK) {			
			monWriteString("Filename Invalid\r");
			goto CLEANUP; // Failed to open so skip the close function...
		}		
		
		Mon_Display_ID3();
		fat_fileTell(&file, (unsigned long *)&meta_data_offset); // used to remove metadata from ffwd...
		
		while (mp3Regval != 0) { // Reset the decode time on the mp3 chip...
			mp3_VS1053_write_reg(SCI_DECODE_TIME, 0x00);
			mp3_VS1053_write_reg(SCI_DECODE_TIME, 0x00);
			mp3_VS1053_read_register(SCI_DECODE_TIME, &mp3Regval);
		}
		
		mp3_VS1053_wait_for_dreq();
		mp3_VS1053_select_data();
		while (1) {
			monLedsGreenFlash(); // set the LED flashing....
			if (curState != STATE_DISK) {
				playState = STOP_DISK_REMOVED;
				break;
			}				
			if (playState) { // Controlled from the monitor...
				if (playState == STOPPED || playState == STOP_VST) {
					break;
				}
				else if (playState == PAUSED) {
					monPrompt();
					monLedsGreen(); // set the LED solid....
					vos_wait_semaphore(&sPaused);
					if (playState == PLAYING) {
						monPrompt();				
					}
					else {
						break;
					}
				}
				else if (playState == PAUSED_E) {
					monLedsGreen(); // set the LED solid....
					vos_wait_semaphore(&sPaused);
					if (playState != PLAYING) {
						break;
					}
				}
			}			
			if (skipState) {// Check the skip state...
				if (skipState == FFWD) {
					vos_lock_mutex(&mMP3File);
					offset_file = CalculateFrameSkip();
					if (fat_fileSeek(&file, offset_file, FAT_SEEK_CUR) == FAT_OK) {
						dataAvailable = 0;
						elapsed += 5;
						Mon_Write_Time();
						mp3_VS1053_prepare_skip();
						mp3_VS1053_write_reg(SCI_DECODE_TIME, (unsigned short)elapsed);
					}
					else {
						skipState = NO_SKIP;
						vos_unlock_mutex(&mMP3File);	
						break; // Skipped beyond the eof marker therefore play the next tune....
					}			
					skipState = NO_SKIP;
					vos_unlock_mutex(&mMP3File);				
				}
				else if (skipState == REWD) {
					vos_lock_mutex(&mMP3File);
					offset_file = CalculateFrameSkip();
					if (fat_fileSeek(&file, 0 - offset_file, FAT_SEEK_CUR) == FAT_OK) {
						dataAvailable = 0;
						elapsed -= 5;
						Mon_Write_Time();
						mp3_VS1053_prepare_skip();
						mp3_VS1053_write_reg(SCI_DECODE_TIME, (unsigned short)elapsed);
					}
					else { // Skipped back to the start of the file...
						fat_fileSeek(&file, 0, FAT_SEEK_SET);
						elapsed = 0;
						Mon_Write_Time();
						mp3_VS1053_write_reg(SCI_DECODE_TIME, (unsigned short)elapsed);
					}
					skipState = NO_SKIP;
					vos_unlock_mutex(&mMP3File);
				}
				else if (skipState == SKIP_BWD || skipState == SKIP_FWD || skipState == SKIP_DIR) {
					if (elapsed >= 1 && skipState == SKIP_BWD) {
						skipState = SKIP_START;
					}					
					break;
				}
				skipState = NO_SKIP;
			}
			// Read the decode time from the chip....
			vos_lock_mutex(&mMP3File);	
			if ((i++ % 15) == 0) {
				mp3_VS1053_read_register(SCI_DECODE_TIME, &data);
				if (data > elapsed)	{
					elapsed = data;
					Mon_Write_Time();
				}				
			}
			// Read data from the USB Flash drive...
			fat_fileRead(&file, 32, (char *)buffer, NULL, &actual);
			dataAvailable += actual;		
			vos_unlock_mutex(&mMP3File);
			vos_signal_semaphore(&sDataAvailable);			
			if (actual == 0) { // If we didn't read any data from the file then playback is finished, break!			
				break;
			}
		} // while (1)		
		fat_fileClose(&file); // Don't do this if we failed to open a file...
		
		// Clear the decoder buffers....		
		CancelPlayback();
		endFillByte7_0 = (unsigned char) mp3_VS1053_read_ram(param_endFillByte);
		vos_memset(buffer, (int)endFillByte7_0, 32);
		mp3_VS1053_deselect_control();	
		mp3_VS1053_select_data();	
		for (i = 0; i < 64; i++) {
			unsigned short num_bytes_written;		
			mp3_VS1053_wait_for_dreq();		
			mp3_VS1053_write(buffer, 32, &num_bytes_written);
		}
				
CLEANUP:		
		monLedsGreen(); // set the LED solid....
		fileState = FILE_STATE_NOT_OPEN;
		vos_unlock_mutex(&mPlaying);
		vos_signal_semaphore(&sPlayAll);		
	} // while (1)
}

// Thread sends data to the MP3 decoder..
void MP3()
{	
	mp3_VS1053_ioctl_cb mp3_iocb;
	unsigned short numWritten = 0;
	
	vos_init_semaphore(&sDataAvailable, 0);
	
	mp3_iocb.ioctl_code = VOS_IOCTL_MP3_VS1053_SET_VOLUME;
	mp3_iocb.set.volume.left = 0x40;
	mp3_iocb.set.volume.right = 0x40;
	vos_dev_ioctl(hMP3, &mp3_iocb);
	
	mp3_VS1053_select_data();
	
	while (1) {
		if (curState != STATE_DISK)
			vos_delay_msecs(10);	
		
		mp3_VS1053_wait_for_dreq();
		vos_lock_mutex(&mMP3File);
		mp3_VS1053_select_data();
		if (dataAvailable >= 32) { // 32 byte packet...
			mp3_VS1053_write((unsigned char *)buffer, 32, &numWritten);
			dataAvailable -= numWritten;
		}
		else if (dataAvailable > 0)	{ // less than a full packet...
			mp3_VS1053_write((unsigned char *)buffer, (unsigned short)dataAvailable, &numWritten);
			dataAvailable -= numWritten;
		}
		else {  // No data available so wait for some from the Host...			
			vos_unlock_mutex(&mMP3File);
			vos_wait_semaphore(&sDataAvailable);
			continue;
		}				
		vos_unlock_mutex(&mMP3File);
	}
}
