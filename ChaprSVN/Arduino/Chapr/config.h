//
// config.h
//
//   Configuration for the ChapR board
//
// there were two different major versions of the board, with two different
// pin assignments.  we are using v02 now.  V01 is around for reference for
// the old board.

#define V02


#ifdef V01

#define BT_9600BAUD 	A5
#define BT_CONNECTED	A4
#define BT_MODE		A3
#define BT_RX		12
#define BT_TX		13
#define BT_RESET	11

#define VDIP_CLOCK	6
#define VDIP_MOSI	7
#define VDIP_MISO	8
#define VDIP_CS		9
#define VDIP_RESET	10

#define LED_POWER	A2
#define LED_INDICATE	A1

#define BUTTON		A0
#define TONEPIN		5

#endif

#ifdef V02

#define BT_9600BAUD 	5		// (==>BT) forces BT module to 9600 baud mode
#define BT_CONNECTED	3		// (<==BT) BT module asserts when connect to BT
#define BT_MODE		4		// (==>BT) sets the auto connect mode
#define BT_RX		12
#define BT_TX		13
#define BT_RESET	11

#define VDIP_CLOCK	6
#define VDIP_MOSI	7
#define VDIP_MISO	8
#define VDIP_CS		9
#define VDIP_RESET	10

#define LED_POWER	A2
#define LED_INDICATE	A1

#define BUTTON		A0
#define TONEPIN		A3

#define POWER_ON_HOLD	A4		// (==>board) keeps the power on, lower to turn off
#define POWER_BUTTON	A5		// (<==button) asserted when pressed

#endif


//
// the baud rate that the local serial port will operate in - talking to the IDE terminal
//
#define LOCAL_SERIAL_BAUD	38400

#define PERSONALITYCOUNT        3 //how many personalities have been implemented
#define BOARDBRINGUPVERSION     "1.1" //which version the testing program is
#define CODEVERSION             "1.2"

//these two constants define the time before entering power saving mode and are in milliseconds
// (the second commented out versions are for debugging)

#define LOWPOWERTIMEOUT 300000
//#define LOWPOWERTIMEOUT 10000
#define ZMODETIMEOUT 600000
//#define ZMODETIMEOUT 20000

#define POWEROFFHOLDDOWN 2000 //this is measured in milliseconds
