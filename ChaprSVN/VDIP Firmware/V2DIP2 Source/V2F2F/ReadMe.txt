VNC1L VF2F2 Firmware Readme
---------------------------

Fixed Issues:
- Updated drivers.

Restrictions:
- ATAPI flash disks are not supported. SCSI flash disks with a sector size of 512 bytes are supported.
- LEDs do not flash during enumeration.
- V2F2F does not wait for GPIO input before starting. This was an input switch on the VNC1L version to start copying. Once powered up and the USB buses are enumerated then the firmware will start a copy.

Known Issues:
- N/A

