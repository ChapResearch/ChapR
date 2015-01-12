/*
** cmddisk.c
**
** Copyright © 2010-2011 Future Technology Devices International Limited
**
** C Source file for Vinculum II V2DAP Disk Commands
** in the Command Interpreter
**
** Author: FTDI
** Project: Vinculum II
** Module: Command Monitor
** Requires: VOS
** Comments: Refer to VNC1L Firmware User Manual for detailed command information
**
** History:
**  1 – Initial version
**
*/
#include "vos.h"

#include "monitor.h"
#include "cmdmon.h"
#include "disk.h"

#include "USBHost.h"
#include "msi.h"
#include "BOMS.h"
#include "FAT.h"

extern fat_context fatContext;

file_context_t FILE;

// File state - records whether a file if open for read or for write
// 0 for no files open, 1 for open for read and 2 for write
#define FILE_STATE_NOT_OPEN	  0
#define FILE_STATE_OPEN_READ  1
#define FILE_STATE_OPEN_WRITE 2
unsigned char fileState;

/*
** diskInit
**
** Initialise the file state
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
void diskInit()
{
	fileState = 0;
}

/*
** cmd_dir
**
** Perform a directory command
**
** Parameters: param1: optional file name
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if no file found
** Comments: If a file is supplied, then the file size for that file is shown.
**  If no file then a directory listing is shown.
**  The dir command will always return a carriage return before any output.
*/
unsigned char cmd_dir()
{
	char cr = 0x0d;
	char sp = ' ';
	char *strDIR = " DIR";
	unsigned char result;
	file_context_t fileFind;
	unsigned long size;

	// This function can either have 1 or 0 parameters...
	if (cmdTerminator == ' ')
	{
		// filename specified - show file size
		monReadFileName(1);
		// purge until CR
		monReadCr();

		monWrite(&cr, 1);

		// Cannot perform dir while playing music....
		if (fileState == FILE_STATE_OPEN_WRITE) {
			return MON_ERROR_FILE_OPEN; 
		}

		result = monValidateFileName(1);

		if (result == MON_SUCCESS)
		{
			// We have a valid parameter.
			if (fat_dirTableFind(fatContext, &fileFind, (char *) param1Data) == FAT_OK)
			{
				size = fat_dirEntrySize(&fileFind);
				monWriteFileName((char *) &fileFind);
				monWrite(&sp, 1);
				monAddNumberToConsole((unsigned char *) &size, 4);
				monWrite(&cr, 1);
			}
			else
			{
				result = MON_ERROR_CMD_FAILED;
			}
		}
	}
	else
	{
		// purge until CR
		monReadCr();
		monWrite(&cr, 1);

		// Cannot perform dir while playing music....
		if (fileState == FILE_STATE_OPEN_WRITE) {
			return MON_ERROR_FILE_OPEN; 
		}
		
		// Subdirectory output will report . and .. directories
		if (!fat_dirIsRoot(fatContext))
		{
			monWriteString(".");
			monWriteString(strDIR);
			monWrite(&cr, 1);
			monWriteString("..");
			monWriteString(strDIR);
			monWrite(&cr, 1);
		}		

		// no filename specified - directory listing
		if (fat_dirTableFindFirst(fatContext, &fileFind) == FAT_OK)
		{
			do
			{
				if (fat_dirEntryIsValid(&fileFind))
				{
					monWriteFileName((char *) &fileFind);

					if (fat_dirEntryIsDirectory(&fileFind))
					{
						monWriteString(strDIR);
					}

					monWrite(&cr, 1);
				}
			}
			while (fat_dirTableFindNext(fatContext, &fileFind) == FAT_OK);
		}

		result = MON_SUCCESS;
	}

	return result;
}

/*
** cmd_cd
**
** Change directory.
**
** Parameters: param1: directory to change current directory to
** Returns: MON_SUCCESS, MON_ERROR_CMD_FAILED if  directory not found
** Comments: '/' will change to the root directory.
**  '..' will change up one directory.
*/
unsigned char cmd_cd()
{
	unsigned char result;
	file_context_t fileFind;

	// filename specified:
	monReadFileName(1);
	// purge until CR
	monReadCr();
	
	// check for open file...
	if (fileState != FILE_STATE_NOT_OPEN)
	{
		result = MON_ERROR_FILE_OPEN;
	}
	else
	{
		result = monValidateFileName(1);

		if (result == MON_SUCCESS)
		{
			if (param1Data[0] == '/')
			{
				if (fat_dirChangeDir(fatContext, NULL) != FAT_OK)
				{
					result = MON_ERROR_CMD_FAILED;
				}
			}
			else
			{
				if (fat_dirChangeDir(fatContext, param1Data) != FAT_OK)
				{
					result = MON_ERROR_CMD_FAILED;

					if (fat_fileOpen(fatContext, &fileFind, param1Data, FILE_MODE_HANDLE) == FAT_OK)
					{
						// check if handle is a not directory (different error code)
						if (!fat_dirEntryIsDirectory(&fileFind))
						{
							result = MON_ERROR_INVALID;
						}
					}
				}
			}
		}
	}
	
	return result;
}

/*
** cmd_rd
**
** Read a file and display contents to the monitor.
**
** Parameters: param1: file name
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is already open
**  MON_ERROR_INVALID_FILENAME could not find the specified file,
**  MON_ERROR_INVALID the file specified is not a readable file,
**  MON_ERROR_CMD_FAILED could not read the file.
** Comments:
*/
unsigned char cmd_rd()
{
	char *dataBuffer;                  // Data buffer containing the data read back from the file.
	unsigned long size;
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		if (monValidateFileName(1) == MON_SUCCESS)
		{
			// Fat handle for the file.
			status = fat_dirTableFind(fatContext, &FILE, (char *) param1Data);

			// Get the size of the file
			if (status != FAT_OK)
			{
				status = MON_ERROR_CMD_FAILED;
			}
			else
			{
				size = fat_dirEntrySize(&FILE);
				// Open the selected file for reading...
				status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_READ);

				if (status == FAT_INVALID_FILE_TYPE)
				{
					status = MON_ERROR_CMD_FAILED;
				}
				else
				{
					status = fat_fileRead(&FILE, size, NULL, hMonitor, NULL);

					if (status == FAT_OK)
					{
						status = MON_SUCCESS;
					}
					else
					{
						status = MON_ERROR_CMD_FAILED;
					}
				}

				fat_fileClose(&FILE);  // Close the file after reading.
			}
		}
		else
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
	}

	return status;
}

/*
** cmd_dld
**
** Delete a directory.
**
** Parameters: param1: directory name to delete
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is already open
**  MON_ERROR_INVALID_FILENAME name is not a valid directory name,
**  MON_ERROR_INVALID the name specified is not a directory,
**  MON_ERROR_CMD_FAILED could not read the file,
**  MON_ERROR_DIR_NOT_EMPTY the directory is not empty.
** Comments:
*/
unsigned char cmd_dld()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			if (fat_dirTableFind(fatContext, &FILE, (char *) param1Data) == FAT_OK)
			{
				if (fat_dirEntryIsDirectory(&FILE))
				{
					if (fat_dirDirIsEmpty(&FILE))
					{
						if (fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_HANDLE) == FAT_INVALID_FILE_TYPE)
						{
							status = MON_ERROR_INVALID;
						}
						else
						{
							fat_fileDelete(&FILE);
							fat_fileClose(&FILE);
							status = MON_SUCCESS;
						}
					}
					else
					{
						status = MON_ERROR_DIR_NOT_EMPTY;
					}
				}
				else
				{
					status = MON_ERROR_INVALID;
				}
			}
			else
			{
				status = MON_ERROR_CMD_FAILED;
			}
		}
	}

	return status;
}

/*
** cmd_mkd
**
** Make a new directory.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is already open
**  MON_ERROR_INVALID_FILENAME name is not a valid directory name,
**  MON_ERROR_DISK_FULL no space left on disk,
**  MON_ERROR_CMD_FAILED could not create the new directory.
** Comments:
*/
unsigned char cmd_mkd()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);

	if (cmdTerminator == ' ')
	{
		monReadNumber(2, 4);
		monValidateNumber(2, 4);
		// set create time for new directory
		fat_time(param2);
	}
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			status = fat_dirCreateDir(fatContext, param1Data);

			if (status == FAT_EXISTS)
			{
				status = MON_ERROR_CMD_FAILED;
			}
			else if (status == FAT_DIRECTORY_TABLE_FULL)
			{
				status = MON_ERROR_DISK_FULL;
			}
			else
			{
				status = MON_SUCCESS;
			}
		}
	}

	return status;
}

/*
** cmd_dlf
**
** Delete a file.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is already open
**  MON_ERROR_INVALID_FILENAME name is not a valid file name,
**  MON_ERROR_INVALID could not find file,
**  MON_ERROR_CMD_FAILED could not delete file.
** Comments:
*/
unsigned char cmd_dlf()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			if (fat_dirTableFind(fatContext, &FILE, (char *) param1Data) == FAT_OK)
			{
				if (fat_dirEntryIsFile(&FILE))
				{
					status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_HANDLE);

					if (status == FAT_INVALID_FILE_TYPE)
					{
						status = MON_ERROR_INVALID;
					}
					else
					{
						fat_fileDelete(&FILE);
						fat_fileClose(&FILE);
						status = MON_SUCCESS;
					}
				}
				else
				{
					status = MON_ERROR_INVALID;
				}
			}
			else
			{
				status = MON_ERROR_CMD_FAILED;
			}
		}
	}

	return status;
}

/*
** cmd_wrf
**
** Write to an open file.
**
** Parameters: param1: number of bytes to write.
** Returns: MON_SUCCESS, MON_ERROR_INVALID no file is open or could not read from file,
**  MON_ERROR_CMD_FAILED could not write to file file.
**  MON_ERROR_DISK_FULL if there is no space on the disk.
** Comments:
*/
unsigned char cmd_wrf()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadNumber(1, 4);
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_OPEN_WRITE)
	{
		status = MON_ERROR_INVALID;
		// TODO: read in and ignore number of bytes requested
	}
	else
	{
		monValidateNumber(1, 4);

		status = fat_fileWrite(&FILE, param1, NULL, hMonitor, NULL);

		if (status == FAT_NOT_OPEN)
		{
			status = MON_ERROR_INVALID;
		}
		else if (status == FAT_EOF)
		{
			status = MON_ERROR_CMD_FAILED;
		}
		else if (status == FAT_DISK_FULL)
		{
			status = MON_ERROR_DISK_FULL;
		}
		else if (status != FAT_OK)
		{
			status = MON_ERROR_CMD_FAILED;
		}
		else
		{
			status = MON_SUCCESS;
		}
	}

	return status;
}

/*
** cmd_opw
**
** Open a file for writing.
**
** Parameters: param1: filename to write
**  param2: optional date and time
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN no file is open,
**  MON_ERROR_INVALID_FILENAME name is not a valid file name,
**  MON_ERROR_INVALID could not find file,
**  MON_ERROR_DISK_FULL no space left on disk,
**  MON_ERROR_CMD_FAILED could not delete file.
** Comments:
*/
unsigned char cmd_opw()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);

	if (cmdTerminator == ' ')
	{
		monReadNumber(2, 4);
		monValidateNumber(2, 4);
		// set create time for new file
		fat_time(param2);
	}
	// purge until CR
	monReadCr();
	
	if (fileState == FILE_STATE_OPEN_WRITE)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_READ_PLUS);

			if (status == FAT_NOT_FOUND)
			{
				status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_WRITE_PLUS);
			}

			if (status == FAT_READ_ONLY)
			{
				status = MON_ERROR_READ_ONLY;
			}
			else if (status == FAT_INVALID_FILE_TYPE)
			{
				status = MON_ERROR_INVALID;
			}
			else if (status == FAT_DIRECTORY_TABLE_FULL)
			{
				status = MON_ERROR_DISK_FULL;
			}
			else if (status != FAT_OK)
			{
				status = MON_ERROR_INVALID;
			}
			else
			{
				status = fat_fileSeek(&FILE, 0, FAT_SEEK_END);

				if (status != FAT_OK)
				{
					status = MON_ERROR_CMD_FAILED;
				}

				fileState = FILE_STATE_OPEN_WRITE;
				status = MON_SUCCESS;
			}
		}
	}

	return status;
}

/*
** cmd_clf
**
** Close a file
**
** Parameters: param1: optional filename of open file to close
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_clf()
{
	if (cmdTerminator == ' ')
	{
		monReadFileName(1);
	}
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		fat_fileClose(&FILE);
	}

	fileState = FILE_STATE_NOT_OPEN;
	return MON_SUCCESS;
}

/*
** cmd_rdf
**
** Read a specified number of bytes from a file.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_INVALID no file open to read from or could not read from file,
**  MON_ERROR_CMD_FAILED end of file reached.
** Comments:
*/
unsigned char cmd_rdf()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadNumber(1, 4);
	// purge until CR
	monReadCr();

	if (fileState == FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_INVALID;
	}
	else
	{
		monValidateNumber(1, 4);

		status = fat_fileRead(&FILE, param1, NULL, hMonitor, NULL);

		if (status == FAT_NOT_OPEN)
		{
			status = MON_ERROR_INVALID;
		}
		else if (status == FAT_EOF)
		{
			status = MON_ERROR_CMD_FAILED;
		}
		else if (status != FAT_OK)
		{
			status = MON_ERROR_CMD_FAILED;
		}
		else
		{
			status = MON_SUCCESS;
		}
	}

	return status;
}

/*
** cmd_ren
**
** Rename a file
**
** Parameters: param1: file to rename
**  param2: new name for file
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is open,
**  MON_ERROR_INVALID_FILENAME name is not a valid file name,
**  MON_ERROR_INVALID could not find file,
**  MON_ERROR_CMD_FAILED could not rename file.
** Comments:
*/
unsigned char cmd_ren()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);
	monReadFileName(2);
	// purge until CR
	monReadCr();

	if (fileState != FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			status = monValidateFileName(2);

			if (status != MON_SUCCESS)
			{
				status = MON_ERROR_INVALID_FILENAME;
			}
			else
			{
				status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_HANDLE);

				if (status != FAT_OK)
				{
					status = MON_ERROR_INVALID;
				}
				else
				{
					fat_fileRename(&FILE, (char *) param2Data);
					fat_fileClose(&FILE);
					status = MON_SUCCESS;
				}
			}
		}
	}

	return status;
}

/*
** cmd_opr
**
** Open a file for reading.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_FILE_OPEN a file is already open,
**  MON_ERROR_INVALID_FILENAME name is not a valid file name,
**  MON_ERROR_INVALID could not find file,
**  MON_ERROR_CMD_FAILED could not delete file.
** Comments:
*/
unsigned char cmd_opr()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadFileName(1);

	if (cmdTerminator == ' ')
	{
		monReadNumber(2, 2);
		monValidateNumber(2, 2);
		// move date into high bytes of long date/time
		param2 <<= 16;
		// set create time for new file
		fat_time(param2);
	}
	// purge until CR
	monReadCr();

	if (fileState == FILE_STATE_OPEN_WRITE)
	{
		status = MON_ERROR_FILE_OPEN;
	}
	else
	{
		status = monValidateFileName(1);

		if (status != MON_SUCCESS)
		{
			status = MON_ERROR_INVALID_FILENAME;
		}
		else
		{
			status = fat_fileOpen(fatContext, &FILE, (char *) param1Data, FILE_MODE_READ);

			if (status != FAT_OK)
			{
				status = MON_ERROR_INVALID;
			}
			else
			{
				fileState = FILE_STATE_OPEN_READ;
				status = MON_SUCCESS;
			}
		}
	}

	return status;
}

/*
** cmd_sek
**
** Seek to an offset in a file.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_INVALID no file is open or could not perform seek.
** Comments: Offset is specified from the start of the file
*/
unsigned char cmd_sek()
{
	unsigned char status = MON_ERROR_CMD_FAILED;

	monReadNumber(1, 4);
	// purge until CR
	monReadCr();

	if (fileState == FILE_STATE_NOT_OPEN)
	{
		status = MON_ERROR_INVALID;
	}
	else
	{
		monValidateNumber(1, 4);

		// seek in monitor is actually setpos
		if (fat_fileSetPos(&FILE, param1) != FAT_OK)
		{
			status = MON_ERROR_INVALID;
		}
		else
		{
			status = MON_SUCCESS;
		}
	}

	return status;
}

/*
** cmd_fs, cmd_fse
**
** Report free space on disk.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_fs_handler(unsigned char ext)
{
	unsigned long bytes_l, bytes_h, x;
	char cr = 0x0d;

	// no- parameters purge input until CR
	monReadCr();

	fat_freeSpace(fatContext, &bytes_h, &bytes_l, FAT_FREESPACE_SCAN);

	if (ext == 0)
	{
		if (bytes_h)
		{
			bytes_l = 0xffffffff;
		}
	}

	monAddNumberToConsole((unsigned char *) &bytes_l, 4);

	if (ext)
	{
		monAddNumberToConsole((unsigned char *) &bytes_h, 2);
	}

	monWrite(&cr, 1);

	return MON_SUCCESS;
}

unsigned char cmd_fs()
{
	return cmd_fs_handler(0);
}

unsigned char cmd_fse()
{
	return cmd_fs_handler(1);
}

/*
** cmd_idd, cmd_idde
**
** Display disk identity information.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments:
*/
unsigned char cmd_idd_handler(unsigned char ext)
{
	unsigned long bytes_l, bytes_h;
	unsigned short bytes;
	unsigned long doublelong[2];
	short cr = 0x0d0d;

	msi_ioctl_cb_t disk_iocb;
	msi_ioctl_cb_info_t disk_iocb_info;
	VOS_HANDLE hDisk;
	char *strIDD1 = "\rUSB VID = $";
	char *strIDD2 = "\rUSB PID = $";
	char *strIDD3 = "\rVendor Id = ";
	char *strIDD4 = "\rProduct Id = ";
	char *strIDD5 = "\rRevision Level = ";
	char *strIDD6 = "\rI/F = SCSI";
	char *strIDDFAT32 = "\rFAT32";
	char *strIDDFAT16 = "\rFAT16";
	//char *strIDDFAT12 = "FAT12"; not used
	char *strIDD7 = "\rBytes/Sector = $";
	char *strIDD8 = "\rBytes/Cluster = $";
	char *strIDD9 = "\rCapacity = $";
	char *strIDD0 = "\rFree Space = $";
	char *strIDDBytes = " Bytes";

	hDisk = fat_getDevHandle(fatContext);
	disk_iocb.ioctl_code = MSI_IOCTL_GET_DEV_INFO;
	disk_iocb.get = &disk_iocb_info;
	vos_dev_ioctl(hDisk, &disk_iocb);

	monWriteString(strIDD1);
	monAddMultibyteNumberToConsole((unsigned char *) &disk_iocb_info.vid, 2);
	monWriteString(strIDD2);
	monAddMultibyteNumberToConsole((unsigned char *) &disk_iocb_info.pid, 2);
	monWriteString(strIDD3);
	monWrite((unsigned char *) &disk_iocb_info.vendorId, 8);
	monWriteString(strIDD4);
	monWrite((unsigned char *) &disk_iocb_info.productId, 16);
	monWriteString(strIDD5);
	monWrite((unsigned char *) &disk_iocb_info.rev, 4);
	monWriteString(strIDD6);

	switch (fat_getFSType(fatContext))
	{
	//case 0x12: monWriteString(strIDDFAT12);
		//break;

	case 0x16: monWriteString(strIDDFAT16);
		break;

	case 0x32: monWriteString(strIDDFAT32);
		break;
	}

	fat_bytesPerSector(fatContext, &bytes);
	monWriteString(strIDD7);
	monAddMultibyteNumberToConsole((unsigned char *) &bytes, 2);
	fat_bytesPerCluster(fatContext, &bytes_h);
	monWriteString(strIDD8);
	monAddMultibyteNumberToConsole((unsigned char *) &bytes_h, 3);
	fat_capacity(fatContext, &bytes_h, &bytes_l);

	if (ext == 0)
	{
		if (bytes_h)
		{
			bytes_l = 0xffffffff;
		}
	}

	monWriteString(strIDD9);

	if (ext)
	{
		doublelong[0] = bytes_l;
		doublelong[1] = bytes_h;
		monAddMultibyteNumberToConsole((unsigned char *) doublelong, 6);
	}
	else
		monAddMultibyteNumberToConsole((unsigned char *) &bytes_l, 4);

	monWriteString(strIDDBytes);
	monWriteString(strIDD0);

	if (fat_freeSpace(fatContext, &bytes_h, &bytes_l, FAT_FREESPACE_NO_SCAN) == FAT_OK)
	{
		if (ext == 0)
		{
			if (bytes_h)
			{
				bytes_l = 0xffffffff;
			}
		}

		if (ext)
		{
			doublelong[0] = bytes_l;
			doublelong[1] = bytes_h;
			monAddMultibyteNumberToConsole((unsigned char *) doublelong, 6);
		}
		else
			monAddMultibyteNumberToConsole((unsigned char *) &bytes_l, 4);
	}

	monWriteString(strIDDBytes);
	monWrite(&cr, 2);

	return MON_SUCCESS;
}

unsigned char cmd_idd()
{
	return cmd_idd_handler(0);
}

unsigned char cmd_idde()
{
	return cmd_idd_handler(1);
}

/*
** cmd_dsn
**
** Show disk serial number.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Unique 32 bit identity number for the disk.
*/
unsigned char cmd_dsn()
{
	char cr = 0x0d;
	unsigned long id;

	fat_getVolumeID(fatContext, &id);

	monAddNumberToConsole((unsigned char *) &id, 4);
	monWrite(&cr, 1);

	return MON_SUCCESS;
}

/*
** cmd_dvl
**
** Show Disk volume label.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Does not show FAT32 labels.
*/
unsigned char cmd_dvl()
{
	char cr = 0x0d;
	char label[11];

	fat_getVolumeLabel(fatContext, label);

	monWrite(label, 11);
	monWrite(&cr, 1);

	return MON_SUCCESS;
}

/*
** cmd_dirt
**
** Show file create, modify and access times.
**
** Parameters: none
** Returns: MON_SUCCESS, MON_ERROR_INVALID_FILENAME name is not a valid file name,
**  MON_ERROR_CMD_FAILED could not show time information for file.
** Comments:
*/
unsigned char cmd_dirt()
{
	char cr = 0x0d, sp = ' ';
	file_context_t fileFind;
	unsigned char result = MON_ERROR_CMD_FAILED;
	void *findHandle;
	unsigned short date;
	unsigned char i;

	// filename specified:
	monReadFileName(1);
	// purge until CR
	monReadCr();

	result = monValidateFileName(1);

	if (result == MON_SUCCESS)
	{
		// We have a valid parameter.
		if (fat_dirTableFind(fatContext, &fileFind, (char *) param1Data) == FAT_OK)
		{
			monWriteFileName((char *) &fileFind);
			monWrite(&sp, 1);

			date = fat_dirEntryTime(&fileFind, FAT_DIRENTRYTIME_CREATE_TIME);
			monAddNumberToConsole((unsigned char *) &date, 2);
			date = fat_dirEntryTime(&fileFind, FAT_DIRENTRYTIME_CREATE_DATE);
			monAddNumberToConsole((unsigned char *) &date, 2);
			date = fat_dirEntryTime(&fileFind, FAT_DIRENTRYTIME_ACCESS_DATE);
			monAddNumberToConsole((unsigned char *) &date, 2);
			date = fat_dirEntryTime(&fileFind, FAT_DIRENTRYTIME_MODIFY_TIME);
			monAddNumberToConsole((unsigned char *) &date, 2);
			date = fat_dirEntryTime(&fileFind, FAT_DIRENTRYTIME_MODIFY_DATE);
			monAddNumberToConsole((unsigned char *) &date, 2);
			monWrite(&cr, 1);
		}
		else
		{
			result = MON_ERROR_CMD_FAILED;
		}
	}
	else
	{
		result = MON_ERROR_INVALID_FILENAME;
	}

	return result;
}

/*
** cmd_dri
**
** Disk reset.
**
** Parameters: none
** Returns: MON_SUCCESS
** Comments: Resets the state of the disk.
*/
unsigned char cmd_dri()
{
	// no- parameters purge input until CR
	monReadCr();

	diskResetDisk();

	return MON_SUCCESS;
}

