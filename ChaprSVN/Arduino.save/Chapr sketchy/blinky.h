
#ifndef BLINKY_H
#define BLINKY_H

//
// The different states of the blinky LED are:
//
//	BLINKY_OFF  - the LED is off (duh)
//	BLINKY_ON   - the LED is on (also duh)
//	BLINKY_FAST - the LED is blinking at a rate of 5 times a second'ish (note that "blinking" is on AND off)
//	BLINKY_SLOW - the LED is blinking at a rate of 1 time a second'ish
//
typedef enum { BLINKY_OFF, BLINKY_ON, BLINKY_FAST, BLINKY_SLOW } blinky_state;

class blinky
{
public:
     blinky(int pin);

     void on();
     void off();
     void fast();
     void slow();

     void update();

     void phaseShift();

private:
     int		_pin;
     blinky_state	_state;
     int		_shift;		// in degrees - mod'd by 360 when set
};


#endif BLINKY_H
