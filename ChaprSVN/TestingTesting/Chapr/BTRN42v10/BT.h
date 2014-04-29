
#ifndef BT_h
#define BT_h

#include <Arduino.h>
//#include <stdint.h>
#include <SoftwareSerial.h>

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

BT(uint8_t receive,uint8_t transmit, uint8_t reset);

     void reset();
     void sync();


private:
     uint8_t   _txPin;
     uint8_t   _rxPin;
     uint8_t   _rstPin;
};

#endif /* BT_h */





