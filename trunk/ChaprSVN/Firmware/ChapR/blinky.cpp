//
// blinky.cpp
//
//	this file implements blinky lights!  To use it:
//		0 - include blinky.h
//		1 - define a blinky object by giving it a digital pin where there is an LED
//		2 - call the appropriate funtion to set the state of the light:  on, off, fast, slow
//		3 - somewhere in your "loop()" call the blinky update function to update the
//			status of the blinky lights.  If you use multiple blinky's, you need to call
//			the update for each one.
//	NOTES:
//		- blinky keeps the lights in sync with the system clock, so they will blink in
//		  unison normally.  If you don't like this, call "phaseShift" and it will make
//		  the blinky "half-off" of the normal blinky pattern.
//		  So if you have two blinky's and one is shifted, then they
//		  will blink exactly opposite of each other at the same speed.
//		- the phaseShift is only useful for blinking blinky's - "on" and "off" are not
//		  affected.

#include <Arduino.h>

#include "blinky.h"

//
// NOTE that the _shift has been turned on during init - this makes it so that the booting
//	power LED starts off ON.
//
blinky::blinky(int pin) : _pin(pin), _state(BLINKY_OFF), _shift(true)
{
     pinMode(_pin,OUTPUT);

     update();		// go ahead and turn on things that should go on right away
}

void blinky::on()
{
     _state = BLINKY_ON;

     update();
}

void blinky::off()
{
     _state = BLINKY_OFF;

     update();
}

void blinky::fast()
{
     _state = BLINKY_FAST;

     update();
}

void blinky::slow()
{
     _state = BLINKY_SLOW;

     update();
}

//
// update() - this is the workhorse routine of blinky.  It checks to see if
//		this LED should be on or off based upon the system clock
//		and current settings of the blinky.
//
void blinky::update()
{
     // this routine uses the millis() routine - which returns the number of milliseconds
     // that the Arduino has been running since it was booted.  This number won't roll over
     // unless you leave it running for 50 days :-) but that really doesn't matter for us.

     // the way blinky update works is that is attaches some meaning to the different
     // bits in the millis() reading.  Since it is counting milliseconds, we want our
     // blinking to be:
     //		BLINKY_SLOW - 512 millis "bit"
     //		BLINKY_FAST - 128 millis "bit"
     // By checking to see if the given bit is on/off, the LEDs can be set to blink

     int		output;
     unsigned long	now = millis();

     switch(_state) {
     default:
     case BLINKY_ON:	output = HIGH; break;
     case BLINKY_OFF:	output = LOW; break;
     case BLINKY_SLOW:	output = (now & 512)?(_shift)?LOW:HIGH:(_shift)?HIGH:LOW; break;
     case BLINKY_FAST:	output = (now & 128)?(_shift)?LOW:HIGH:(_shift)?HIGH:LOW; break;
     }

     digitalWrite(_pin,output);
}

void blinky::phaseShift()
{
     _shift = ! _shift;

     // as with slow/fast the LED is only affect when update is run
}

