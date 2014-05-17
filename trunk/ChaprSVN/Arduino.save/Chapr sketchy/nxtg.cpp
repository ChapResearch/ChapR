//
// nxtg.cpp
//
//	(see nxtg.h too)
//
//  These routines know about the message format for the NXT when running NXT-G.
//

#include <arduino.h>
#include "nxtg.h"

//
// nxtGBool()
// nxtGInt()
// nxtGText() - these routines compose a message that will be sent to an NXT
//		running NXT G.  The messages have a fixed format and will
//		but NULL terminated.  The size of the message is returned.
//
//		Note that for the "Int" - it actually transfers a float, but
//		only ints are use for this personality.
//
//		For "text", the size given is WITHOUT the NULL terminator,
//		although the NULL is expected to be there and is copied.
//		The size returned is the entire size including NULL.

int nxtGBool(byte *msgbuff, bool value)
{
     msgbuff[0] = (value)?1:0;
     msgbuff[1] = 0;

     return(2);
}

int nxtGInt(byte *msgbuff, int value)
{
     // the int gets converted to a standard 4-byte float
     float fvalue = value;

     memcpy(msgbuff,&fvalue,4);
     msgbuff[4] = 0;
     return(5);
}

int nxtGText(byte *msgbuff, char *text, int size)
{
     size += 1;
     memcpy(msgbuff,text,size);
     return(size);
}
