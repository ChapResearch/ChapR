//this is the button class

#include <Arduino.h>
#include "button.h"

button::button (int pin) : _pin(pin), wasPressed(false)
{
  pinMode(_pin, INPUT);
}

bool button::isPressed()
{
  wasPressed = digitalRead(_pin);
  return wasPressed;
}

bool button::hasChanged()
{
  int current = digitalRead(_pin);
  if (current != wasPressed){
    wasPressed = current;
    return true;
  }
  wasPressed = current;
  return false;
}
