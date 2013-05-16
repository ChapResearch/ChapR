
#ifndef BT_h
#define BT_h

#include <Arduino.h>
//#include <stdint.h>
#include <SoftwareSerial.h>

//
// these defines are used to the operational baud for the BT interface
//
#define BT_OP_BAUD	38400
#define BT_SU_BAUD     	"38"
#define BT_U_BAUD	"38.4"

//
// Note that an inquiry scan returns (for example)	:
//	Inquiry,T=5,COD=0
//	Found 2
//	0002762A6CAE,ERIC-REDFOOT,3E010C
//	00165300C332,NXT - A,804
//	Inquiry Done
//

typedef enum { BT_BLANK,	// a blank line "\r"
	       BT_AOK,		// the "AOK\r" response from connection attempts
	       BT_CMD,		// the "CMD\r" prompt
	       BT_TRYING,	// when a connect is being attempted
	       BT_INQUIRYDONE,	// the "Inquiry Done\r" token 
	       BT_CONNECT,	// the "CONNECT\r" token
	       BT_CONNECTFAILED,// the "CONNECT failed\r" token
	       BT_ERR,		// the "ERR\r" token

	       BT_OTHER		// something of the form "(stuff)\r" was returned
} btResponse;

class BT : public SoftwareSerial
{
public:

     BT(uint8_t receive,
	uint8_t transmit, 
	uint8_t reset, 		// TO BT> used to reset the BT module (necessary?)
	uint8_t auto, 		// TO BT> sets auto connect mode - off when pairing
	uint8_t baud,		// TO BT> sets 9600 baud mode
	uint8_t connect);	// FROM BT< high when connected

     void opMode();
     void configMode(char *);
     void zombieMode();
     bool connected();		// returns true if connected
     void btWrite(byte *buffer, int size);

// BIG NOTE - the pins on the RN-42 need voltage dividers - but the current
//		implementation ** Sun Apr 21 14:47:33 2013 ** doesn't have
//		them - and have the wierd 1 resistor implementation.  this
//		needs to be fixed.

private:
     uint8_t   _rstPin;			// output to RN-42
     uint8_t   _autoModePin;		// output to RN-42
     uint8_t   _9600BaudPin;		// output to RN-42
     uint8_t   _connectPin;		// input from RN-42

     void baud9600mode(bool);
     void autoConnectMode(bool);
     void reset();
     void specialPin(int,int);
     void btSend(char *string);
};

#endif /* BT_h */





