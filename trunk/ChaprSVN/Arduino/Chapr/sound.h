
#ifndef SOUND_H
#define SOUND_H

class sound
{
public:
     sound(int soundPin);

     void confirm();
     void squeep();

private:
     int	_pin;

};

#endif SOUND_H
