#include <Arduino.h>
#include "sound.h"
#include "power.h"

//
// these two defines turn on/off the timer2 that is used to produce tones
// we turn it off and on to save power since most of the time we're NOT
// producing sound
//
#define SOUND_ON	tonePowerOn()
#define SOUND_OFF       tonePowerOff()

sound::sound(int pin) :
     _pin(pin)
{
     pinMode(_pin,OUTPUT);
     SOUND_OFF;
}

void sound::confirm()
{
     SOUND_ON;
     tone(_pin,440,50);
     delay(50);
     tone(_pin,880,50);
     delay(50);
     SOUND_OFF;
}

void sound::icky()
{
     SOUND_ON;
     noTone(_pin);
     for (int i = 880; i > 220; i -= 10) {
	  tone(_pin,i);
	  delay(1);
     }
     tone(_pin, 220);
     delay(100);
     noTone(_pin);
     SOUND_OFF;
}

void sound::squeep()
{
     SOUND_ON;
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
     SOUND_OFF;
}

void sound::yawn()
{
     SOUND_ON;
     noTone(_pin);

     tone(_pin,5000,40);
     delay(40);

     for( int i=4; i--; ) {
	  tone(_pin,110,50);
	  delay(100);
     }
     SOUND_OFF;
}

void sound::start()
{
  SOUND_ON;
  noTone(_pin);
  
  for (int i = 100; i < 2000; i += 5) {
	  tone(_pin,i);
	  delay(1);
  }
  noTone(_pin);
  SOUND_OFF;
}

void sound::beep()
{
  SOUND_ON;
  noTone(_pin);
  
  tone(_pin,1500);
  delay(150);
  
  noTone(_pin);
  SOUND_OFF;
}

void sound::boop()
{
  SOUND_ON;
  noTone(_pin);
  
  tone(_pin,500);
  delay(150);
  
  noTone(_pin);
  SOUND_OFF;
}

void sound::kill()
{
  SOUND_ON;
  noTone(_pin);
  
  for (int i = 2000; i > 100; i -= 5) {
	  tone(_pin,i);
	  delay(1);
  }
  noTone(_pin);
  SOUND_OFF;
}

void sound::select()
{
  SOUND_ON;
  noTone(_pin);
  
  tone(_pin,1500);
  delay(50);

  tone(_pin,1000);
  delay(50);

  tone(_pin,1500);
  delay(50);
  
  noTone(_pin);
  SOUND_OFF;
}

/*void sound::endStart() //only used to signify the beginning of endGame
{
  SOUND_ON;
  noTone(_pin);
  
  tone(_pin,1500);
  delay(50);

  tone(_pin,1000);
  delay(50);

  tone(_pin,1500);
  delay(50);
  
  noTone(_pin);
  SOUND_OFF;
  }*/

