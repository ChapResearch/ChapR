//this is the button class

#include <Arduino.h>
#include "config.h"
#include "button.h"

button::button (int pin) : _pin(pin), _wasPressed(false), _inverted(false), _target(0)
{
  pinMode(_pin, INPUT);
  _wasPressed = check();
}

button::button (int pin, bool inverted) : _pin(pin), _wasPressed(false), _inverted(inverted), _target(0)
{
  pinMode(_pin, INPUT);
  if(_inverted) {
     digitalWrite(_pin,HIGH);	// turns on the pull-up resistor for inverted buttons;
  }
  _wasPressed = check();
}

//
// check() - returns the current status of the button. ONLY TO BE USED IF
//           you don't care about debounce or monitoring for the change.
//
bool button::check()
{
     bool	retval = digitalRead(_pin);

     return(_inverted?!retval:retval);
}

//
// isPressed() - returns true if the button is pressed, and false otherwise.
//
bool button::isPressed()
{
     return _wasPressed;
}

//
// hasChanged() - returns true if the button has changed. When the button changes
//                this routine enters a "debounce" period during which no other
//                changes will be reported. This serves to "debounce" the mechanical
//                switches. Use isPressed() to get the current value (true = pressed).
//
bool button::hasChanged()
{
     if (millis() > _target){ // checks to see if it is in the "debounce period"
       int current = check();
       if (current != _wasPressed){ // if it has changed and is not in "debounce"
         _wasPressed = current;
         _target = millis() + DEBOUNCE;
         return true;
       }
     }
     
     return false;
}
