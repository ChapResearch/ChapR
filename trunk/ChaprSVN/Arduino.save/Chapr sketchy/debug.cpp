//
// debug.cpp
//
//	Debug routines.
//

#include <arduino.h>
#include "debug.h"

void dumpDataHex(char *label, byte *buffer, int count)
{
     Serial.print(label);
     Serial.print(": ");
     dumpDataHex(buffer,count);
}

void dumpDataHex(byte *buffer, int count)
{	
     int	i = 0, value;

     for(;count > 0; count--, i++, buffer++) {
	  value = (unsigned char)*buffer;
	  Serial.print("0x");
	  if( value< 16) {
	       Serial.print("0");
	  }
	  Serial.print(value,HEX);
	  Serial.print(" ");
	  if( i%8 == 7) {
	       Serial.println("");
	  }
     }

     // if we didnt' end on an even boundary, kick out a newline

     if(i%8 != 0) {
	  Serial.println("");
     }
}

