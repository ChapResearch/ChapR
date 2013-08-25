

#define NXT_DIR_CMD    '\x00'		// Direct command - reply required
#define NXT_SYS_CMD    '\x01'		// System command - reply required
#define NXT_REPLY_CMD  '\x02'		// Reply command

#define NXT_DIR_CMD_NR '\x80'		// Direct command - NO REPLY
#define NXT_SYS_CMD_NR '\x81'		// System command - NO REPLY

#define NXT_GET_DEV_INFO '\x9b'		// Get Device Info command
#define NXT_REBOOT       '\x97'        //Reboot system command

#define NXT_REBOOT_MAGICSTRING  "Let's dance: SAMBA"  //string for booting NXT


extern int nxtMsgCompose(byte *output, 		// the output buffer to scribble things to - min 22 bytes
			 byte UserMode,		// the usermode value
			 byte StopPgm,		// the wait-for-start value
			 byte *USBJoystick1,	// buffers for the two joysticks
			 byte *USBJoystick2);

extern bool nxtQueryDevice(VDIP *, int, char **, char **, long *);
