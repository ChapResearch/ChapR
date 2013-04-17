
//#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
#include <Arduino.h>
//#include <inttypes.h>
//#include <Stream.h>

#include "VDIPSPI.h"

//
// standard definitions of the clock pin
//
#define SPI_CLK_UP_1	digitalWrite(_clockPin, HIGH)
#define SPI_CLK_DOWN_1	digitalWrite(_clockPin, LOW)

//
// these definitions slow things down a bit
//
#define SPI_CLK_UP	SPI_CLK_UP_1
#define SPI_CLK_DOWN	SPI_CLK_DOWN_1

//
// constructor
//
VDIPSPI::VDIPSPI(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin)
{
  pinMode(clockPin,OUTPUT);     this->_clockPin = clockPin;
  pinMode(mosiPin,OUTPUT);      this->_mosiPin = mosiPin;
  pinMode(misoPin,INPUT);       this->_misoPin = misoPin;
  pinMode(csPin,OUTPUT);        this->_csPin = csPin;

  // start the pins off in known states

  SPI_CLK_DOWN;
  digitalWrite(_mosiPin,LOW);
  digitalWrite(_csPin,LOW);
}

//
// private methods
//

//
// _header() - sends out the header for a read/write for the status
//             or data bytes.  clock is low upon exit.  cs is high
//             upon exit.
//
void VDIPSPI::_header(bool do_read, bool statusByte)
{
  SPI_CLK_DOWN;
  digitalWrite(_csPin, HIGH);
  digitalWrite(_mosiPin,HIGH);                    // prepare for interaction
  SPI_CLK_UP;
  SPI_CLK_DOWN;
  digitalWrite(_mosiPin,(do_read?HIGH:LOW));         // read or write
  SPI_CLK_UP;
  SPI_CLK_DOWN;
  digitalWrite(_mosiPin,(statusByte?HIGH:LOW));   // targeting status or data
  SPI_CLK_UP;
  SPI_CLK_DOWN;
}
//void VDIPSPI::_oldheader(bool do_read, bool statusByte)
//{
//  SPI_CLK_UP;
//  digitalWrite(_csPin, HIGH);
//  digitalWrite(_mosiPin,HIGH);                    // prepare for interaction
//  SPI_CLK_DOWN;
//  SPI_CLK_UP;
//  digitalWrite(_mosiPin,(do_read?HIGH:LOW));         // read or write
//  SPI_CLK_DOWN;
//  SPI_CLK_UP;
//  digitalWrite(_mosiPin,(statusByte?HIGH:LOW));   // targeting status or data
//  SPI_CLK_DOWN;
//}

//
// _statusbit() - read the status bit for either a read or a
//                write, and return true if the read/write
//                worked, or false otherwise.  When reading a
//                it will fail if the data being read has been
//                read before.  When writing it will fail if
//                the buffer space overflowed.  In either case
//                the read/write needs to be done again.
//
//                Upon exit the clock and cs are low.
//
bool VDIPSPI::_statusbit()
{
  unsigned char   statusbit;

//
// the next two lines were previously reversed - it fixed the problem
//	putting them this way!  Which makes sense if you study "recv()".
//
  statusbit = digitalRead(_misoPin);
  SPI_CLK_UP;

  SPI_CLK_DOWN;
  digitalWrite(_csPin,LOW);
  SPI_CLK_UP;		// give the VDIP an extra clock pulse to consume CS
  SPI_CLK_DOWN;

  return(statusbit != HIGH); // HIGH on the last bit indicates old data or bad write
}


//
// public methods
//

//
// send() - sends data on the SPI buss to the VDIP.  Returns 
//          true if the write was accepted, or false otherwise.
//          If false, that means that the VDIP buffer was full
//          and the write needs to be attempted again.
//
bool VDIPSPI::send(unsigned char data, bool statusByte /* = false */)
{
  _header(false,statusByte);                   // set up for a write

  for(int i=8; i--; ) {
    digitalWrite(_mosiPin,(data&0x80)?HIGH:LOW);
    SPI_CLK_UP;
    SPI_CLK_DOWN;
    data <<=1;
  }

  return(_statusbit());
}

//
// recv() - the receive routine that grabs a byte from
//           either status or data.  The routine returns true if
//           the read was of new data, false otherwise.  The given
//           data pointer is filled-in in either case.
//
bool VDIPSPI::recv(unsigned char *data, bool statusByte /* = false */)
{
  *data = 0;
  _header(true,statusByte);                    // set up for a read

  for( int i = 8; i--; ) {
    *data <<= 1;
    *data |= (digitalRead(_misoPin) == HIGH)?0x01:0x00;
    SPI_CLK_UP;
    SPI_CLK_DOWN;
  }

  return(_statusbit());
}
//bool VDIPSPI::oldrecv(unsigned char *data, bool statusByte /* = false */)
//{
//  *data = 0;
//  _header(true,statusByte);                    // set up for a read
//
//  for( int i = 8; i--; ) {
//    *data <<= 1;
//    SPI_CLK_UP;
//    *data |= (digitalRead(_misoPin) == HIGH)?0x01:0x00;
//    SPI_CLK_DOWN;
//  }
//
//  return(_statusbit());
//}

