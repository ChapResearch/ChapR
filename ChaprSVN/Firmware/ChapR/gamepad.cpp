#include <Arduino.h>
#include "VDIP.h"
#include "gamepad.h"

// constructor - loads up a blank (but valid) gamepad

Gamepad::Gamepad(int _id) : translator(NULL), init(NULL), id(_id), x1(0), y1(0), x2(0), y2(0), buttons(0), tophat(0), initialized(false)
{
}

#ifdef DEBUG

void Gamepad::debugPrint(char *prefix)
{
     Serial.print(prefix);
     Serial.print(": X1:");
     Serial.print(x1);
     Serial.print(" Y1:");
     Serial.print(y1);
     Serial.print(" X2:");
     Serial.print(x2);
     Serial.print(" Y1:");
     Serial.print(y2);
     Serial.print(" TOP: ");
     Serial.print(tophat);
     Serial.println("");
}

#endif

//
// clear() - clear the data in the gamepad, setting it to the "neutral" position
//	     for all things.  Centered joysticks, unpressed buttons.
//
void Gamepad::clear()
{
     x1 = y1 = x2 = y2 = 0;
     buttons = 0;
     tophat = 0;
}

//
// load() - given a byte array of data from a USB read of a gamepad, load it into
//	    the standardized form in this gamepad object/structure.  Assumes that
//	    the incoming byte array has 7 useful bytes.
//
void Gamepad::load(byte *usbdata)
{
     x1 = ((int)(usbdata[0]))-128;			// joystick 1 (left) X axis 
     y1 = ((int)(usbdata[1]))-128;			// joystick 1 (left) Y axis 
     x2 = ((int)(usbdata[2]))-128;			// joystick 2 (right) X axis 
     y2 = ((int)(usbdata[3]))-128;			// joystick 2 (right) Y axis 

     // usbdata[4] has buttons 4-1 in top 4 bits
     // usbdata[5] has buttons 12-5

     buttons = ((unsigned int)((usbdata[4]&0xf0)>>4)) | (((unsigned int)usbdata[5])<<4);
     
     tophat = usbdata[4] & 0x0f;				// tophat
     if (tophat & 0x08) {					// 0x08 means that no tophat pressed
         tophat = 0;
     } else{
       tophat++; //we have to map north to 1 to match the canonical form
     }
     //debugPrint("");
}

//
// update() - updates the gamepad to see if anything has changed.  If so, returns true,
//		false if nothing has changed.  If there was a change, the gamepad is
//		updated to include the new values.
//
bool Gamepad::update(VDIP *vdip)
{
     byte	data[MAXTRANSLATE];
     int	count;

     if(!initialized) {
	  initialized = true;
	  if(init != NULL) {
	       (*init)(id,vdip);	// port is 1 or 2 in this case
	  }
     }

     count = vdip->getJoystick(id-1,(char *)data);	// note, ID is either 0 or 1 (not 1 or 2)
     if(translator != NULL) {
	  return((*translator)(data,count,*this));
     }
     return(false);
}

//
// deviceUpdate() - update the internal concept of the device that is plugged into
//		    this particular gamepad.
//
void Gamepad::deviceUpdate(VDIP *vdip)
{
     unsigned short	vid, pid;
     int		type;

     initialized = false;

     // note that we ignore the type - we just match on vid/pid

//     Serial.print("device Update on ");
//     Serial.print(id-1);

     if (vdip->portConnection(id-1,&type,&vid,&pid)) {	// ID is either 0 or 1 (not 1 or 2) in VDIP
	  driverLookup(vid,pid,&translator,&init);	// may come back as NULL too
//	  Serial.print("   VID:0x");
//	  Serial.print(vid,HEX);
//	  Serial.print("   PID:0x");
//	  Serial.print(pid,HEX);
     } else {
	  translator = NULL;
	  init = NULL;
     }
//     if(translator == NULL) {
//	  Serial.print("  (no translator)");
//     }
//     Serial.println("");
}
