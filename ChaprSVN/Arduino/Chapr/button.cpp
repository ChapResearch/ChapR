//this is the button class

#include <Arduino.h>
#include "button.h"

button::button (int pin) : _pin(pin), _wasPressed(false), _inverted(false)
{
  pinMode(_pin, INPUT);
  (void)isPressed();
}

button::button (int pin, bool inverted) : _pin(pin), _wasPressed(false), _inverted(inverted)
{
  pinMode(_pin, INPUT);
  if(_inverted) {
     digitalWrite(_pin,HIGH);	// turns on the pull-up resistor for inverted buttons;
  }
  (void)isPressed();
}

bool button::check()
{
     bool	retval = digitalRead(_pin);

     return(_inverted?!retval:retval);
}

bool button::isPressed()
{
     _wasPressed = check();
     return _wasPressed;
}

bool button::hasChanged()
{
     int current = check();
     if (current != _wasPressed){
	  _wasPressed = current;
	  return true;
     }
     _wasPressed = current;
     return false;
}
