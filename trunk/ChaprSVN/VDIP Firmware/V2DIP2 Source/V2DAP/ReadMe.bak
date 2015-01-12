VNC1L V2DAP Firmware Readme
---------------------------

Fixed Issues:
- Correct output of DIR command to show . and .. directories when current directory is not the root directory.
- Corrected pin configuration for FIFO interface data mode selection on 48-pin device.
- Updated drivers.
- Handle multi-partition and multi-LUN disks better.
- Fixed reflash disk test to only update ROM if a different revision of ROM file is found on disk.
  The VinUser utility can be used to modify version numbers of ROM files.

Restrictions:
- When using with a V2EVAL board Rev 1 always REMOVE jumper JP9.
- V2DAP CDC driver not implemented therefore data-mode use of CDC devices not supported.
- ATAPI flash disks are not supported. SCSI flash disks with a sector size of 512 bytes are supported.
- EEPROM commands for FT232 devices connected USB Host are not supported in V2DAP or V2DPS. USBHostFT232 Driver allows the EEPROM data to be changed.
- V2DPS and V2DAP firmware must be compiled at optimisation level 1 or above due to space restriction in the ROM. This affects some watch variables while debugging. A solution to allow full debugging will be provided at a later date.

Known Issues:
- N/A

