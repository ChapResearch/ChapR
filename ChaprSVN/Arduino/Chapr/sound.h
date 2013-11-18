
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
     void up();
     void down();
     void kill();

private:
     int	_pin;
};

#endif SOUND_H
