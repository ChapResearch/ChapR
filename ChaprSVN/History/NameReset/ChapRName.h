
#ifndef CHAPRNAME_H
#define CHAPRNAME_H


class ChapRName
{
public:
     ChapRName();

     void setFromString(char*);
     char *get();
     void setFromConsole();
     bool setFromFlashDrive();

private:
     void write(char*);
     char *read();
};


#endif CHAPRNAME_H