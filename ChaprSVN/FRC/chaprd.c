#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <termios.h>
#include <arpa/inet.h> 
#include <signal.h>
#include <sys/time.h>

#define CHAPR_DATA_SIZE 24

typedef struct chapRPacket{
int cmd;
int joy1_TH_m; // MSB
  int joy1_TH_l; // LSB
  int joy1_type;
  int joy1_x1;
  int joy1_y1;
  int joy1_B1;
  int joy1_x2;
  int joy1_y2;
  int joy1_B2;
  int joy1_x3;
  int joy1_y3;
  int zero;
  int joy2_TH_m; // two bytes
  int joy2_TH_l; // two bytes
  int joy2_type;
  int joy2_x1;
  int joy2_y1;
  int joy2_B1;
  int joy2_x2;
  int joy2_y2;
  int joy2_B2;
  int joy2_x3;
  int joy2_y3;
} chapRPacket;

// TODO - kill signal, logging

void signalHandler(int signal)
{
  syslog(LOG_INFO, "received signal: %d", signal);
}

void logMsg(char *msg)
{
	
}

void errorMsg(char *msg)
{
  fprintf(stderr,"ERROR: %s\n",msg);
}

void debug_string(char *msg, char *arg)
{
#ifdef DEBUG 
  printf("DEBUG: %s%s\n", msg, arg);
#endif
}

void debug_int(char *msg, int arg)
{
#ifdef DEBUG 
  printf("DEBUG: %s%i\n", msg, arg);
#endif
}

void debug_dump(char *msg, char *buffer, int size){
#ifdef DEBUG
  int i;
  for (i = 0; i < size; i++){
    printf("%i:%x\n", i, buffer[i]);
  }
#endif
}

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
  long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  result->tv_sec = diff / 1000000;
  result->tv_usec = diff % 1000000;

  return (diff<0);
}

/*
readChapRPacket() - formats data from the USB into a ChapR packet
                    (waits until it finds one if USB present)
*/
chapRPacket *readChapRPacket(int fd)
{
  static chapRPacket cp;
  unsigned char buf[50]; // overkill, but no one cares
  unsigned char rawData;
  int state = 0;
  unsigned char checkSum = 0;
  int count = 0;

  static struct timeval begin, end, diff; // used for timeout of connection
  static struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 200000L; // operate at 5K BAUD (a little slower, because of processing time)
  // used to be 200000L
  struct timespec timeLeft; // not used, but still needed as a parameter
  static int connected = 0; 
  static int firstZero = 0;


  while (1){
    debug_string("about to read", "");
    int rval = read(fd, (void *) &rawData, 1);
    debug_string("just read", "");
    if (rval < 0){
      syslog(LOG_CRIT, "read failed (rval < 0, errno %d)...exiting", errno);
      debug_string("read failed", "");
      closelog();
      exit(1);
    }
    else if (rval == 0){
      debug_string("read 0", "");
      if (connected){ // if we think we are still connected
	if (firstZero == 0){ // if this is the first indication of a problem
	  firstZero = 1;
	  gettimeofday(&begin, NULL);
	} else {
	  gettimeofday(&end, NULL);
	  timeval_subtract(&diff, &end, &begin);
	  printf("elapsed time%ld\n", diff.tv_sec);
	  if (diff.tv_sec > 5){
	    connected = 0;
	    return (chapRPacket *) NULL;
	  }
	}
      }
      debug_string("not connected", "");
    } else {
      firstZero = 0; // looks like there isn't a problem after all
      if (connected == 0){
	connected = 1;
      }
      switch (state){
      case 0:
      case 1:
      case 2:
	if (rawData == 0xff){
	  state++;
	  checkSum = 0;
	  count = 0;
	} else {
	  state = 0;
	}
	break;
      case 3:
	buf[count] = rawData;
	checkSum += rawData;
	count++;
	if (count == CHAPR_DATA_SIZE){
	  state++;
	}
	break;

      case 4:
	checkSum = checkSum & 0x7f;
	if (checkSum == rawData){
	  cp.cmd        = (int) buf[0];
	  cp.joy1_TH_m  = (int) buf[1];
	  cp.joy1_TH_l  = (int) buf[2];
	  cp.joy1_type  = (int) buf[3];
	  cp.joy1_x1    = (int) buf[4];
	  cp.joy1_y1    = (int) buf[5];
	  cp.joy1_B1    = (int) buf[6];
	  cp.joy1_x2    = (int) buf[7]; //was 7
	  cp.joy1_y2    = (int) buf[8]; //was 8
	  cp.joy1_B2    = (int) buf[9];
	  cp.joy1_x3    = (int) buf[10]; //was 10
	  cp.joy1_y3    = (int) buf[11]; //was 11

// zero
	  cp.joy2_TH_m  = (int) buf[13];
	  cp.joy2_TH_l  = (int) buf[14];
	  cp.joy2_type  = (int) buf[15];
	  cp.joy2_x1    = (int) buf[16];
	  cp.joy2_y1    = (int) buf[17];
	  cp.joy2_B1    = (int) buf[18];
	  cp.joy2_x2    = (int) buf[19];
	  cp.joy2_y2    = (int) buf[20];
	  cp.joy2_B2    = (int) buf[21];
	  cp.joy2_x3    = (int) buf[22];
	  cp.joy2_y3    = (int) buf[23];
	  // zero
	  return &cp;
	} else {
	  debug_string("g", "");
	  state = 0;
	}
	break;
      } // end of switch
    } // end of else
    nanosleep(&sleepTime, &timeLeft);
  } // end of while
}

int translateHeader(char *buffer, chapRPacket *cp)
{
  static int index = 1;
  int size = 0;

  buffer[size++] = index >> 8; // big endian form of the index
  buffer[size++] = index & 0x00FF;
  buffer[size++] = (int) 1; // protocol version
  buffer[size++] = cp->cmd; // cmd to be sent
  buffer[size++] = 0x10; // request progStart
  buffer[size++] = (int) 0; // alliance station ID

  index++;
  return size;
}

#define kJoystick1Tag 12
#define kJoystickDesc1TCPTag 2
#define DEVTYPE_JOYSTICK 0x14

/* translateChapRPacket() - translates from a received ChapR packet to a v1 driver's station packet
			    for transmission over UDP
*/
int translateChapRPacket(char *buffer, chapRPacket *cp)
{
  int size = translateHeader(buffer, cp);

  // -----------------------------------------1st joystick--------------------------------------------
  switch(cp->joy1_type){
  case 0: // Gamestop Xbox 360 
  case 1: // Afterglow Xbox 360 
  case 2: // Microsoft Xbox 360 
    buffer[size++] = (uint) 0x0e; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x06; // # of axes
    buffer[size++] = cp->joy1_x1;
    buffer[size++] = cp->joy1_y1;
    buffer[size++] = cp->joy1_x2;
    buffer[size++] = cp->joy1_y2;
    buffer[size++] = cp->joy1_x3;
    buffer[size++] = cp->joy1_y3;
    buffer[size++] = (uint) 0x0a; // # of bttns
    buffer[size++] = (uint) ((cp->joy1_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy1_B1) | ((cp->joy1_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x01; // # of POV's (or D-pad's)
    buffer[size++] = cp->joy1_TH_m;
    buffer[size++] = cp->joy1_TH_l;
    break;
  case 3: // Logitech F310
  default:
    buffer[size++] = (uint) 0x0c; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x04; // # of axes
    buffer[size++] = cp->joy1_x1;
    buffer[size++] = cp->joy1_y1;
    buffer[size++] = cp->joy1_x2;
    buffer[size++] = cp->joy1_y2;
    buffer[size++] = (uint) 0x0c; // # of bttns
    buffer[size++] = (uint) ((cp->joy1_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy1_B1) | ((cp->joy1_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x01; // # of POV's (or D-pad's)
    buffer[size++] = cp->joy1_TH_m;
    buffer[size++] = cp->joy1_TH_l;
    break;
  case 5:
    buffer[size++] = (uint) 0x09; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x03; // # of axes
    buffer[size++] = cp->joy1_x1;
    buffer[size++] = cp->joy1_y1;
    buffer[size++] = cp->joy1_x2;
    buffer[size++] = (uint) 0x0b; // # of bttns
    buffer[size++] = (uint) ((cp->joy1_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy1_B1) | ((cp->joy1_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x00; // # of POV's (or D-pad's)
    break;
  }

  // -------------------------------------------2nd joystick -----------------------------------------------
  switch(cp->joy2_type){
  case 0: // Gamestop Xbox 360 
  case 1: // Afterglow Xbox 360 
  case 2: // Microsoft Xbox 360 
    buffer[size++] = (uint) 0x0e; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x06; // # of axes
    buffer[size++] = cp->joy2_x1;
    buffer[size++] = cp->joy2_y1;
    buffer[size++] = cp->joy2_x2;
    buffer[size++] = cp->joy2_y2;
    buffer[size++] = cp->joy2_x3;
    buffer[size++] = cp->joy2_y3;
    buffer[size++] = (uint) 0x0a; // # of bttns
    buffer[size++] = (uint) ((cp->joy2_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy2_B1) | ((cp->joy2_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x01; // # of POV's (or D-pad's)
    buffer[size++] = cp->joy2_TH_m;
    buffer[size++] = cp->joy2_TH_l;
    break;
  case 3: // Logitech F310
  default:
    buffer[size++] = (uint) 0x0c; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x04; // # of axes
    buffer[size++] = cp->joy2_x1;
    buffer[size++] = cp->joy2_y1;
    buffer[size++] = cp->joy2_x2;
    buffer[size++] = cp->joy2_y2;
    buffer[size++] = (uint) 0x0c; // # of bttns
    buffer[size++] = (uint) ((cp->joy2_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy2_B1) | ((cp->joy2_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x01; // # of POV's (or D-pad's)
    buffer[size++] = cp->joy2_TH_m;
    buffer[size++] = cp->joy2_TH_l;
    break;
  case 5:
    buffer[size++] = (uint) 0x09; // size of data
    buffer[size++] = (uint) kJoystick1Tag;
    buffer[size++] = (uint) 0x03; // # of axes
    buffer[size++] = cp->joy2_x1;
    buffer[size++] = cp->joy2_y1;
    buffer[size++] = cp->joy2_x2;
    buffer[size++] = (uint) 0x0b; // # of bttns
    buffer[size++] = (uint) ((cp->joy2_B2)>>1); // ChapR packet had been offset to prevent 'FF's
    buffer[size++] = (uint) (cp->joy2_B1) | ((cp->joy2_B2&0x01)<<7); // moves button from B2 to B1
    buffer[size++] = (uint) 0x00; // # of POV's (or D-pad's)
    break;
  }
	
  return size;
}

/* formatInfo() - translates the index of the joystick type into info about that joystick
                and formats the info into the "Joystick Descriptor" to be sent over TCP.
                This function will be called for each existing joystick by translateJoystickDesc()
*/
int formatInfo(int joystick_type, char *buffer, int start_val, int index)
{
  int size = start_val;
  uint numAxes, bttnCount, numPOV, isXbox;
  char *name;
  uint axes[10];
  switch(joystick_type){
  case 0: // Gamestop Xbox 360 
  case 1: // Afterglow Xbox 360 
  case 2: // Microsoft Xbox 360 
    isXbox = 1;
    name = "XBOX 360";
    numAxes = 6;
    bttnCount = 10;
    numPOV = 1;
    break;
  case 3: // Logitech F310
  default:
    isXbox = 0;
    name = "Logitech Dual Action";
    numAxes = 4;
    axes[0] = 0;
    axes[1] = 1;
    axes[2] = 2;
    axes[3] = 5;
    bttnCount = 12;
    numPOV = 1;
    break;
  case 5:
    isXbox = 0;
    name = "Logitech Attack 3 USB";
    numAxes = 3;
    axes[0] = 0;
    axes[1] = 1;
    axes[2] = 2;
    bttnCount = 11;
    numPOV = 0;
    break;
  }

  buffer[size++] = 0; // placeholder for MSB of size
  buffer[size++] = 0; // placeholder for LSB of size
  buffer[size++] = kJoystickDesc1TCPTag; // tag
  buffer[size++] = index; // index
  buffer[size++] = isXbox;
  buffer[size++] = DEVTYPE_JOYSTICK; // type
  buffer[size++] = (uint) strlen(name); // name length

  int i;
  for (i = 0; i < strlen(name); i++){
    buffer[size++] = name[i];
  }
  buffer[size++] = numAxes;
  for (i = 0; i < numAxes; i++){
    buffer[size++] = axes[i];
  }
  buffer[size++] = bttnCount;
  buffer[size++] = numPOV;
  
  buffer[start_val] = (size - start_val - 2)>>8; // MSB of index
  buffer[start_val+1] = (size - start_val - 2)&0x00FF; // LSB of index

  return size - start_val; // return true size, rather than position
}

/* translateJoystickDesc() - formats the joystick descriptor to be sent over TCP, including
                           placeholders for all joysticks after the first two. NOTE: the first
                           two joysticks are always assumed to be present. The ChapR does not
                           indicate whether a joystick is plugged in or not. It will simply
                           send zeroes if nothing is plugged in.
*/
int translateJoystickDesc(char *buffer, chapRPacket *cp)
{
  int size = formatInfo(cp->joy1_type, buffer, 0, 0);
  size += formatInfo(cp->joy2_type, buffer, size, 1);

  // fills rest of joystick tags with empty values
  int i;
  for (i = 2; i < 6; i++){
    buffer[size++] = (uint) 0x00; // MSB of size
    buffer[size++] = (uint) 0x08; // LSB of size
    buffer[size++] = kJoystickDesc1TCPTag; // tag
    buffer[size++] = (uint) i; // index
    buffer[size++] = (uint) 0; // isXBox
    buffer[size++] = (uint) 0xFF; // type is null
    buffer[size++] = (uint) 0; // name length
    buffer[size++] = (uint) 0; // axes count
    buffer[size++] = (uint) 0; // button count
    buffer[size++] = (uint) 0; // POV count
    // TODO - add the above casts everywhere
  }
  return size;
}

// returns the number of bytes sent (-1 on error)
int send_packet (int port, int sd, struct hostent *hp, char *buffer, int theSize)
{
  struct sockaddr_in	dest_addr;
  
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  memcpy((void *)&dest_addr.sin_addr, hp->h_addr_list[0], hp->h_length);
  
  return sendto(sd, (void *)buffer, (size_t)theSize, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr));
}	

// UDP_send() - given the open UDP socket, send a packet to it
//
void UDP_send(int sd, struct hostent *hp, char *buffer, int theSize)
{
  int port = 1110;
  
  if (send_packet(port, sd, hp, buffer, theSize) == -1){
    debug_int("UDP_errno: ", errno);
  }
}

// TCP_send() - given the open TCP socket, send a packet to it
//
void TCP_send(int sd, struct hostent *hp, char *buffer, int theSize)
{
  int port = 1740;

  if (send_packet(port, sd, hp, buffer, theSize) == -1){
    debug_int("TCP_errno: ", errno);
  }
}

//
// findMe() - go find my local loopback address.  I know, I know, it is 127.0.0.1, but the host
//			table REALLY should be consulted.
struct hostent *findMe()
{
  struct hostent *hp;

  hp = gethostbyname("localhost");
  if (!hp) {
    errorMsg("could not find ME!");
    return (struct hostent *)NULL;
  }

  return(hp);
}

//
// UDP_init() - returns an open socket for sending networking packets to the loopback address (127.0.0.1)
//
int UDP_init()
{
  int fd;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    errorMsg("cannot create socket");
    debug_int("UDP init errno", errno);
  }

  debug_string("initialized UDP", "");
  return(fd);
}

//
// TODO
//
void sendHelloPackets(int sd, struct hostent *hp)
{
  unsigned char hello[6];
  hello[0] = 0;
  hello[2] = 1;
  hello[3] = 0;
  hello[4] = 0;
  hello[5] = 0;
  int i;
  for (i = 0; i < 3; i++){
    hello[1] = i; // set the index of the packet
    UDP_send(sd, hp, hello, 6);
    sleep(1);
  }
}

//
// TCP_init() - returns an open socket for sending networking packets to the loopback address (127.0.0.1)
//			Code taken from http://www.thegeekstuff.com/2011/12/c-socket-programming/
//
int TCP_init(struct hostent *hp)
{
  debug_string("connecting", "");
  int fd;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    errorMsg("cannot create socket");
  }

  int port = 1740;
	
  static struct sockaddr_in dest_addr;

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_port = htons(port);
  memcpy((void *)&dest_addr.sin_addr, hp->h_addr_list[0], hp->h_length);

  while (connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1){
    debug_int("TCP_connect_error:", errno);
    sleep(1);
  }

  return(fd);
}

//
// openUSBPort() - opens the appropriate USB port (the one with the FirePlug).
//                 Returns an open file descriptor (read-only) for the USB port.
//                 Blocks while waiting for an appropriate USB port.
//
int openUSBPort(){
  struct stat buf;
  char *ports[2] = {"/dev/ttyUSB0","/dev/ttyUSB1"}; 
  int i;
  int fd;
  struct termios t;
	
  t.c_iflag = IGNBRK | IGNPAR;
  t.c_cflag = CS8 | CREAD | CLOCAL | B38400;

  while (1){
    for (i = 0; i < 2; i++){
      // check if ttyUSB0 or 1 is available
      // TODO - check if it has FirePlug connected
      // TODO - do testing of plugging/unplugging and other USB devices
      if (stat(ports[i], &buf) == 0){
	syslog(LOG_INFO, "opened port: %s",ports[i]);
	fd = open(ports[i], O_RDONLY);				
	debug_int("open USB errno: ", errno);
	debug_int("stat: ", buf.st_dev);
	tcsetattr(fd, TCSANOW, &t);
	return fd;
      }			
    }
    // loop every x seconds, waiting patiently
    debug_string("checked once", "");
    sleep(10);
  }
}

void TCP_ping(int sd, struct hostent *hp)
{
  unsigned char buf[2];
  buf[0] = buf[1] = 0;
  TCP_send(sd, hp, buf, sizeof(buf));
}

int daemonize(){
        
  /* Our process ID and Session ID */
  pid_t pid, sid;
        
  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then
     we can exit the parent process. */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* Change the file mode mask */
  umask(0);
                
  /* Open any logs here */        
                
  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }
        
  /* Change the current working directory */
  if ((chdir("/")) < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }
        
  /* Close out the standard file descriptors */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  return pid;
}

int main(void) {

  int pid;

  openlog("ChapR", LOG_PID | LOG_NOWAIT | LOG_CONS | LOG_NDELAY, LOG_DAEMON);
  syslog(LOG_INFO, "chaprd running...");

#ifndef DEBUG
  pid = daemonize(); // initializes the daemon
  syslog(LOG_INFO, "Daemonized");
#endif
  (void) signal(SIGHUP, signalHandler);
  (void) signal(SIGPIPE, signalHandler); // can also use SIG_IGN to ignore the signal errno EPIPE

  /* The Big Loop */
  while (1) {

    struct hostent *hp = findMe(); // get loopback address
	  
    int udp_sd = UDP_init();
    sendHelloPackets(udp_sd, hp); // causes TCP port to be opened

    int tcp_sd = TCP_init(hp);

    static int joy1_type = 0; // indicates if a new joystick has been inputted
    static int joy2_type = 0; // indicates if a new joystick has been inputted

    int fd = -1;
    while (fd == -1){
      fd = openUSBPort();
      debug_int("fd of USB: ", fd);	
    }
    while (1){
      chapRPacket *cp;

      cp = readChapRPacket(fd);
      debug_string("read packet!", "");
      if (cp == NULL){
	debug_string("USB Port was closed somehow", "");
	break; // the USB port was closed somehow
      }
      if (cp->joy1_type != joy1_type || cp->joy2_type != joy2_type){ // new joystick plugged in
	unsigned char tcp_buffer[1024];
	int size = translateJoystickDesc(tcp_buffer, cp);
	TCP_send(tcp_sd, hp, tcp_buffer, size); // inform roboRIO of new type
	debug_string("sent TCP packet", "");
	joy1_type = cp->joy1_type;
	joy2_type = cp->joy2_type;
      }
      unsigned char udp_buffer[1024]; 
      int size = translateChapRPacket(udp_buffer, cp);
      UDP_send(udp_sd,hp,udp_buffer,size);
      debug_string("sent UDP packet",""); 
    }	
    close(fd); // close fd to FirePlug
    syslog(LOG_INFO, "just closed port: %d", fd);
    joy1_type = 0; // reset joy1
    joy2_type = 0; // reset joy2

    // while it may be unneccessary to close the TCP/UDP ports, there is
    // no reason to let them simply time out; might as well close and reopen them
    close(tcp_sd); // close TCP socket
    syslog(LOG_INFO, "just closed port: %d", tcp_sd);
    close(udp_sd); // close UDP socket
    syslog(LOG_INFO, "just closed port: %d", udp_sd);
    sleep(5);
  }

  exit(EXIT_SUCCESS);
}
