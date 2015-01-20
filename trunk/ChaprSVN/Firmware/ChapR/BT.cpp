#include <Arduino.h>
#include "config.h"
#include "BT.h"
#include "SoftwareSerial.h"

//
// constructor
//
BT::BT(uint8_t receive,
       uint8_t transmit,
       uint8_t reset, 		// TO BT> used to reset the BT module (necessary?)
       uint8_t automode,	// TO BT> sets auto connect mode - off when pairing
       uint8_t baud,		// TO BT> sets 9600 baud mode
       uint8_t connect)	:	// FROM BT< high when connected
     SoftwareSerial(receive,transmit),
     _rstPin(reset),
     _autoModePin(automode),
     _9600BaudPin(baud),
     _connectPin(connect)
{
     pinMode(_connectPin,INPUT);

     // the following pins are wrong on the board - they use a 1 resistor
     // mechanism that needs to be replaced by a 2 resistor voltage divider
     // like that used on the TX pin.  when that is fixed, the following
     // will be used:
     //
     //		pinMode(_rstPin,OUTPUT);
     //		pinMode(_autoModePin,OUTPUT);
     //		pinMode(_9600BaudPin,OUTPUT);


     pinMode(_rstPin,INPUT);

#ifdef V01
     pinMode(_autoModePin,INPUT);
     pinMode(_9600BaudPin,INPUT);
#endif

#ifdef V02
    pinMode(_autoModePin,OUTPUT);
    pinMode(_9600BaudPin,OUTPUT);
#endif

}

//
// btSpecialPin() - the three pins above need to be fixed on the board
//			to a voltage divider.  Until then, this call
//			needs to be used for these pins.  when that is
//			fixed, blow away this routine.
//
void BT::specialPin(int pin, int value)
{
     if (value == HIGH) {
	  pinMode(pin,INPUT);		// lets the pull-up resistor take over
	                                // high-impedance mode
     } else {
	  digitalWrite(pin,LOW);	// drive the low first
	  pinMode(pin,OUTPUT);		// then turn it into an output
     }
}

//
// reset() - reset the bluetooth device - note that it is separate
//		from the sync/baud rate setup.  Use sync() for that.
//		Note that at least 500 ms needs to lapse after this
//		call before sync() is called.
//
//	** Thu Oct 24 14:49:17 2013 **
//	EJR - I moved the delays to inside the reset so they can be applied
//		consistently. 
//
void BT::reset()
{
     delay(100);

     specialPin(_rstPin,LOW);		// to be changed to digitalWrite(_rstPin,LOW)

     // needs to be low for min 160us, but 10 ms works great

     delay(10);

     specialPin(_rstPin,HIGH);		// to be changed to digitalWrite(_rstPin,HIGH)

     // reset seems to need a nice long wait before the RN-42 will respond well
     // to serial input - I'm seeing a weird immediate echo on the TX line if I
     // don't wait for this long

     delay(750);
}

//
// BT MODES -------------------------------------------------------
//
//   The BT RN-42 module only has a UART connection, so getting the
// thing up and running requires that you connect with the RN-42 at
// the same speed.  The devices always comes from the factory set at
// 115200 baud.  This would be a fine speed to communicate at, EXCEPT
// that we need to use the SoftwareSerial Arduino library to talk to
// it, and that thing can't reliably do 115200 on a 16MHz Uno R3.
// From all reports, it can do 38400 reliably (send and recv).
//
//   So, we, instead, operate the RN-42 in two modes "configation"
// mode and "operational" mode.  The configuration mode is only
// entered when needed, and the "operational" mode is the normal mode
// of operation.  Here how it works:
//
//   CONFIG mode is entered whenever the ChapR needs to pair with a
// brick.  CONFIG mode is entered by pressing and holding the button
// upon boot.
//
// NOTE that the following pins are defined:
//
//	GPIO6	- the _autoModePin
//	GPIO7	- the _9600BaudPin
//	GPIO2	- the _connetPin
//
// When CONFIG mode is requested, the following things occur:
//
//	pull LOW GPIO6	  // disconnects and terminates auto connect mode (has pull-up)
//	allow HIGH GPIO7  // (normally held low) sets 9600 baud
//	send "$$$"	  // enters command mode (after each command, wait)
// 	SU,38		  // sets baud to 38400 into firmware for next time
//	SN,<name>	  // sets the name (use the EEPROM stored value)
//	SM,4		  // sets the auto connect mode for next time
//	U,38.4,N	  // switches to 38400 and exits command mode
//	pull LOW GPIO7	  // removes 9600 baud mode
//
//
// At this point, wait for a connect just like normal.  HOWEVER,
// we are discoverable because we've left GPIO6 LOW.  This means
// that it may not try to reconnect after the first time with a
// connection.  We MAY want to change this to notice the connection
// the first time, and re-enter auto mode.
//
// If no CONFIG mode requested, enter OPERATIONAL MODE
//
//		pull LOW GPIO7	  // this should be the boot-up default
//		allow HIGH GPIO6  // turns on auto-connect mode
//
// then wait for a connect just like normal

void BT::configMode(char *name)
{
     // get out of auto connect mode and set 9600 baud

     autoConnectMode(false);
     baud9600mode(true);
     begin(BT_CONFIG_BAUD);		// set the SoftwareSerial baud rate appropriately

     // each mode configuration starts with a reset() to ensure that
     // the latest modes are enabled and baud rates set

     reset();

     btSend("$$$");		// get into command mode

     delay(30);

     // see the bottom of the file for notes about GPIOs

     // NOTE - that power could be set lower here, but for now we set it to the
     //		maximum - allowing the module to negotiate down if necessary.
     //		Note, too, that it is dependent upon the BT firmware 4.77 vs. 6.15

#define btV615
//#define btV477

#ifdef btV615
     btSend("SA,4");            // tells the RN-42 to use simple pin mode authentication
     btSend("\r");

     delay(30);

     btSend("SY,000C\r");	// set power to 12 db
#endif
#ifdef btV477
     btSend("SY,0004\r");	// set power to 12 db
#endif
     delay(30);

     btSend(BT_SU_BAUD_STRING);	// set appropriate baud

//     btSend("SU,");		// old way
//     btSend(BT_SU_BAUD);
//     btSend("\r");

     delay(30);

     btSend("SN,");		// set the appropriate name
     btSend(name);
     btSend("\r");

     delay(30);

     btSend("SM,4");		// auto connect mode
     btSend("\r");

     delay(30);

     btSend("SR,Z");		// erased previously stored connection
     btSend("\r");

     delay(30);

     btSend("SX,1");		// set bonding mode (only stored device can attach)
     btSend("\r");

     delay(50);			// this command takes a little more time

//     btSend("S?,1");		// attempt master/slave flip
//     btSend("\r");
//     delay(30);		// no reason to do this any more - we do it manually

     btSend("U,");		// do an immediate baud rate setting
     btSend(BT_U_BAUD);		// to eliminate the need for a reboot
     btSend(",N\r");		// (exits command mode too)

     baud9600mode(false);	// get out of 9600 mode, but leave auto connect off
     
     flushReturnData();
}

void BT::setRemoteAddress(char *address)
{
  autoConnectMode(false);
  baud9600mode(true);
  begin(BT_CONFIG_BAUD);        // set the SoftwareSerial baud rate appropriately
     // each mode configuration starts with a reset() to ensure that
     // the latest modes are enabled and baud rates set

  reset();

  btSend("$$$");		// get into command mode
  delay(30);

  btSend("SR,");
  btSend(address);
  btSend("\r");
  delay(30);

  btSend("---");		// and out of command mode
  btSend("\r");
  delay(30);

  baud9600mode(false);	// get out of 9600 mode, but leave auto connect off
  autoConnectMode(true);
  
  flushReturnData();
}

void BT::opMode()
{
     // fire-up auto connect mode and kill 9600 baud

     autoConnectMode(true);
     baud9600mode(false);
     begin(BT_OP_BAUD);		// set the SoftwareSerial baud rate appropriately

     // each mode configuration starts with a reset() to ensure that
     // the latest modes are enabled and baud rates set

     reset();

     // we're in 38400 baud in this case, or should be

     btSend("$$$");		// get into command mode

     delay(30);

     btSend("Q,1");		// make the chapr undiscoverable (don't know if this works)
     btSend("\r");

     delay(30);

     btSend("---");		// and out of command mode
     btSend("\r");
    
     flushReturnData();
}

void BT::autoConnectMode(bool turnOn)
{
#ifdef V01
     if (turnOn) {
	  specialPin(_autoModePin,HIGH);
     } else {
	  specialPin(_autoModePin,LOW);
     }
#endif

#ifdef V02
     if (turnOn) {
	  digitalWrite(_autoModePin,HIGH);
     } else {
	  digitalWrite(_autoModePin,LOW);
     }
#endif
}

void BT::baud9600mode(bool turnOn)
{
#ifdef V01
     if (turnOn) {
	  specialPin(_9600BaudPin,HIGH);
     } else {
	  specialPin(_9600BaudPin,LOW);
     }
#endif

#ifdef V02
     if (turnOn) {
	  digitalWrite(_9600BaudPin,HIGH);
     } else {
	  digitalWrite(_9600BaudPin,LOW);
     }
#endif
}

//
// connected() - returns true if the BT is connected, false otherwise
//
bool BT::connected()
{
     return(digitalRead(_connectPin) == HIGH);
}

void BT::btSend(char *string)
{
     write(string);		// may end-up with a delay in here
}

void BT::btWrite(byte *buffer, int size)
{
     write(buffer,size);
}

//
// getResponse() - this routine knows about all of the potential responses from the RN-42
//		   so it listens for one of them.  It will continue to listen until it
//		   gets SOME response (even a blank line) or times-out.  So, this routine
//		   will BLOCK (stop the rest of the program) until the response is received
//		   or the timeout occurs.
//

//btResponse BT::getResponse(long timeout)
//{
//
//}

//
// recv() - receive data back from the BT module.  This code assumes that a line is
//		always return - that is, something ending in a '\r'.  The given timeout
//		(in ms) will cause this routine to return if a whole line doesn't
//		come back (either nothing or a partial line). It returns the number of
//              characters written into the buffer (not including the null terminator)
//
int BT::recv(char *buffer, long timeout)
{
     long target = millis() + timeout;
     int i = 0;

     do {
          if (available()){
            buffer[i] = read();
            if (buffer[i] == '\r' || buffer[i] == '\n'){
              break;
            }
            i++;
          }
     } while (millis() < target);

     buffer[i] = '\0';
     
     return i;
}

//
// recv() - receive the specified number of bytes without null termination
int BT::recv(byte *buffer, int count, long timeout){
     //read first two bytes to determine size (use timeout as an escape hatch)
     long target = millis() + timeout;
     int i = 0;
     
     while (millis() < target && count){
       if (available()){
            buffer[i] = read();
            i++;
            count--;
       }
     }
     
     return i;
}

#define TIMEOUT 1000

void BT::flushReturnData()
{
  delay(40); //delay to make sure final messages from RN-42 are received
  while (available()){
    read();
  } 
}

// 
// addressFilter() - given a human constructed text-based BT address, convert it to
//			something that is a valid BT hex address (for use in the RN-42).
//
//	      RETURNS: true if a "valid" address is there, "false" otherwise.
//
//		NOTES:  - the incoming buffer is changed.
//			- the incoming buffer should be NULL terminated or the results
//			  may be unexpected (though no crashes)
//
bool BT::addressFilter(char *buffer, int size)
{
     char	*ptr;
     int	 i;

     for(i=0, ptr = buffer; i < size; i++, ptr++) {

	  if (*ptr == '\0') {			// reached the end
	       break;
	  }

	  if (*ptr >= 'a' && *ptr <= 'f') {	// convert lower case hex to upper case
	       *ptr -= '\x20';
	       continue;			// this char is valid, move on
	  }

	  if (*ptr < '0' || *ptr > 'F' || (*ptr < 'A' && *ptr > '9')) {

	       // slide down stuff to punt bad address characters
	       // this REALLY counts on ASCII ordering!  Kinda' standard...
	       // When this happens, the size of the string strinks

	       char	*pfrom;
	       char	*pto;
	       int	 j;

	       size--;

	       for(pfrom = ptr+1, pto = ptr, j = i; j < size; j++, pto++, pfrom++) {
		    *pto = *pfrom;
		    if (*pto == '\0') {
			 break;			// found and copied null terminator
		    }
	       }

	       // we need to re-process the current character at this point
	       // so prepare the variables for increment at the end of the loop...

	       i--;
	       ptr--;
	  }

     }

     // only return true if we have exactly 12 characters (6 hex bytes)

     return(strlen(buffer) == 12);
}

bool BT::checkVersion()
{
  char buf[50];
  delay(100);
  autoConnectMode(false);
  baud9600mode(true);
  begin(BT_CONFIG_BAUD);      // set the SoftwareSerial baud rate appropriately
  reset();

  btSend("$$$");
  delay(200);
  recv(buf, 1000);
  delay(100);

  // are we connected at all?

  if (strcmp(buf, "CMD") != 0){
       return(false);
  }

  // seems to be talking to us, ask for its version

  btSend("ver\r");
  delay(200);

  // can get up to three lines back, so print them out
  recv(buf, 1000);
  if (buf[0] != '\0'){
       Serial.println(buf);
  }

  recv(buf, 1000);
  if (buf[0] != '\0'){
       Serial.println(buf);
  }

  recv(buf, 1000);
  if (buf[0] != '\0'){
       Serial.println(buf);
  }
  flushReturnData();

  return true;
}

//
// We USED to have a zombie mode when we couldn't turn ourselves off.
// It would cause use to enter low power mode.
// The code here, though, may be useful in the future...so it wasn't deleted
//
#ifdef NOTDEF
void BT::zombieMode()
{
     // fire-up auto connect mode and kill 9600 baud

     autoConnectMode(false);
     baud9600mode(false);
     begin(BT_OP_BAUD);		// set the SoftwareSerial baud rate appropriately

     // each mode configuration starts with a reset() to ensure that
     // the latest modes are enabled and baud rates set

     reset();

     // we're in 38400 baud in this case, or should be

     btSend("$$$");		// get into command mode

     delay(30);

     btSend("K,");		// disconnects any connection
     btSend("\r");

     delay(30);

     btSend("Z");              //enters deep sleep mode
     btSend("\r");

    flushReturnData();
}
#endif

///////////////////////////////////////////////////////////////////
//  GPIO NOTES
//
//   We've done some work on the GPIOs of the BT module, though
//   decided not to impement it yet.  Here's the data
//
// set the state of the GPIO in/out - the following map is our guide
//
//		GPIO    Dir     Description            	GPIO    Dir     Description
//		---------------------------             ---------------------------
//		0       ?                               8	OUT	RF status
//		1       ?                               9	IN
//		2       OUT     connect signal          10	IN
//		3       IN      (unused)                11	IN
//		4       IN      factory reset           
//		5       OUT     connect status          
//		6       IN      auto connect            
//		7       IN      9600 baud               
//
// set GPI08 - GPIO11 as inputs (normally only 8 is an output anyway
//     btSend("S*,0f00\r");
//     delay(30);
//
// turn off GPIO 0,1,5 0b00100011 mask, 0b00000000 value
//     btSend("S@,2300\r");
//     delay(30);
//     btSend("S&,2300\r");	// no pull-ups
//     delay(30);
///////////////////////////////////////////////////////////////////
