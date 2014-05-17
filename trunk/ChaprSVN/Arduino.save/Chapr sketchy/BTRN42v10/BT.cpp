#include <Arduino.h>

#include "BT.h"
#include "SoftwareSerial.h"

//
// constructor
//
BT::BT(uint8_t receive, uint8_t transmit, uint8_t reset) :
     SoftwareSerial(receive,transmit),
     _rxPin(receive),
     _txPin(transmit),
     _rstPin(reset)
{
     pinMode(_rxPin,INPUT);
     pinMode(_txPin,OUTPUT);
}

//
// reset() - reset the bluetooth device - note that it is separate
//		from the sync/baud rate setup.  Use sync() for that.
//		Note that at least 500 ms needs to lapse after this
//		call before sync() is called.
//
void BT::reset()
{
     // to do a reset, take the _rstPin out of tri-state and low
     // then pull it back tri-state - this allows us to be cheap
     // relative to the 3.3v reset pin which has a pull-up.
     // note that ATMEGA pins default to INPUT (hi-imped) so
     // it is assumed that they are set as INPUT pins before
     // this routine.

     digitalWrite(_rstPin,LOW);	// ensure it has a low in output buffer
     pinMode(_rstPin,OUTPUT);	// generate the output 

     // needs to be low for min 160us, but 10 ms works great

     delay(10);

     pinMode(_rstPin,INPUT);	// then put it back where it was
}

//
// sync() - assuming that the bt has been reset, sync-up with it
//		and get prepared for further communication.
//
void BT::sync()
{
      // enter command mode at 115200 baud - the default upon reset

     begin(115200);
     delay(320);		// can this be smaller?
     write("$$$");
     delay(15);			// 

     // first pull it out of slave mode - put it back later if it is appropriate

     println("SN,ChapR-1");
     delay(100);
     println("SM,1");
     delay(100);
     println("R,1");

//     println("Q");
//     delay(100);
//     println("W");


//     delay(5000);		// for testing, wait 5 seconds and exit command mode

//     write("---\r");

     // reduce speed to 57600 - then start programming the thing

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
//		come back (either nothing or a partial line).
//
//void BT::recv(char *buffer, long timeout)
//{
//     long	*target = millis() + timeout;
//
//     do {
//	  if( read(
     


//
// private methods
//
