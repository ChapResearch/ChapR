#include <Arduino.h>
#include "sound.h"

sound::sound(int pin) :
     _pin(pin)
{
     pinMode(_pin,OUTPUT);
}

void sound::confirm()
{
  tone(_pin,440,50);
  delay(50);
  tone(_pin,880,50);
}

void sound::squeep()
{
     noTone(_pin);
     for (int i = 880; i < 3520; i += 20) {
	  tone(_pin,i);
	  delay(1);
     }
     for (int i = 3520; i > 880; i -= 20) {
	  tone(_pin,i);
	  delay(1);
     }
     noTone(_pin);
}

//add icky noise
