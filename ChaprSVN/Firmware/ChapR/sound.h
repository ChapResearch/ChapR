
#ifndef SOUND_H
#define SOUND_H

class sound
{
public:
     sound(int soundPin);

     void confirm();
     void yawn();
     void squeep();
     void icky();
     void start();
     void beep();
     void boop();
     void kill();
     void select();

private:
     int	_pin;
};

#endif SOUND_H
