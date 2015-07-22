//
// watchdog.cpp
//
//	This file implements the watchdog functions to keep any software
//	bugs from fatally locking up the ChapR.  The issue we're trying
//	to protect against is lock-ups which cause the user NOT to be
//	able to turn off the ChapR.  The big two places where lock-ups
//	have occured are (1) during low-power situations, and (2) when
//	plugging/unplugging joysticks.  If a joystick is "whacked" it
//	will often lock-up the ChapR because the VDIP stops responding.
//
//	NOTE - these are coded as simple C functions, so the include
//	file doesn't instantiate a class.
//

#include <Arduino.h>
#include <avr/wdt.h>
#include "watchdog.h"
#include "power.h"
#include "sound.h"

//
// watchdogFeed() - "feed" the watchdog.  If the watchdog isn't fed enough
//			- like when the ChapR "locks-up" - then the interrupt
//			function will turn the ChapR off.
//
void watchdogFeed()
{
     wdt_reset();
}

//
// watchdogOff() - (I wanted to name this something cute like "watchdogKennel()" -
//		    but decided to be more boring) This function suspends the
//		    watchdog so that long-term things can happen, like board bring-up.
//		    See watchdogOn() for descriptions of the bits.
//
void watchdogOff()
{
     cli();		// stop interrupts

     // Enter Watchdog Configuration mode:
     WDTCSR |= (1<<WDCE) | (1<<WDE);

     // turn off "interrupt" and "reset" actions of the watchdog
     WDTCSR =  (0<<WDIE) | (0<<WDE);

     sei();		// start up again
}

//
// watchdogOn() - the opposite of watchdogOff() - it turns on the watchdog.
//
void watchdogOn()
{
     // we can't use the "easy" functions like wdt_enable() because they set the
     // "reset" mode on the watchdog.  We need the interrupt mode.

     cli();		// stop interrupts
     wdt_reset();	// reset the timer for now

     // watchdog is programmed by setting thethe WDTCSR register:
     //
     //	 bit 7      6      5      4      3      2      1      0
     //  ,------,------,------,------,------,------,------,------,
     //	 | WDIF | WDIE | WDP3 | WDCE |  WDE | WDP2 | WDP1 | WDP0 |
     //  '------'------'------'------'------'------'------'------'
     //
     //	WDIF = tells you when you are in a watchdog interrupt
     // WDIE = enables interrupts
     // WDCE = configuration mode for 4 cycles
     // WDE  = enables system reset on timeout (as opposed to interrupt routine)
     // WDP? = four bits programming the time-out (see doc) some values:
     //		0 0 0 0 = 16 ms
     //		0 1 1 0 = 1000 ms (1 second)
     //		0 1 1 1 = 2000 ms
     //		1 0 0 0 = 4000 ms
     //		1 0 0 1 = 8000 ms

     // Enter Watchdog Configuration mode:
     WDTCSR |= (1<<WDCE) | (1<<WDE);

     // turn on interrupt, turn off reset, and 4 seconds timeout
     WDTCSR =  (1<<WDIE) | (0<<WDE) |
	       (1<<WDP3) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0);

     // turn on interrupts again
     sei();
}

//
// Watch Dog Bite! - So the watchdog bite is worse than its bark!  This is the interrupt
//		    routine that is called when the watchdog goes off.  It simply issues
//		    the icky sound and shuts down the ChapR.  Note that this routine is
//		    running as a interrupt - so interrupts are normally off during processing.
//		    So we have to turn them back on so our sounds will work.
//
//		    The declaration is a bit weird - this instructs the compiler to put
//		    a pointer to this routine in the interrupt vector table - specifically
//		    the entry for the watchdog timer.
//
ISR(WDT_vect) {
  Serial.println("bite");
     extern sound beeper;

     // OK - this is weird - but we need to turn on interrupts, even though we are in an interrupt
     // so that we can issue our sounds before we power down.

     sei();

     beeper.icky();	// double icky followed by the powerdown sound
     beeper.icky();	// ...makes it very distinctive from anything else
     powerDown();	// no return from this routine
}
