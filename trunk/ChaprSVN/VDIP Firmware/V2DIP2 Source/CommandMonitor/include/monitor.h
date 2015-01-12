/*
** monitor.h
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Header file for Vinculum II V2DAP Command Monitor
**
** Author: FTDI
** Project: Vinculum II
** Module: Vinculum II Applications
** Requires: VOS
** Comments: Refer to VNC1L Firmware User Manual for detailed operational description
**
** History:
**  1 – Initial version
**
*/
#ifndef __monitor_h__
#define __monitor_h__

#define MON_SUCCESS				   0        // Operation successful.
#define MON_NO_CMD				   1        // Operation successful.
#define MON_ERROR_BAD_CMD		   2        // Command doesn't exist.
#define MON_ERROR_CMD_FAILED	   3        // Command failed.
#define MON_ERROR_DISK_FULL		   4        // Disk is full.
#define MON_ERROR_INVALID		   5        // Invalid operation.
#define MON_ERROR_READ_ONLY		   6        // File or directory read only.
#define MON_ERROR_FILE_OPEN		   7        // A file is already open.
#define MON_ERROR_DIR_NOT_EMPTY	   8        // Directory is not empty.
#define MON_ERROR_INVALID_FILENAME 9        // FileName is not in a valid format.
#define MON_ERROR_NO_DISK		   10       // Command doesn't exist.

/* MONITOR FLAGS */
#define MON_SCS					   (1 << 0) // Shortened Command Set (bit zero set).
#define MON_ECS					   (0 << 0) // Extended Command Set (bit zero cleared).
#define MON_IPH					   (0 << 1) // Sets monitor to binary mode (bit 1 set).
#define MON_IPA					   (1 << 1) // Sets monitor to ASCII mode (bit 1 cleared).
#define MON_SUD					   (1 << 2) // Set the disk into suspend mode, it will wake the disk everytime there is
                                            // a command read from the monitor and then put it back into suspend.

typedef unsigned char (*FP)(void);          // Typedef for our function pointer.

void monInit(unsigned char);
void monCheck(unsigned char);
void monWriteString(char *str);
unsigned char monReadCmd();
unsigned char monReadNumber(unsigned char paramNo, unsigned char dataLength);
unsigned char monReadFileName(unsigned char paramNo);
unsigned char monWriteFileName(char *dataToWrite);
void monReadCr(void);
unsigned char monWrite(char *dataToWrite, unsigned short dataLength);
unsigned char monRead(char *dataToRead, unsigned short dataLength);
void monAddMultibyteNumberToConsole(unsigned char *dataToWrite, unsigned char dataLength);
char monValidateFileName(char Param_No);
char monValidateNumber(char Param_No, unsigned char dataLength);
char monValidateDateTime(char Param_No, char format);
void monAddNumberToConsole(unsigned char *dataToWrite, unsigned char dataLength);
unsigned char monASCIItoDec(char hexVal);
void monOnline();
void monVersion();
void monPrompt();
void monNoDisk();
void monNoUpgrade();
void monDeviceDetected(unsigned char);
void monDeviceRemoved(unsigned char);
void monSlaveEnabled();
void monSlaveDisabled();
void monBadCommand();
void monCommandFailed();
void monDiskFull();
void monInvalid();
void monReadOnly();
void monFileOpen();
void monDirNotEmpty();
void monFilenameInvalid();
void monLedsGreen();
void monLedsGreenFlash();
void monLedsRed();
void monLedsOff();
void monLedsOffNow();
void monDataMode();

#define MAX_CMD_LEN	   4
#define NUM_CMDS	   100

#define MON_TIME_16BIT 0               // 16 bit date/time format.
#define MON_TIME_32BIT 1               // 32 bit date/time format.

struct stCommand                       // Structure for storing commands.
{
	unsigned long  ecs;                // Extended command set version of the command.
	unsigned short scs;                // Short command set version of the commmand
	FP			   cmd_func;           // Pointer to function for this operation.
};

/* Globals */
/* Application Variables */
extern unsigned char cmdData[MAX_CMD_LEN]; // Data read from the Monitor.
extern unsigned char cmdDataIndex;         // Global for the amount of data that we have read in from monitor.
extern unsigned char cmdTerminator;        // last character - either a space or a carriage return
extern unsigned char param1Data[11];       // Data read from the Monitor.
extern unsigned char param2Data[11];       // Data read from the Monitor.

/* Command Parameters */
extern unsigned long param1;               // Stores the first number parameter of a cmd
extern unsigned long param2;               // Stores the second number parameter of a cmd
extern unsigned char Mon_Flags;            // Flags for the monitor, ascii/hex or ecs/scs etc...

extern VOS_HANDLE hMonitor;
extern VOS_HANDLE hBoms;
extern VOS_HANDLE hUsb1;
extern VOS_HANDLE hUsb2;
extern VOS_HANDLE hFT232;              // used only for V2DPS

#endif                                 // __monitor_h__

