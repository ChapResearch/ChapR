//
// VDIPlow.cpp
//
//   This is the low-level VDIP code.  It was moved from here to keep make the replacement
//   of the VDIP processing code easier.  This includes the class initialization and such.
//

#include <Arduino.h>

#include "VDIPSPI.h"
#include "VDIP.h"

#define RESET_DELAY_SECS		2

//
// constructor
//
VDIP::VDIP(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin, uint8_t resetPin) :
    VDIPSPI(clockPin,mosiPin,misoPin,csPin),
    _resetPin(resetPin),
    _resetDelay(false)
{
     digitalWrite(_resetPin,HIGH);   // low is reset, done before shifting to output mode
     pinMode(_resetPin,OUTPUT);

     ports[0].port = -1;		// both ports start out as not initialized
     ports[1].port = -1;

     // we do a reset whenever the VDIP object is initialized - since there is only one
     // and it is only initialized upon power-up, then we're ok with this

     reset();
}


//
// readBytes() - given a byte count, do a read of the number of bytes
//              requested from the monitor.  The array better be big enough...
//              this has a timeout that will cause a return if hit.  If timeout
//		is given as zero, THIS IS A BLOCKING CALL - which should never
//		be done.  The timeout is given in milliseconds.
//
//		RETURNS true if data was received, false if the timeout was hit.
//
bool VDIP::readBytes(int count, char  *retarray, int timeout)
{
     long	 target = millis() + timeout;
     
     for (int i=0; i < count; i++) {
	  while(!recv((unsigned char *)retarray+i)) {
	       if((timeout != 0) && (millis() > target)) {
		    return(false);
	       }
	  }
     }

     return(true);
}


//
// sendBytes() - given a byte count - send those bytes to the monitor.  It is
//               assumed that the monitor is in a good place to receive the
//               bytes, or this routine may timeout.  If timeout is given as
//		 zero, this routine WILL BLOCK - which is bad.  The timeout
//		 is given in milliseconds.
//
//		RETURNS true if data was sent, false if the timeout was hit.
//
bool VDIP::sendBytes(int count, const char *buffer, int timeout)
{
     long	 target = millis() + timeout;

     for (int i=0; i < count; i++) {
	  while(!send(buffer[i],false)) {
	       if((timeout != 0) && (millis() > target)) {
		    return(false);
	       }
	  }
     }
}

//
// flush() -    Since it is possible that async data can come from the
//		monitor before or after executing a command, this routine
//		checks for and consumes all data sitting in the monitor buffer.
//
//		This routine assumes that if there is nothing in the buffer
//		when it is called, then nothing will be in the buffer by
//		the time this routine finishes.  sure, that means that there
//		is a race condition.
//
//		If there IS something in the buffer, then the routine will wait
//		for the configured number of milliseconds for the buffer to clear,
//		while consuming the data in the meantime.  Then it will try again
//		for a clear buffer, until the flush is completely "flushed."
//
//		Soooo, this routine may actually take more time than the "timeout".
//
void VDIP::flush(int timeout)
{
     long		target = millis() + timeout;
     unsigned char	buffer[1];

     while(recv(buffer)) {

	  // if was something in the buffer, assume there is more and consume
	  // it until the timeout clears

	  while(millis() < target) {
	       (void)recv(buffer);
	  }

	  // loop around to check again
     }
}

//
// sync() - "sync-up" with the VDIP board prior to issuing a command.  This makes
//	    sure that the board is ready for a command before interacting with it.
//	    Granted there is still the possibility that some darned async data
//	    could fill the VDIP buffer before the command after this sync.  But
//	    there isn't much we can do about that...alas.
//
//	    BIG NOTE - this routine doesn't have a timeout.  The point here is 
//	    that if this routine can't get in sync with the board, then we have
//	    insurmountable problems!  In other words, major bummer.
//
//	RETURNS:
//		false - not yet in sync, need to call it again to get in sync (and
//			go ahead and do other stuff in the mean-time, like reset
//			something else or blink an LED)
//		true - we're in sync with the VDIP - go ahead and issue a command
//
//	ASSUMPTIONS:
//		- reset() will set _resetDelay and _resetTarget appropriately
//
bool VDIP::sync()
{
     // note that we're using the "E" (echo) to sync - this is the same in
     // both short and extended command set - so this can be used during reset

     const char		sBuffer[2] = { 'E', '\r' };

     // check to see if we are still in reset delay

     if(_resetDelay) {
	  if(millis() < _resetTarget) {
	       flush();			// just keep flushing the VDIP output buffer
	       return(false);		// still in reset delay
	  }

	  // we just got out of reset delay, time to initialize the VDIP.
	  // To do this, get in sync, then execute the init() command(s).

	  _resetDelay = false;

	  sync();			// RECURSIVE!
	  init();

	  // we assume (rightly/wrongly) that we are in sync after this
	  // so just return - no need to do it again below

	  return(true);
     }

     // all reset-style processing is done above - at this point we're just in the
     // middle of operations looking to ensure that we're sync'd with the VDIP

     // the "new" sync is blocking style, that is, if the VDIP misbehavesw, then
     // it will never return...but if that is the case, we have bigger problems anyway.
     // Below, we send out the sync/echo command and simply wait for its return.
     // This has the same effect as a flush, by-the-way.

     sendBytes(2,sBuffer,0);		// a blocking send of the sync/echo string

     while(true) {
	  unsigned char	RChar, prevRChar;

	  if(recv(&RChar)) {
	       if(RChar == '\r' && prevRChar == 'E') {
		    // we're sync'd up here!
		    return(true);
	       }
	       prevRChar = RChar;
	  }
     }

     // we never get here
}

//
// reset() - reset the VDIP.  After calling this routine, nothing can really be done
//	     with the VDIP until it is out of reset delay.  The other low-level commands,
//	     though, will take care of this for us.
//
//	     In essence, there is a four-part reset strategy.  This call is the first part:
//	     resetting the device.  The second part is to wait for an appropriate amount
//	     of time for the device to get itself reset.  Part 3 - get in sync with the
//	     the device, and Part 4 - set the appropriate operational parameters.
//
//	     The sync() command processes steps 2,3, and 4. This routine just kicks it
//	     off.
//
void VDIP::reset()
{
     _resetDelay = true;		// we are beginning the reset delay

     digitalWrite(_resetPin,LOW);   	// low is reset

     // note that there used to be a delay in here to hold the reset low for awhile
     // because the VDIP doc didn't say anything about how the reset pin worked (falling edge
     //	for example).  However, the delay doesn't work when the reset is being called as
     // part of the object initialization because the delay() timer isn't yet up and running.
     // Soooo, we just count on the VDIP watching for the falling edge.

     digitalWrite(_resetPin,HIGH);

     _resetTarget = millis() + (RESET_DELAY_SECS*1000);
}

//
// init() - initializes the VDIP.  This is only done after a reset/sync.  Anything that
//		needs to be done to get the VDIP ready for operations is done here.
//		Given the importance of init() it blocks.  That is, if it doesn't return
//		we're in big trouble anyway.
//
//		This routine assumes that we're in sync.
//
void VDIP::init()
{
     // the only thing we need to do here is go into shortened command mode
     // we are, by default, in binary value mode already upon reset so we don't
     // need to deal with that.  Note, by-the-way, that this command works in
     // either extended or shortened command mode.

     const char		sBuffer[4] = { 'S', 'C', 'S', '\r' };
     unsigned char	rBuffer[1];

     (void)sendBytes(4,sBuffer,0);		// just blocks waiting to send

     while(!recv(rBuffer)) {			// spin around waiting for first return byte
     }

     while(rBuffer[0] != '\r') {		// will potentially block waiting for '\r'
	  recv(rBuffer);
     }
}
