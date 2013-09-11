** Sun Sep 08 11:55:15 2013 **

The files in this directory are the firmware files for the VDIP2
(based upon the VNC1L) from FTDI (called vinculum, too).

The ".FTD" files are firmware files that can be loaded to the VDIP
from flash.  If there are multiple versions in this directory, they
will be named appropriately.  To use them they must be name FTRFB.FTD.

The ".ROM" files are firmware files that must be loaded using the
bootloader procedure.  Same type of activity, but a different way of
loading than with the FTD files.

Some of the VDIPs we get need firmware updates to act correctly.  this
need is indicated by the VDIP coming up, but not reading gamepads
correctly.  Normally, a flash drive with an FTD file works great,
UNLESS you accidentally already have a ChapR load (v0.3 and later)
that has flash drive reading.  Then it can easily interfere.

Unfortunately, if this happens it is possible to "brick" (make
inoperable) a VDIP2.  If this happens, the FTD load procedure may no
longer work.  the ROM procedure must be used.  The rest of this
document describes that procedure.

To do this, you need to use a modified FTDI programmer.  This is the
programmer we use to program the Arduino.  See the directory
"ChaprSVN/Reference/FTDI Breakout Board" for the instructions how to
modify the FTDI programmer board.  It's pretty simple.

Then a 5 (or 6 I guess) wire cable needs to be created that connects
to the 6-pin connector of the modified FTDI programmer.  Here is the
map for the wires to create the VDIP add-on for connecting to the
modified FTDI breakout:

		       FTDI Programmer
VDIP2 Pin  Wire Color   Connector Pin   Description
--- -----  ---- ------  ------ ------   ----- -----
26 (GND)     	Black	       1	GND		(can be any GND really)
17 (RTS)	Brown	       2	CTS
NC		Red	       3	POWER (5v)
16 (RXD)	Orange	       4	TX0
14 (TXD)	Yellow	       5	RXI
18 (CTS)	Green	       6	RTS (prev DTR)
31 (/PROG)	Black	       X	/PROG - hold low to program

These wires are temporarily soldered to the top of the VDIP.  For
the /PROG I connected a jumper to the pin.  

That's the preparation.  To do the programming you use the VPROG
program on the PC.  this program can be found on the FTDI web site,
but is also in this directory.

But before programming the VDIP, though, you need to load the assist
program onto the ChapR so it will allow the programming by VPROG.
This program is in this directory too.

Turn on the ChapR while running on battery.  The assist program will
keep it powered up.  Then press the WFS button, which will reset the
VDIP, and since the /PROG pin is wired low, it will reset into
programming mode.  Then the VPROG program will work.

