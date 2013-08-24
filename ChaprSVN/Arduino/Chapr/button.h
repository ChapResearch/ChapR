

#ifndef BUTTON_H
#define BUTTON_H

class button
{
 public:
  button(int pin);
  button(int pin, bool inverted);
  
  bool isPressed();
  bool hasChanged();

 private:
  int _pin;
  bool _wasPressed;
  bool _inverted;
  bool check();
};

#endif BUTTON_H

