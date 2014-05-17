

#ifndef BUTTON_H
#define BUTTON_H

class button
{
 public:
  button(int pin);
  button(int pin, bool inverted);
  
  bool isPressed();
  bool hasChanged();
  bool check();

 private:
  int _pin;
  bool _wasPressed;
  bool _inverted;
  long _target;
};

#endif BUTTON_H

