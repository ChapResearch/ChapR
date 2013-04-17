
//
//  The remote control is layout like this:
//	  LEFT 	     ,--------------------,      RIGHT
//     1---, 2---,   | ,----------------, |   1---, 2---,
//     | + | | + |   | |()()        ()()| |   | + | | + |
//     '---' '---'   | |()() 	    ()()| |   '---' '---'
//     3---, 4---,   | |()()   	    ()()| |   3---, 4---,
//     | + | | + |   | |  () 	    ()  | |   | + | | + |
//     '---' '---'   | |     	        | |   '---' '---'
//     5---, 6---,   | '----------------' |   5---, 6---,
//     | + | | + |   |    .-, ,--, ,-.    |   | + | | + |
//     '---' '---'   |   '._| |__| |_.'   |   '---' '---'
//     7---, 8---,   |        ,--,        |   7---, 8---,
//     | + | | + |   |        '--'        |   | + | | + |
//     '---' '---'   |      	          |   '---' '---'
//     	       	     '--------------------'
//
//  The remote check routine returns a short (16 bits) with
//  with the 16 buttons encoded within them.  Note that there
//  may NOT be 16 buttons in any implementations (including
//  the first that this code supports).  The state of the
//  buttons is return in two bytes:
//
//            LEFT (MSB)         RIGHT (LSB)
//          _ _ _ _ _ _ _ _    _ _ _ _ _ _ _ _
//  ushort |8|7|6|5|4|3|2|1|  |8|7|6|5|4|3|2|1|
//
//  This encoding is what's passed around.  When the buttons'
//  hook-up configuration changes, this remains the same.
//
//  Defines make checking for buttons easier and if things change
//  the defines remain the same.  RL1 = Remote Left 1

#define RL1   (0x0100)
#define RL2   (0x0200)
#define RL3   (0x0400)
#define RL4   (0x0800)
#define RL5   (0x1000)
#define RL6   (0x2000)
#define RL7   (0x4000)
#define RL8   (0x8000)

#define RR1   (0x0001)
#define RR2   (0x0002)
#define RR3   (0x0004)
#define RR4   (0x0008)
#define RR5   (0x0010)
#define RR6   (0x0020)
#define RR7   (0x0040)
#define RR8   (0x0080)

//
// remoteInit() - called to initialize the remote.  At this point
//                all it does is configure the sensors appropriately.
//                This routine is intimately tied to the way in which
//                the switches are wired.  Current configuration is:
//
//	  LEFT 	  	    		       	 RIGHT
//	 ,-----------,  ,---------,   ,---------------,
//     1-'-, 2---,   '--|3  MUX  2|---'       1---, 2-'-,
//     | + | | + |------|4  out  1|-----------| + | | + |
//     '---' '---'      '-----,---'           '---' '---'
//       ,------------,       |	       	   ,----------,
//     3-'-, 4---,    '-,   ,---------,  ,-'  3---, 4-'-,
//     | + | | + |---,  '---|1  MUX  4|--' ,--| + | | + |
//     '---' '---'   '------|2  out  3|----'  '---' '---'
//       ,---------- NC     '-----,---'   NC----------,
//     5-'-, 6---,            |   |           5---, 6-'-,
//     | + | | + |--------,   |   |   ,-------| + | | + |
//     '---' '---'        |   |   |   |       '---' '---'
//	 ,------------ NC |   |   |   |  NC ----------,
//     7-'-, 8---,        |   |   |   |       7---, 8-'-,
//     | + | | + |---- NC |   |   |   |  NC --| + | | + |
//     '---' '---'        S4  S1  S2  S3      '---' '---'
//
//
void remoteInit()
{
  SensorType[S1] = sensorHiTechnicTouchMux;
  SensorType[S2] = sensorHiTechnicTouchMux;
}

//
// remoteSample() - samples the remote and returns an unsigned short
//                  (two bytes) organized as described above.  Note
//                  that this routine is intimately tied to the way
//                  in which the remote is wired - see remoteInit()
//                  for the coded configuration of the hardware.
//
unsigned short remoteSample()
{
  unsigned short   left = 0;
  unsigned short   right = 0;

  right = (SensorValue[S1] & 0x0003) | (SensorValue[S2] & 0x000c) | (SensorValue[S3]?0x0010:0);
  left = ((SensorValue[S1] & 0x000c)>>2) | ((SensorValue[S2] & 0x0003)<<2) | (SensorValue[S4]?0x0010:0);

  return((left<<8) | right);
}

//
// remoteDisplay() - given a byte (actually the bottom eight bits of an
//                   unsigned short) display the state of the buttons
//                   beginning at the given x/y and moving down and to
//                   the right.  The 1's in the given mask tell which
//                   buttons to draw (ie - 0xff draws all eight buttons)
//
void remoteDisplay(unsigned short target, int x, int y, unsigned short mask)
{
#define SPACING 14
  int    i;
  int    x_save;

  x_save = x;
  for( i = 0; i < 8; i++) {
    if(mask&0x01) {
      if(target&0x01) {
	nxtDisplayRICFile(x,y,"buttonOn.ric");
      } else {
	nxtDisplayRICFile(x,y,"buttonOff.ric");
      }
    }
    target = target>>1;
    mask = mask>>1;
    x += SPACING;
    if (i % 2) {
      y -= SPACING;
      x = x_save;
    }
  }
}
