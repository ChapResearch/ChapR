
extern int nxtMsgCompose(byte *output, 		// the output buffer to scribble things to - min 22 bytes
			 byte UserMode,		// the usermode value
			 byte StopPgm,		// the wait-for-start value
			 byte *USBJoystick1,	// buffers for the two joysticks
			 byte *USBJoystick2);
