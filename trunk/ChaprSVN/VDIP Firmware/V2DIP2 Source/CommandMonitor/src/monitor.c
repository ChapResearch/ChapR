/*
** monitor.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Command Monitor
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
#include "vos.h"

#include "UART.h"
#include "USBHost.h"
#include "USB.h"

#include "monitor.h"
#include "cfgmonitor.h"
#include "cmdmon.h"
#include "host.h"
#include "disk.h"
#ifdef V2MSC_FIRMWARE
#include "v2msc.h"
#endif // V2MSC_FIRMWARE	

// Numbers, letters and these symbols $ % ‘ - _ @ ~ ` ! ( ) { } ^ # & /
rom unsigned short maskCodePage[8] = {0x0000, 0x0000, 0xe3ff, 0x03ff, 0xffff, 0xc7ff, 0xffff, 0xe7ff};

/*
** commands
**
** Structure used a lookup table for monitor commands.
**
** Comments: Commands can be disk commands and require a disk to be present.
*/
// macro to build an 32bit work from up to 4 characters of a command
#define MON_COMMANDCODE(A, B, C, D) ((A << 24) | (B << 16) | (C << 8) | D)
// macro for command short codes (SCS) that require a disk
#define MON_CMDDISKREQ(A)			(A | 0x0100)
// macro for command short codes (SCS) that do not require a disk
#define MON_CMDNORESTR(A)			(A)
rom struct stCommand commands[81] =
{
	// Monitor Operations
	{MON_COMMANDCODE(0,	  0,	0,	 0),	MON_CMDDISKREQ(0x00), cmd_cr  },
	{MON_COMMANDCODE(0,	  'S',	'C', 'S'),	MON_CMDNORESTR(0x10), cmd_scs },
	{MON_COMMANDCODE(0,	  0,	0,	 0x10), MON_CMDNORESTR(0x10), cmd_scs },
	{MON_COMMANDCODE(0,	  'E',	'C', 'S'),	MON_CMDNORESTR(0x11), cmd_ecs },
	{MON_COMMANDCODE(0,	  'I',	'P', 'A'),	MON_CMDNORESTR(0x90), cmd_ipa },
	{MON_COMMANDCODE(0,	  'I',	'P', 'H'),	MON_CMDNORESTR(0x91), cmd_iph },
	{MON_COMMANDCODE(0,	  'S',	'B', 'D'),	MON_CMDNORESTR(0x14), cmd_sbd },
	{MON_COMMANDCODE(0,	  'F',	'W', 'V'),	MON_CMDNORESTR(0x13), cmd_fwv },
	{MON_COMMANDCODE(0,	  0,	0,	 'E'),	MON_CMDNORESTR(0x45), cmd_echo},
	{MON_COMMANDCODE(0,	  0,	0,	 'e'),	MON_CMDNORESTR(0x65), cmd_echo},
	// Disk Operations
	{MON_COMMANDCODE(0,	  'D',	'I', 'R'),	MON_CMDDISKREQ(0x01), cmd_dir },
	{MON_COMMANDCODE(0,	  0,	'C', 'D'),	MON_CMDDISKREQ(0x02), cmd_cd  },
	{MON_COMMANDCODE(0,	  0,	'R', 'D'),	MON_CMDDISKREQ(0x04), cmd_rd  },
	{MON_COMMANDCODE(0,	  'D',	'L', 'D'),	MON_CMDDISKREQ(0x05), cmd_dld },
	{MON_COMMANDCODE(0,	  'M',	'K', 'D'),	MON_CMDDISKREQ(0x06), cmd_mkd },
	{MON_COMMANDCODE(0,	  'D',	'L', 'F'),	MON_CMDDISKREQ(0x07), cmd_dlf },
	{MON_COMMANDCODE(0,	  'W',	'R', 'F'),	MON_CMDDISKREQ(0x08), cmd_wrf },
	{MON_COMMANDCODE(0,	  'O',	'P', 'W'),	MON_CMDDISKREQ(0x09), cmd_opw },
	{MON_COMMANDCODE(0,	  'C',	'L', 'F'),	MON_CMDDISKREQ(0x0A), cmd_clf },
	{MON_COMMANDCODE(0,	  'R',	'D', 'F'),	MON_CMDDISKREQ(0x0B), cmd_rdf },
	{MON_COMMANDCODE(0,	  'R',	'E', 'N'),	MON_CMDDISKREQ(0x0C), cmd_ren },
	{MON_COMMANDCODE(0,	  'O',	'P', 'R'),	MON_CMDDISKREQ(0x0E), cmd_opr },
	{MON_COMMANDCODE(0,	  'S',	'E', 'K'),	MON_CMDDISKREQ(0x28), cmd_sek },
	{MON_COMMANDCODE(0,	  0,	'F', 'S'),	MON_CMDDISKREQ(0x12), cmd_fs  },
	{MON_COMMANDCODE(0,	  'F',	'S', 'E'),	MON_CMDDISKREQ(0x93), cmd_fse },
	{MON_COMMANDCODE(0,	  'I',	'D', 'D'),	MON_CMDDISKREQ(0x0F), cmd_idd },
	{MON_COMMANDCODE('I', 'D',	'D', 'E'),	MON_CMDDISKREQ(0x94), cmd_idde},
	{MON_COMMANDCODE(0,	  'D',	'S', 'N'),	MON_CMDDISKREQ(0x2D), cmd_dsn },
	{MON_COMMANDCODE(0,	  'D',	'V', 'L'),	MON_CMDDISKREQ(0x2E), cmd_dvl },
	{MON_COMMANDCODE('D', 'I',	'R', 'T'),	MON_CMDDISKREQ(0x2F), cmd_dirt},
	// Power Management
	{MON_COMMANDCODE(0,	  'S',	'U', 'D'),	MON_CMDDISKREQ(0x15), cmd_sud },
	{MON_COMMANDCODE(0,	  'W',	'K', 'D'),	MON_CMDDISKREQ(0x16), cmd_wkd },
	{MON_COMMANDCODE(0,	  'S',	'U', 'M'),	MON_CMDNORESTR(0x17), cmd_sum },
	// Unused I/O Pin Commands
	{MON_COMMANDCODE(0,	  'I',	'O', 'R'),	MON_CMDNORESTR(0x29), cmd_ior },
	{MON_COMMANDCODE(0,	  'I',	'O', 'W'),	MON_CMDNORESTR(0x2A), cmd_iow },
	// Printer Commands
	{MON_COMMANDCODE(0,	  'P',	'G', 'S'),	MON_CMDNORESTR(0x81), cmd_pgs },
	{MON_COMMANDCODE(0,	  'P',	'S', 'R'),	MON_CMDNORESTR(0x82), cmd_psr },
	// USB Device Commands
	{MON_COMMANDCODE(0,	  'Q',	'P', '1'),	MON_CMDNORESTR(0x2B), cmd_qp1 },
	{MON_COMMANDCODE(0,	  'Q',	'P', '2'),	MON_CMDNORESTR(0x2C), cmd_qp2 },
	{MON_COMMANDCODE(0,	  0,	'Q', 'D'),	MON_CMDNORESTR(0x85), cmd_qd  },
	{MON_COMMANDCODE(0,	  0,	'S', 'C'),	MON_CMDNORESTR(0x86), cmd_sc  },
	{MON_COMMANDCODE(0,	  'D',	'S', 'D'),	MON_CMDNORESTR(0x83), cmd_dsd },
	{MON_COMMANDCODE(0,	  'D',	'R', 'D'),	MON_CMDNORESTR(0x84), cmd_drd },
	{MON_COMMANDCODE(0,	  'S',	'S', 'U'),	MON_CMDNORESTR(0x9A), cmd_ssu },
	{MON_COMMANDCODE(0,	  0,	'S', 'F'),	MON_CMDNORESTR(0x87), cmd_sf  },
	{MON_COMMANDCODE(0,	  'Q',	'S', 'S'),	MON_CMDNORESTR(0x98), cmd_qss },
	// FTDI Device Commands
	{MON_COMMANDCODE(0,	  'F',	'B', 'D'),	MON_CMDNORESTR(0x18), cmd_fbd },
	{MON_COMMANDCODE(0,	  'F',	'M', 'C'),	MON_CMDNORESTR(0x19), cmd_fmc },
	{MON_COMMANDCODE(0,	  'F',	'S', 'D'),	MON_CMDNORESTR(0x1A), cmd_fsd },
	{MON_COMMANDCODE(0,	  'F',	'F', 'C'),	MON_CMDNORESTR(0x1B), cmd_ffc },
	{MON_COMMANDCODE(0,	  'F',	'G', 'M'),	MON_CMDNORESTR(0x1C), cmd_fgm },
	{MON_COMMANDCODE(0,	  'F',	'S', 'L'),	MON_CMDNORESTR(0x22), cmd_fsl },
	{MON_COMMANDCODE(0,	  'F',	'S', 'B'),	MON_CMDNORESTR(0x23), cmd_fsb },
	{MON_COMMANDCODE(0,	  'F',	'G', 'B'),	MON_CMDNORESTR(0x24), cmd_fgb },
	// VMusic Commands
	{MON_COMMANDCODE(0,	  'V',	'P', 'F'),	MON_CMDDISKREQ(0x1D), cmd_vpf },
	{MON_COMMANDCODE(0,	  'V',	'R', 'F'),	MON_CMDDISKREQ(0x89), cmd_vrf },
	{MON_COMMANDCODE(0,	  'V',	'S', 'T'),	MON_CMDDISKREQ(0x20), cmd_vst },
	{MON_COMMANDCODE(0,	  'V',	'3', 'A'),	MON_CMDDISKREQ(0x21), cmd_v3a },
	{MON_COMMANDCODE(0,	  'V',	'R', 'A'),	MON_CMDDISKREQ(0x8A), cmd_vra },
	{MON_COMMANDCODE(0,	  'V',	'R', 'R'),	MON_CMDDISKREQ(0x8F), cmd_vrr },
	{MON_COMMANDCODE(0,	  'V',	'S', 'F'),	MON_CMDDISKREQ(0x25), cmd_vsf },
	{MON_COMMANDCODE(0,	  'V',	'S', 'B'),	MON_CMDDISKREQ(0x26), cmd_vsb },
	{MON_COMMANDCODE(0,	  'V',	'S', 'D'),	MON_CMDDISKREQ(0x8E), cmd_vsd },
	{MON_COMMANDCODE(0,	  0,	'V', 'P'),	MON_CMDDISKREQ(0x8B), cmd_vp  },
	{MON_COMMANDCODE(0,	  0,	'V', 'F'),	MON_CMDDISKREQ(0x8C), cmd_vf  },
	{MON_COMMANDCODE(0,	  0,	'V', 'B'),	MON_CMDDISKREQ(0x8D), cmd_vb  },
	{MON_COMMANDCODE(0,	  'V',	'R', 'D'),	MON_CMDDISKREQ(0x1F), cmd_vrd },
	{MON_COMMANDCODE(0,	  'V',	'W', 'R'),	MON_CMDDISKREQ(0x1E), cmd_vwr },
	{MON_COMMANDCODE(0,	  'V',	'S', 'V'),	MON_CMDDISKREQ(0x88), cmd_vsv },
	// VDFC Commands
	{MON_COMMANDCODE(0,	  0,	'A', ':'),	MON_CMDDISKREQ(0x41), cmd_a	  },
	{MON_COMMANDCODE(0,	  0,	'B', ':'),	MON_CMDDISKREQ(0x42), cmd_b	  },
	{MON_COMMANDCODE(0,	  'I',	'M', 'S'),	MON_CMDDISKREQ(0x26), cmd_ims },
	{MON_COMMANDCODE(0,	  'I',	'M', 'F'),	MON_CMDDISKREQ(0x25), cmd_imf },
	{MON_COMMANDCODE(0,	  'C',	'P', 'S'),	MON_CMDDISKREQ(0x21), cmd_cps },
	{MON_COMMANDCODE(0,	  'C',	'P', 'F'),	MON_CMDDISKREQ(0x20), cmd_cpf },
	// Debug Commands
	{MON_COMMANDCODE(0,	  0,	'S', 'D'),	MON_CMDDISKREQ(0x03), cmd_sd  },
	{MON_COMMANDCODE(0,	  0,	'S', 'W'),	MON_CMDDISKREQ(0x92), cmd_sw  },
	{MON_COMMANDCODE(0,	  'F',	'W', 'U'),	MON_CMDNORESTR(0x95), cmd_fwu },
	{MON_COMMANDCODE(0,	  'D',	'R', 'I'),	MON_CMDDISKREQ(0x99), cmd_dri },
	{MON_COMMANDCODE(0,	  'I',	'N', 'I'),	MON_CMDNORESTR(0xff), cmd_ini },
	// End
	{0xFFFF,			  0xFF, 0 /* 0*/}
};

/* Globals */
/* Application Variables */
unsigned char cmdData[MAX_CMD_LEN];    // Data read from the Monitor.
unsigned char cmdTerminator;           // last character - either a space or a carriage return
unsigned char cmdDataIndex;            // Global for the amount of data that we have read in from monitor.
unsigned char param1Data[11];          // Data read from the Monitor for parameter 1
unsigned char param2Data[11];          // Data read from the Monitor for parameter 2

/* Command Parameters */
unsigned long param1;                  // Stores the first number parameter of a cmd
unsigned long param2;                  // Stores the second number parameter of a cmd
unsigned char Mon_Flags;               // Flags for the monitor, ascii/hex or ecs/scs etc...

extern unsigned char fileState;

// from state machine program
extern unsigned char checkDataMode(void);

// for flashing LEDs
void monFlashLedThread(void);
char monFlashLED = 0;
char monLEDstate = 0x60;
	
/*
** monInit
**
** Initialise the monitor.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monInit(unsigned char flags)
{
	Mon_Flags = flags;
	vos_memset(deviceList, 0, sizeof(deviceList));
	diskInit();
	vos_create_thread_ex(20, 120, monFlashLedThread, "LED flash", 0);
}

/*
** monWriteString
**
** Write a string to the monitor.
**
** Parameters: NULL terminated string to write to monitor.
** Returns: none
** Comments:
*/
void monWriteString(char *str)
{
	unsigned short len;
	unsigned short actual;

	// changed from len = strlen(str); to remove need for string library
	len = 0;
	while (str[len])
	{
		len++;
	}
	vos_dev_write(hMonitor, (unsigned char *) str, len, &actual);
}

/*
** monVersion
**
** Write firmware version to the monitor.
**
** Parameters: none
** Returns: none
** Comments: Firmware version is stored in reserved area of ROM
*/
extern rom char userDataArea[16];
void monVersion()
{
	char letter;
	char count;

	for (count = 0; count < 16; count++)
	{
		letter = userDataArea[count];
		if (letter == 0)
		{
			break;
		}
		monWrite(&letter, 1);
	}
}

/*
** monPrompt
**
** Write the standard prompt to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monPrompt()
{
#ifdef CFGMONITOR_SHOW_PROMPT
	char *strPrompt = "D:\\>\r";
	char *strPromptS = ">\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strPromptS);
	}
	else
	{
		monWriteString(strPrompt);
	}

#endif                                 // CFGMONITOR_SHOW_PROMPT
}

/*
** monNoDisk
**
** Write the no disk message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monNoDisk()
{
	char *strNoDisk = "No Disk\r";
	char *strNoDiskS = "ND\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strNoDiskS);
	}
	else
	{
		monWriteString(strNoDisk);
	}
}

/*
** monNoUpgrade
**
** Write the no upgrade message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monNoUpgrade()
{
	char *strNoUpgrade = "No Upgrade\r";
	char *strNoUpgradeS = "NU\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strNoUpgradeS);
	}
	else
	{
		monWriteString(strNoUpgrade);
	}
}

/*
** monDeviceDetected
**
** Write the device detected message to the monitor.
**
** Parameters: hostPort: USB port that has detected a device
**  Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monDeviceDetected(unsigned char hostPort)
{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
	char *strDeviceDet = "Device Detected P";
	char *strDeviceDetS = "DD";
	char letter;

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strDeviceDetS);
	}
	else
	{
		monWriteString(strDeviceDet);
	}

	letter = hostPort + '1';
	monWrite(&letter, 1);
	letter = 0x0d;
	monWrite(&letter, 1);
#endif                                 // CFGMONITOR_SHOW_CONNECTS_REMOVALS
}

/*
** monDeviceRemoved
**
** Write the device removed message to the monitor.
**
** Parameters: hostPort: USB port that has removed a device
**  Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monDeviceRemoved(unsigned char hostPort)
{
#ifdef CFGMONITOR_SHOW_CONNECTS_REMOVALS
	char *strDeviceRem = "Device Removed P";
	char *strDeviceRemS = "DR";
	char letter;

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strDeviceRemS);
	}
	else
	{
		monWriteString(strDeviceRem);
	}

	letter = hostPort + '1';
	monWrite(&letter, 1);
	letter = 0x0d;
	monWrite(&letter, 1);
#endif                                 // CFGMONITOR_SHOW_CONNECTS_REMOVALS
}

/*
** monSlaveEnabled
**
** Write the slave enabled message to the monitor.
**
** Parameters: hostPort: USB port that has detected a device
**  Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monSlaveEnabled()
{
	char *strSlaveEn = "Slave Enabled\r";
	char *strSlaveEnS = "SDA\r";
	char letter;

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strSlaveEnS);
	}
	else
	{
		monWriteString(strSlaveEn);
	}
}

/*
** monSlaveDisabled
**
** Write the slave disabled message to the monitor.
**
** Parameters: hostPort: USB port that has detected a device
**  Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monSlaveDisabled()
{
	char *strSlaveDis = "Slave Disabled\r";
	char *strSlaveDisS = "SDD\r";
	char letter;

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strSlaveDisS);
	}
	else
	{
		monWriteString(strSlaveDis);
	}
}

/*
** monBadCommand
**
** Write the bad command message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monBadCommand()
{
	char *strBadCmd = "Bad Command\r";
	char *strBadCmdS = "BC\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strBadCmdS);
	}
	else
	{
		monWriteString(strBadCmd);
	}
}

/*
** monCommandFailed
**
** Write the command failed message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monCommandFailed()
{
	char *strCmdFailed = "Command Failed\r";
	char *strCmdFailedS = "CF\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strCmdFailedS);
	}
	else
	{
		monWriteString(strCmdFailed);
	}
}

/*
** monDiskFull
**
** Write the disk full message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monDiskFull()
{
	char *strDiskFull = "Disk Full\r";
	char *strDiskFullS = "DF\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strDiskFullS);
	}
	else
	{
		monWriteString(strDiskFull);
	}
}

/*
** monInvalid
**
** Write the invalid file message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monInvalid()
{
	char *strInvalid = "Invalid\r";
	char *strInvalidS = "FI\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strInvalidS);
	}
	else
	{
		monWriteString(strInvalid);
	}
}

/*
** monReadOnly
**
** Write the read only message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monReadOnly()
{
	char *strReadOnly = "Read Only\r";
	char *strReadOnlyS = "RO\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strReadOnlyS);
	}
	else
	{
		monWriteString(strReadOnly);
	}
}

/*
** monFileOpen
**
** Write the file open message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monFileOpen()
{
	char *strFileOpen = "File Open\r";
	char *strFileOpenS = "FO\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strFileOpenS);
	}
	else
	{
		monWriteString(strFileOpen);
	}
}

/*
** monDirNotEmpty
**
** Write the directory not empty message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monDirNotEmpty()
{
	char *strDirNotEmpty = "Dir Not Empty\r";
	char *strDirNotEmptyS = "NE\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strDirNotEmptyS);
	}
	else
	{
		monWriteString(strDirNotEmpty);
	}
}

/*
** monFilenameInvalid
**
** Write the filename invalid message to the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: none
** Comments:
*/
void monFilenameInvalid()
{
	char *strFilenameInvalid = "Filename Invalid\r";
	char *strFilenameInvalidS = "FN\r";

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		monWriteString(strFilenameInvalidS);
	}
	else
	{
		monWriteString(strFilenameInvalid);
	}
}

/*
** monReadCmd
**
** Read a command in from the monitor.
**
** Parameters: Mon_Flags: ECS or SCS versions
** Returns: MON_SUCCESS for a command, MON_NO_CMD for no command
** Comments:
*/
unsigned char monReadCmd()
{
	common_ioctl_cb_t uart_iocb;       // UART iocb for getting bytes available.
	char charRead = '\0';              // The char that we have just read.
	unsigned int dataAvail = 0;        // How much data is available to be read?
	unsigned int result = MON_NO_CMD;

	uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
	vos_dev_ioctl(hMonitor, &uart_iocb);

	dataAvail = uart_iocb.get.queue_stat; // How much data to read?

	if (dataAvail)
	{
		// start flashing green LED
		monLedsGreenFlash();
		
		// So we have data on the queue, read one byte...
		for (cmdDataIndex = 0; cmdDataIndex < MAX_CMD_LEN; cmdDataIndex++)
		{
			cmdData[cmdDataIndex] = '\0'; // Null terminate command string
		}

		cmdDataIndex = 0;                 // Clear our index...
		param1 = 0;                       // Reset param 1
		param2 = 0;                       // Reset param 2

		do
		{
			monRead(&charRead, 1);

			// A CR or space must terminate all of the commands, read until we get this...			
			if ((charRead == 0x0D) || (charRead == ' '))
			{
#ifdef V2MSC_FIRMWARE
				if ((cmdDataIndex == 0) && (charRead == ' '))
				{
					cmdData[cmdDataIndex] = charRead;
				}
				cmd_pause(0);
#endif // V2MSC_FIRMWARE	
				break;
			}
			
#ifdef V2MSC_FIRMWARE
			if ((cmdDataIndex == 0) && ((charRead == 'e') || (charRead == 'E')))
			{
				cmd_pause(1);
			}
#endif // V2MSC_FIRMWARE	

			if (cmdDataIndex < MAX_CMD_LEN)
			{
				cmdData[cmdDataIndex] = charRead; // Append our command buffer...
				cmdDataIndex++;                   // Increment the index...
			}
		}
		while (1);

		cmdTerminator = charRead;

		result = MON_SUCCESS;
	}

	return result;
}

/*
** monReadFileName
**
** Read a filename in from the monitor.
**
** Parameters: paramNo: either in param1data or param2data
** Returns: MON_SUCCESS for a filename, MON_NO_CMD for no filename
** Comments: Return array of characters with padding to 8.3 format
*/
unsigned char monReadFileName(unsigned char paramNo) // Read a filename from the monitor
{
	// common_ioctl_cb_t uart_iocb; // UART iocb for getting bytes available.
	char charRead = '\0';                            // The char that we have just read.
	char lastCharRead = '.';                         // The char that we have just read.
	unsigned char i;
	unsigned char result = MON_NO_CMD;
	unsigned char *param;
	unsigned char paramDataIndex = 0;

	if (paramNo == 1)
	{
		param = &param1Data[0];
	}
	else
	{
		param = &param2Data[0];
	}

	for (i = 0; i < 11; i++)
	{
		param[i] = ' ';
	}

	do
	{
		monRead(&charRead, 1);
		
		// skip any leading spaces
		if ((paramDataIndex == 0) && (charRead == ' '))
			continue;

		// A CR or space must terminate a filename, read until we get this...
		if ((charRead == 0x0D) || (charRead == ' '))
			break;

		if ((charRead == '.') && (lastCharRead != '.'))
		{
			if (paramDataIndex < 8)
			{
				if (param[0] != '.')
				{
					paramDataIndex = 8;
				}
			}
		}
		else
		{
			if (paramDataIndex < 11)
			{
				param[paramDataIndex] = charRead; // Append our parameters buffer...
				paramDataIndex++;                 // Increment the index...
			}
		}

		lastCharRead = charRead;
	}
	while (1);

	cmdTerminator = charRead;

	return MON_SUCCESS;
}

/*
** monReadNumber
**
** Read a number in from the monitor.
**
** Parameters: paramNo: either in param1data or param2data
**  dataLength: size in bytes of number expected
**  Mon_Flags: different formats for IPA and IPH modes
** Returns: MON_SUCCESS for a number, MON_NO_CMD for no number
** Comments: Numbers are input MSB first.
**  Prepend number with $ or 0x for hexadecimal otherwise decimal.
*/
unsigned char monReadNumber(unsigned char paramNo, unsigned char dataLength)
{
	char charRead = '\0';              // The char that we have just read.
	unsigned char i;
	unsigned char hexinput = FALSE;
	unsigned char result = MON_NO_CMD;
	unsigned char *param;
	unsigned long value = 0;
	unsigned char paramDataIndex = 0;
	unsigned char stack[16];
	unsigned char stackIndex = 0;

	if (paramNo == 1)
	{
		param = &param1Data[0];
	}
	else
	{
		param = &param2Data[0];
	}

	if (Mon_Flags & MON_IPA)           // Read the data in ASCII format.
	{
		for (i = 0; i < 11; i++)
		{
			param[i] = 0;
		}

		// burn any leading zeros or spaces (forever!)
		do
		{
			monRead(&charRead, 1);
			if (charRead == ' ') charRead = '0';
		}
		while (charRead == '0');

		if ((charRead == '$') || (charRead == 'x'))
		{
			hexinput = TRUE;
			monRead(&charRead, 1);
		}

		// special case for 'S' used in VDPS
		if ((charRead == 's') || (charRead == 'S'))
		{
			param[0] = charRead;

			do
			{
				monRead(&charRead, 1);
			}
			while (charRead != 0x0d);
			
			// save the command terminator character
			cmdTerminator = charRead;
		}
		else
		{
			// A CR must terminate all of the commands, read until we get this...
			while ((charRead != 0x0D) && (charRead != ' '))
			{
				if (stackIndex < 16)
				{
					// stack up BCD (or HEX) characters
					stack[stackIndex] = monASCIItoDec(charRead);
					stackIndex++;
				}

				monRead(&charRead, 1);
			}
			
			// save the command terminator character
			cmdTerminator = charRead;

			if (hexinput)
			{
				while (stackIndex > 0)
				{
					stackIndex--;
					charRead = stack[stackIndex];

					if (stackIndex > 0)
					{
						stackIndex--;
						charRead += (stack[stackIndex] * 16);
					}

					param[paramDataIndex] = charRead; // Append our parameter buffer...
					paramDataIndex++;                 // Increment the index...
				}
			}
			else
			{
				value = 0;
				paramDataIndex = 0;

				while (stackIndex > 0)
				{
					stackIndex--;

					value += stack[paramDataIndex];
					paramDataIndex++;

					if (stackIndex > 0)
					{
						value *= 10;
					}
				}

				vos_memcpy(param, &value, 4);
			}
		}
	}
	else
	{
		if (dataLength)
		{
			do
			{
				dataLength--;
				monRead(&charRead, 1);
				param[dataLength] = charRead;
			}
			while (dataLength != 0);
		}

		// save the command terminator character
		cmdTerminator = charRead;
	}

	return MON_SUCCESS;
}

/*
** monReadCr
**
** Read until a carriage return is received from the monitor.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monReadCr()
{
	char charRead;

	// read until carriage return
	if (cmdTerminator != 0x0d)
	{
		do
		{
			monRead(&charRead, 1);
		}
		while (charRead != 0x0d);
	}
}

/*
** monWrite
**
** Write data buffer to the monitor.
**
** Parameters: dataToWrite: pointer to data to write
**  dataLength: size in bytes of buffer
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if could not write
** Comments: Data written in RAW format.
*/
unsigned char monWrite(char *dataToWrite, unsigned short dataLength)
{                                      // Write data back to the monitor...
	unsigned short numWritten;

	vos_dev_write(hMonitor, (unsigned char *) dataToWrite, dataLength, &numWritten);

	if (numWritten == dataLength)
		return MON_SUCCESS;
	else
		return MON_ERROR_CMD_FAILED;
}

/*
** monWriteFileName
**
** Read a filename in from the monitor.
**
** Parameters: dataToWrite: pointer to filename (padded to 8.3)
** Returns: MON_SUCCESS
** Comments: Filename is reformatted without padding and with '.' added for extension if required.
*/
unsigned char monWriteFileName(char *dataToWrite)
{                                      // Write data back to the monitor...
	unsigned char i;
	unsigned char dot = '.';
	unsigned short numWritten;

	for (i = 0; i < 11; i++)
	{
		if (dataToWrite[i] == ' ')
		{
			if (i < 8)
			{
				if (dataToWrite[0] == '.')
					break;

				if (dataToWrite[8] == ' ')
					break;

				i = 8;
				vos_dev_write(hMonitor, &dot, 1, &numWritten);
			}
			else
			{
				break;
			}
		}
		else if (i == 8)
		{
			vos_dev_write(hMonitor, &dot, 1, &numWritten);
		}

		vos_dev_write(hMonitor, (unsigned char *) &dataToWrite[i], 1, &numWritten);
	}

	return MON_SUCCESS;
}

/*
** monRead
**
** Read data buffer from the monitor.
**
** Parameters: dataToRead: pointer to buffer to read data into
**  dataLength: size in bytes of data to read
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if could not write
** Comments: Data read in RAW format.
*/
unsigned char monRead(char *dataToRead, unsigned short dataLength)
{
	common_ioctl_cb_t uart_iocb;       // UART iocb for getting bytes available.
	unsigned short numRead;

	uart_iocb.ioctl_code = VOS_IOCTL_COMMON_GET_RX_QUEUE_STATUS;
	uart_iocb.get.queue_stat = 0;

	do
	{
		vos_dev_ioctl(hMonitor, &uart_iocb);
	}
	while (uart_iocb.get.queue_stat < dataLength);

	vos_dev_read(hMonitor, (unsigned char *) dataToRead, dataLength, &numRead);

	if (numRead == dataLength)
		return MON_SUCCESS;
	else
		return MON_ERROR_CMD_FAILED;
}

/*
** monAddNumberToConsole
**
** Write a number to the monitor.
**
** Parameters: dataToWrite: pointer to value to write
**  dataLength: size in bytes of number
**  Mon_Flags: different formats for IPA and IPH modes
** Returns: MON_SUCCESS for a number, MON_NO_CMD for no number
** Comments: Numbers are output LSB first.
**  Prepend number with $ for hexadecimal.
**  Numbers are displayed as single bytes.
*/
void monAddNumberToConsole(unsigned char *dataToWrite, unsigned char dataLength)
{
	if (Mon_Flags & MON_IPA)           // Write the data back in ASCII format.
	{
		char str[4];
		unsigned char data;
		unsigned char ch;
		str[0] = '$';
		
		str[3] = ' ';

		while (dataLength)
		{
			dataLength--;
			data = *dataToWrite++;
			ch = data >> 4;

			if (ch > 9)
				str[1] = 'A' - 10 + ch;
			else
				str[1] = '0' + ch;

			ch = data & 0x0f;

			if (ch > 9)
				str[2] = 'A' - 10 + ch;
			else
				str[2] = '0' + ch;

			monWrite(str, 4);
		}
	}
	else                               // Send the number back as binary format.
	{
		monWrite((char *) dataToWrite, (short) dataLength);
	}
}

/*
** monAddMultibyteNumberToConsole
**
** Write a number to the monitor.
**
** Parameters: dataToWrite: pointer to value to write
**  dataLength: size in bytes of number
**  Mon_Flags: different formats for IPA and IPH modes
** Returns: MON_SUCCESS for a number, MON_NO_CMD for no number
** Comments: Numbers are output LSB first.
**  Prepend number with $ for hexadecimal.
**  Numbers are displayed as a single value.
*/
void monAddMultibyteNumberToConsole(unsigned char *dataToWrite, unsigned char dataLength)
{
	char str[2];
	unsigned char data;
	unsigned char ch;

	while (dataLength)
	{
		dataLength--;
		data = dataToWrite[dataLength];
		ch = data >> 4;

		if (ch > 9)
			str[0] = 'A' - 10 + ch;
		else
			str[0] = '0' + ch;

		ch = data & 0x0f;

		if (ch > 9)
			str[1] = 'A' - 10 + ch;
		else
			str[1] = '0' + ch;

		monWrite(str, 2);
	}
}

/*
** monValidateFileName
**
** Validate a filename contains only allowable characters.
**
** Parameters: paramNo: use param1data or param2data
** Returns: MON_SUCCESS, MON_ERROR_INVALID_FILENAME if not valid
** Comments: Bitmap used for describing valid characters
*/
unsigned char monValidateFileName(char paramNo)
{
	unsigned char *param;
	unsigned char temp;
	unsigned char i;
	char result = MON_SUCCESS;
	unsigned short mask;
	
	if (paramNo == 2)
	{
		param = &param2Data[0];
	}
	else
	{
		param = &param1Data[0];
	}

	// check for blank filename
	if (*param == ' ')
	{
		result = MON_ERROR_INVALID_FILENAME;
	}

	for (i = 0; i < 11; i++)
	{
		temp = *param;
		
		if (temp == 0xe5)
		{
			temp = 0x05;
		}

		if (temp < 128)
		{
			// check for illegal characters
			if (((maskCodePage[temp >> 4] >> (temp & 0x0f)) & 1) == 0)
			{
				result = MON_ERROR_INVALID_FILENAME;
				break;
			}

			// make letters uppercase
			if ((temp >= 'a') && (temp <= 'z'))
			{
				*param -= 0x20;
			}
		}

		param++;
	}

	return result;
}

/*
** monValidateNumber
**
** Validate a filename contains only allowable characters.
**
** Parameters: paramNo: use param1data or param2data
**  dataLength: length of data expected
** Returns: MON_SUCCESS
** Comments: No validation performed but param1 or param2 variable is updated to correct value.
*/
unsigned char monValidateNumber(char paramNo, unsigned char dataLength)
{
	if (paramNo == 2)
	{
		vos_memcpy(&param2, &param2Data[0], (short) dataLength);
	}
	else
	{
		vos_memcpy(&param1, &param1Data[0], (short) dataLength);
	}

	return MON_SUCCESS;
}

/*
** monValidateDateTime
**
** Validate a date or time is valid.
**
** Parameters: paramNo: use param1data or param2data
**  format: choose between 32 bit date/time and 16 bit date
** Returns: MON_SUCCESS, MON_ERROR_INVALID if not valid.
** Comments:
*/
unsigned char monValidateDateTime(char paramNo, char format)
{
	int len = 0;
	int param;                         // Param1 or Param2
	char result = MON_ERROR_INVALID;
	int tmp_num = 0;

	return monValidateNumber(paramNo, 4);

#if 0

	if (monValidateNumber(paramNo, 4) != MON_SUCCESS)
	{
		return MON_ERROR_INVALID;
	}

	if (paramNo == 1)
		param = param1;
	else
		param = param2;

	if (format == MON_TIME_32BIT)
	{
		// bits 31 - 25 Year
		tmp_num = (param & 0xFE000000) >> 25;

		if ((tmp_num < 0 || tmp_num > 127))
			return result;

		// bits 24 - 21 Month
		tmp_num = (param & 0x01E00000) >> 21;

		if (tmp_num < 1 || tmp_num > 12)
			return result;

		// bits 20 - 16 Days
		tmp_num = (param & 0x001F0000) >> 16;

		if (tmp_num < 1 || tmp_num > 31)
			return result;

		// bits 15 - 11 Hours
		tmp_num = (param & 0x0000F800) >> 11;

		if (tmp_num < 0 || tmp_num > 23)
			return result;

		// bits 10 - 5 Minutes
		tmp_num = (param & 0x000007E0) >> 5;

		if (tmp_num < 0 || tmp_num > 59)
			return result;

		// bits 4 - 0 Seconds
		tmp_num = (param & 0x0000001F);

		if (tmp_num < 0 || tmp_num > 29)
			return result;

		return MON_SUCCESS;
	}
	else
	{
		// We don't care what is in the first MS 16bits, maybe?
		// bits 15 - 9 Year
		tmp_num = (param & 0x0000FE00) >> 9;

		if (tmp_num < 0 || tmp_num > 127)
			return result;

		// bits 8 - 5 Month
		tmp_num = (param & 0x000001E0) >> 5;

		if (tmp_num < 1 || tmp_num > 12)
			return result;

		// bits 4 - 0 Days
		tmp_num = (param & 0x0000001F);

		if (tmp_num < 1 || tmp_num > 31)
			return result;

		return MON_SUCCESS;
	}

#endif
}

/*
** monASCIItoDec
**
** Convert a hexadecimanl character into a nibble.
**
** Parameters: hexVal: ASCII character for hexVal (0-9A-F)
** Returns: hexadecimal value of character.
** Comments:
*/
unsigned char monASCIItoDec(char hexVal)
{
	switch (hexVal)
	{
	case '0': return 0;

	case '1': return 1;

	case '2': return 2;

	case '3': return 3;

	case '4': return 4;

	case '5': return 5;

	case '6': return 6;

	case '7': return 7;

	case '8': return 8;

	case '9': return 9;

	case 'A': return 10;

	case 'a': return 10;

	case 'B': return 11;

	case 'b': return 11;

	case 'C': return 12;

	case 'c': return 12;

	case 'D': return 13;

	case 'd': return 13;

	case 'E': return 14;

	case 'e': return 14;

	case 'F': return 15;

	case 'f': return 15;

	default: return 0;
	}
}

/*
** monCmdProcess
**
** Processes a command and wakes/suspends disks as required.
**
** Parameters: scs: command code in Short Command Set
**  func: pointer to command function
**  disk: whether a disk is connected or not
** Returns: value returned from func(),
**  MON_ERROR_CMD_FAILED if no function,
**  MON_ERROR_NO_DISK if command requires a disk.
** Comments:
*/
unsigned char monCmdProcess(unsigned short scs, FP func, unsigned char disk)
{
	// NOTE: MON_ERROR_BAD_CMD must never be returned here
	unsigned char status = MON_ERROR_CMD_FAILED;

	if (Mon_Flags & MON_SUD)
	{
		// Wake the disk prior to sending a command.
		hostWake(hUsb2);
	}

	// check for disk require command
	if ((scs >> 8) > 0)
	{
		if (disk == 0)
		{
			status = MON_ERROR_NO_DISK;
		}
	}

	if (status != MON_ERROR_NO_DISK)
	{
		if (func)
		{
			status = func();
		}
	}

	if (Mon_Flags & MON_SUD)
	{
		// Put the disk back into suspend after completing the command.
		hostSuspend(hUsb2);
	}

	return status;
}

/*
** monChkCmd
**
** Decodes a command from the command structure.
**
** Parameters: disk: whether a disk is connected or not
** Returns: value returned from monCmdProcess(),
**  MON_ERROR_BAD_CMD if command not recognised.
** Comments:
*/
unsigned char monChkCmd(unsigned char disk)
{
	unsigned int cmd = 0;
	int i = 0;
	int j = 0;
	char cr = 0x0d;
	char tDataRead = '\0';             // Use a temp value so that we don't have to alter what is in cmdData
	char result = MON_ERROR_BAD_CMD;

	// Extended Command Mode...
	// exception for ECS, E and e
	if ((Mon_Flags & MON_SCS) == 0)
	{
		while (cmdData[i] != '\0')
		{
			tDataRead = cmdData[i];

			if (cmdData[i] > 0x60 && cmdData[i] < 0x7B)
			{
				tDataRead = cmdData[i] - 0x20; // Convert into UPPERCASE.
			}

			cmd = cmd << 8 | tDataRead;

			i++;

			if (i == MAX_CMD_LEN)
				break;
		}

		// Search our data structure for the command.
		i = 0;

		while (commands[i].ecs != 0xFFFF)
		{
			// this line used for debugging
			j = commands[i].ecs;

			if (commands[i].ecs == cmd)
			{
				// Found our command within the data structure...
				result = monCmdProcess(commands[i].scs, commands[i].cmd_func, disk);
				break;
			}

			i++;
		}
	}

	if (Mon_Flags & MON_SCS)           // Short Command Mode...
	{
		cmd = cmdData[0];

		i = 0;

		while (commands[i].ecs != 0xFFFF)
		{
			if ((commands[i].scs & 0xff) == cmd)
			{
				// Found our command within the data structure...
				result = monCmdProcess(commands[i].scs, commands[i].cmd_func, disk);
				break;
			}

			i++;
		}
	}

	if (cmdTerminator != 0x0d)
	{
		if (result == MON_ERROR_BAD_CMD)
		{
			monReadCr();
		}
	}

	return result;
}

/*
** monChkCmd
**
** Reads a command in, decodes and performs the command, then displays a message
**  reporting the command status to the monitor.
**
** Parameters: disk: whether a disk is connected or not
** Returns: none
** Comments:
*/
void monCheck(unsigned char disk)
{
	unsigned char status;

	status = monReadCmd();

	if (status == MON_SUCCESS)
	{
		// got a command, now decrypt it...
		status = monChkCmd(disk);
		
		// turn off flashing green LED turned on in monChkCmd()
		if (disk) monLedsGreen();
		else monLedsOff();
	}

	if (status != MON_NO_CMD)
	{
		if (status == MON_ERROR_BAD_CMD)
		{
			monBadCommand();
		}
		else if (status == MON_ERROR_CMD_FAILED)
		{
			monCommandFailed();
		}
		else if (status == MON_ERROR_DISK_FULL)
		{
			monDiskFull();
		}
		else if (status == MON_ERROR_INVALID)
		{
			monInvalid();
		}
		else if (status == MON_ERROR_READ_ONLY)
		{
			monReadOnly();
		}
		else if (status == MON_ERROR_FILE_OPEN)
		{
			monFileOpen();
		}
		else if (status == MON_ERROR_DIR_NOT_EMPTY)
		{
			monDirNotEmpty();
		}
		else if (status == MON_ERROR_INVALID_FILENAME)
		{
			monFilenameInvalid();
		}
		else if (status == MON_ERROR_NO_DISK)
		{
			monNoDisk();
		}
		else
		{
			monPrompt();
		}
	}
}

/*
** monPowerLedOn
**
** Turn on the power LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monPowerLedOn()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;
#ifdef V2MSC_FIRMWARE
	monLEDstate &= (~0x10);
#elseif
	monLEDstate &= (~0x20);
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monPowerLedOff
**
** Turn off the power LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monPowerLedOff()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;	
#ifdef V2MSC_FIRMWARE
	monLEDstate |= 0x10;
#elseif
	monLEDstate |= 0x20;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monActivityLedOn
**
** Turn on the activity LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monActivityLedOn()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;
#ifdef V2MSC_FIRMWARE
	monLEDstate &= (~0x20);
#elseif
	monLEDstate &= (~0x40);
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monActivityLedOff
**
** Turn off the activity LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monActivityLedOff()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;
#ifdef V2MSC_FIRMWARE
	monLEDstate |= 0x20;
#elseif
	monLEDstate |= 0x40;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monLedsGreen
**
** Turn on green LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsGreen()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;
#ifdef V2MSC_FIRMWARE
	monLEDstate &= (~0x30);
	monLEDstate |= 0x10;
#elseif
	monLEDstate &= (~0x60);
	monLEDstate |= 0x20;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monLedsRed
**
** Turn on red LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsRed()
{
#ifdef CFGMONITOR_LEDS_FLASH
	monFlashLED = 0x00;	
#ifdef V2MSC_FIRMWARE
	monLEDstate &= (~0x30);
	monLEDstate |= 0x20;
#elseif
	monLEDstate &= (~0x60);
	monLEDstate |= 0x40;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monLedsOff
**
** Turn off all LEDs.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsOff()
{
#ifdef CFGMONITOR_LEDS_FLASH
	char data;
	
	monFlashLED = 0x00;	
#ifdef V2MSC_FIRMWARE
	monLEDstate = 0x30;
#elseif
	monLEDstate = 0x60;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}
	
/*
** monLedsOffNow
**
** Turn off all LEDs for suspend mode
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsOffNow()
{
#ifdef CFGMONITOR_LEDS_FLASH
	char data;
	monLedsOff();
	// turn the LEDs off right now (override thread)
	vos_gpio_read_port(GPIO_PORT_A, &data);
#ifdef V2MSC_FIRMWARE
	data |= 0x30;
#elseif
	data |= 0x60;
#endif // V2MSC_FIRMWARE	
	vos_gpio_write_port(GPIO_PORT_A, data);
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monLedsGreenFlash
**
** Begin flashing green LED.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monLedsGreenFlash()
{
#ifdef CFGMONITOR_LEDS_FLASH
#ifdef V2MSC_FIRMWARE
	monFlashLED = 0x10;
#elseif
	monFlashLED = 0x40;
#endif // V2MSC_FIRMWARE	
#endif	// CFGMONITOR_LEDS_FLASH
}

/*
** monLedsOff
**
** Turn off all LEDs.
**
** Parameters: none
** Returns: none
** Comments:
*/
void monFlashLedThread()
{
#ifdef CFGMONITOR_LEDS_FLASH
	unsigned char data;

	while (1)
	{
		vos_delay_msecs(125);
		if (monFlashLED)
		{
			vos_gpio_read_port(GPIO_PORT_A, &data);
			data ^= monFlashLED;
			vos_gpio_write_port(GPIO_PORT_A, data);
		}
		else
		{
			vos_gpio_read_port(GPIO_PORT_A, &data);
#ifdef V2MSC_FIRMWARE
			data &= (~0x30);
#elseif
			data &= (~0x60);
#endif // V2MSC_FIRMWARE	
			data |= monLEDstate;
			vos_gpio_write_port(GPIO_PORT_A, data);
		}
	}
#endif	// CFGMONITOR_LEDS_FLASH
}
