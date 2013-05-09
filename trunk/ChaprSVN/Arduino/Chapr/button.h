

#ifndef BUTTON_H
#define BUTTON_H

class button
{
 public:
  button(int pin);
  
  bool isPressed();
  bool hasChanged();

 private:
  int _pin;
  bool wasPressed;
};

#endif BUTTON_H

