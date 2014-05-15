
//
// comm() - reads data from the VDIP if any exists.  If it does    put it in the line buffer.  It will keep reading
//          until the buffer is full, at which point calls to
//          it will not have any effect.  The buffer is emptied
//          with a call(s) to getLine().  Returns true if the
//          called worked as expected (doesn't mean anything
//          was read) and false if the buffer was ALREADY full,
//          in which case nothing was attempted to be read.
//
bool VDIP::comm()
{
  unsigned char  incoming;

  if(_count >= BUFFER_SIZE) {
    return(false);
  }

  if (recv(&incoming)) {
    _buffer[_count++] = incoming;
  }

  return(true);
}




//
// comm() - reads data from the VDIP if any exists.  If it does
//          put it in the line buffer.  It will keep reading
//          until the buffer is full, at which point calls to
//          it will not have any effect.  The buffer is emptied
//          with a call(s) to getLine().  Returns true if the
//          called worked as expected (doesn't mean anything
//          was read) and false if the buffer was ALREADY full,
//          in which case nothing was attempted to be read.
//
bool VDIP::comm()
{
  unsigned char  incoming;

  if(_count >= BUFFER_SIZE) {
    return(false);
  }

  if (recv(&incoming)) {
    _buffer[_count++] = incoming;
  }

  return(true);
}

//
// getLine() - checks the VDIP comm buffer to see if there is a line that has been received (data terminted by a
//             0x0D).  There may be multiple lines that were received, but this routine gets the FIRST one.
//             The line is copied to the getLine() return buffer WITHOUT the terminating 0x0D.  The comm
//             buffer is slid down so other lines can be processed.  Returns a pointer to the getLine
//             return buffer if a line is ready, NULL otherwise. Note that if the comm buffer is full, then
//             getLine() will "empty" it even though it may not have a terminating 0x0D.
//             NOTE - this only works for incoming '\r'-terminated data.  For data that is "counted" this
//             doesn't work.  fortunately, all async data is '\r'-terminated.
//
unsigned char *getLine()
{
  // special casing the full-buffer case makes this code easier to read!

  if (_count >= BUFFER_SIZE) {          // a little defensive programming with the ">="
    memcpy(_linebuffer,_buffer,_count);
    _linebuffer[_count] = '\0';         // _linebuffer is 1 bigger than _buffer
    _count = 0;
    return(_linebuffer);
  }

  // starting from the beginning, look for a 0x0D (\r)

  for (int i=0; i < _count; i++) {
    if(_buffer[i] == '\r') {
      // found a line!

      memcpy(_linebuffer,_buffer,(size_t)i);  // everything BUT the '\r'
      _linebuffer[i] = '\0';                  // and terminate

      // slide down the remaining data in the comm buffer, adjusting for skipping the '\r'
      i++;
      _count -= i;
      memcpy(_buffer,(&_buffer+i),_count);

      return(_linebuffer);
    }
  }

  // didn't find a line yet
  return(NULL);
}


//
// number of times to loop through setup trying to initialize
//
#define SETUP_LOOPS       3
#define SETUP_TIMEOUT_MS  500

//
// setup() - resets the VDIP and sets communication with it. May timeout, and try again,
//           up to 3 times.  Returns true if the setup worked OK, false if setup was tried
//           a few times and failed.  Then:
//             - configures short codes
//             - checks current devices plugged in, & configures
//
bool VDIP::setup()
{
  unsigned char *line;

  for (int i=SETUP_LOOPS; i--;) {      // try three times with the given timeout

    reset();

    startTime = now();
    timeoutTime = startTime + SETUP_TIMEOUT_MS;

    while(not timed out) {

      comm();      // read data from the VDIP if it exists

      if (line = getLine()) {
	// we have a line, process it, but it needs to be useful
	// which at this case is only the "D>" line
      }
    }

    // at this point we're looking for the prompt that indicates that
    // the VDIP is up and running

    cmd = processCommand();          // process the line that was read
    switch(cmd) {
    }
  }
}

//
// parseResponse() - used to parse responses from VDIP for practically everything.
//                   It knows about the extended responses that occur during start-up
//                   as well as the short-code responses.  Normally, the linebuffer
//                   is given as the argument.  Note that the argument coming in should
//                   NOT have the '\r' at the end.
//
int parseResponse(unsigned char *buffer)
{
  // this routine is written to parse the VDAP firmware responses.  VDFC, VDPS, VDIF,
  //  specific responses are not parsed appropriately, falling into the "unknown" response

  //   Description           Extended         Short & 0x      Note
  //   -----------------     ---------------- -------------   ----------------------------------
  //   cmd completion        "D:\>"           ">"  0x3e       Ready for a(nother) incoming cmd
  //   no disk               "No Disk"        "ND" 0x4e 0x44  In response to a single '\r'
  //   bad cmd               "Bad Command"    "BC" 0x42 0x43  command not recognized
  //   
  //   ASYNC RESPONSES
  //   Description           Extended               Short & 0x            Note
  //   -----------------     ----------------       --------------        ----------------------------------
  //   device into usb 1     "Device Detected P1"   "DD1" 0x44 0x44 0x31  may come up during power-up too
  //   device removed usb 1  "Device Removed P1"    "DR1" 0x44 0x52 0x31
  //   device into usb 2     "Device Detected P2"   "DD1" 0x44 0x44 0x32  may come up during power-up too
  //   device removed usb 2  "Device Removed P2"    "DR1" 0x44 0x52 0x32


  //   DISK OPS RESPONSES
  //   Description           Extended            Short & 0x       Note
  //   -----------------     ----------------    --------------   ----------------------------------
  //   file/dir not found    "Command Failed"    "CF" 0x43 0x46
  //   disk full             "Disk Full"         "DF" 0x44 0x46   should never come up
  //   invalid access        "Invalid"           "FI" 0x46 0x49
  //   read only file        "Read Only"         "RO" 0x52 0x4f
  //   file open already     "File Open"         "FO" 0x46 0x4f
  //   dir not empty         "Dir Not Empty"     "NE" 0x4e 0x45
  //   filename invalid      "Filename Invalid"  "FN" 0x46 0x4e
  //   no upgrade file found "No Upgrade"        "NU" 0x4e 0x55   may come up during power-up too (only on P2)
  //
  //
  // completion of a command
}

//
// getHIDReport() - assuming that the given channel has an USB-HID device attached,
//                  ask for, then wait for, and consume a report from it.  Returns true if the
//                  report was received appropriately, or false if the timeout was hit.
//
