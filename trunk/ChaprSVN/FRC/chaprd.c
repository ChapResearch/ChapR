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

#define CHAPR_PCKT_SIZE 33

typedef struct chapRPacket{
  int cmd;
  int digitalIn;
  int zero1;
  int analog1; // two bytes
  int joy1_TH;
  int analog2; // two bytes
  int joy1_B1;
  int analog3; // two bytes
  int joy1_B2;
  int analog4; // two bytes
  int zero2;
  int joy1_x1;
  int joy1_y1;
  int zero;
  int joy1_x2;
  int joy1_y2;
  int joy2_B1;
  int joy1_x3;
  int joy1_y3;
  int zero3;
  int joy2_x1;
  int joy2_y1;
  int joy2_B2;
  int joy2_x2;
  int joy2_y2;
  int joy2_TH;
  int joy2_x3;
  int joy2_y3;
} chapRPacket;

typedef struct dsPacket{
  int indexm;
  int indexl;
  int zconst1;
  int control;
  int zconst2;
  int posally;
  int nconst3;
  int zconst4;
  int joy1_x1;
  int joy1_y1;
  int joy1_x2;
  int joy1_y2;
  int joy1_x3;
  int joy1_y3;
  int joy1_B2;
  int joy1_B1;
  int nconst6;
  int zconst7;
  int joy2_x1;
  int joy2_y1;
  int joy2_x2;
  int joy2_y2;
  int joy2_x3;
  int joy2_y3;
  int joy2_B2;
  int joy2_B1;
  int nconst8;
  int zconst9;
  int joy3_x1;
  int joy3_y1;
  int joy3_x2;
  int joy3_y2;
  int joy3_x3;
  int joy3_y3;
  int joy3_B2;
  int joy3_B1;
  int nconst10;
  int zconst11;
  int joy4_x1;
  int joy4_y1;
  int joy4_x2;
  int joy4_y2;
  int joy4_x3;
  int joy4_y3;
  int joy4_B2;
  int joy4_B1;
  int nconst12;
  int zconst13;
  int joy5_x1;
  int joy5_y1;
  int joy5_x2;
  int joy5_y2;
  int joy5_x3;
  int joy5_y3;
  int joy5_B2;
  int joy5_B1;
  int nconst14;
  int zconst15;
  int joy6_x1;
  int joy6_y1;
  int joy6_x2;
  int joy6_y2;
  int joy6_x3;
  int joy6_y3;
  int joy6_B2;
  int joy6_B1;
} dsPacket;

// TODO - kill signal, logging

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

//
// this define converts the two-byte chapr FRC analogs to an int
//
#define ANALOG_CONVERT(buffer,i1,i2)	((buffer[i1]<8)|buffer[i2])

//
// readChapRPacket() - formats data from the USB into a ChapR packet
//                     (waits until it finds one if USB present)
//
chapRPacket *readChapRPacket(int fd)
{
	static chapRPacket cp;
	unsigned char buf[50]; // overkill, but no one cares
	unsigned char rawData;
	int state = 0;
	int checkSum = 0;
	int count = 0;

	while (1){
	  int rval = read(fd, (void *) &rawData, 1);
		if (rval <= 0){
		  if (rval == 0){
		    debug_int("zero rval:", errno);
		    if (errno == ENOENT){ // file descriptor is no longer valid
		      debug_int("bad f:", errno);
		      return (chapRPacket *) NULL;
		    } else {
		      continue;
		    }
		  }
		  debug_int("goodbye cruel world...:", errno);
		  exit(1);
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
			if (count == CHAPR_PCKT_SIZE){
				state++;
			}
			break;

		case 4:
			checkSum = checkSum & 0x7f;
			if (checkSum == rawData){
			  cp.cmd = (int) buf[0];
			  cp.digitalIn = (int) buf[1];
			  // zero
			  cp.analog1 = ANALOG_CONVERT(buf,3,4);
			  cp.joy1_TH = (int) buf[5];
			  cp.analog2 = ANALOG_CONVERT(buf,6,7);
			  cp.joy1_B1 = (int) buf[8];
			  cp.analog3 = ANALOG_CONVERT(buf,9,10);
			  cp.joy1_B2 = (int) buf[11];
			  cp.analog4 = ANALOG_CONVERT(buf,12,13);
			  // zero
			  cp.joy1_x1 = (int) buf[15];
			  cp.joy1_y1 = (int) buf[16];
			  // zero
			  cp.joy1_x2 = (int) buf[18];
			  cp.joy1_y2 = (int) buf[19];
			  cp.joy2_B1 = (int) buf[20];
			  cp.joy1_x3 = (int) buf[21];
			  cp.joy1_y3 = (int) buf[22];
			  // zero
			  cp.joy2_x1 = (int) buf[24];
			  cp.joy2_y1 = (int) buf[25];
			  cp.joy2_B2 = (int) buf[26];
			  cp.joy2_x2 = (int) buf[27];
			  cp.joy2_y2 = (int) buf[28];
			  cp.joy2_TH = (int) buf[29];
			  cp.joy2_x3 = (int) buf[30];
			  cp.joy2_y3 = (int) buf[31];
			  return &cp;

			} else {
			  debug_string("g", "");
				state = 0;
			}
			break;
		}
	}
}

// 
// translateChapRPacket() - translates from a received ChapR packet to a driver's station packet
//				for transmission over the ethernet (socket)
//
dsPacket *translateChapRPacket(chapRPacket *cp)
{
	static dsPacket dsp;
	static int index = 0;
	index++;

	dsp.indexm = (int) ((index & 0xff00)>>8);
	dsp.indexl = (int) (index & 0xff);
	dsp.zconst1 = (int) 0;
	dsp.control = cp->cmd; 
	dsp.zconst2 = (int) 0;
	dsp.posally = (int) 0; //red 1
	dsp.nconst3 = (int) 9;
	dsp.zconst4 = (int) 0;
	dsp.joy1_x1 = cp->joy1_x1;
	dsp.joy1_y1 = cp->joy1_y1;
	dsp.joy1_x2 = cp->joy1_x2;
	dsp.joy1_y2 = cp->joy1_y2;
	dsp.joy1_x3 = cp->joy1_x3;
	dsp.joy1_y3 = cp->joy1_y3;
	dsp.joy1_B2 = cp->joy1_B2;
	dsp.joy1_B1 = cp->joy1_B1;
	dsp.nconst6 = (int) 9;
	dsp.zconst7 = (int) 0;
	dsp.joy2_x1 = cp->joy2_x1;
	dsp.joy2_y1 = cp->joy2_y1;
	dsp.joy2_x2 = cp->joy2_x2;
	dsp.joy2_y2 = cp->joy2_y2;
	dsp.joy2_x3 = cp->joy2_x3;
	dsp.joy2_y3 = cp->joy2_y3;
	dsp.joy2_B2 = cp->joy2_B2;
	dsp.joy2_B1 = cp->joy2_B1;
	dsp.nconst8 = (int) 0;
	dsp.zconst9 = (int) 9;
	dsp.joy3_x1 = (int) 0;
	dsp.joy3_y1 = (int) 0;
	dsp.joy3_x2 = (int) 0;
	dsp.joy3_y2 = (int) 0;
	dsp.joy3_x3 = (int) 0;
	dsp.joy3_y3 = (int) 0;
	dsp.joy3_B2 = (int) 0;
	dsp.joy3_B1 = (int) 0;
	dsp.nconst10 = (int) 9;
	dsp.zconst11 = (int) 0;
	dsp.joy4_x1 = (int) 0;
	dsp.joy4_y1 = (int) 0;
	dsp.joy4_x2 = (int) 0;
	dsp.joy4_y2 = (int) 0;
	dsp.joy4_x3 = (int) 0;
	dsp.joy4_y3 = (int) 0;
	dsp.joy4_B2 = (int) 0;
	dsp.joy4_B1 = (int) 0;
	dsp.nconst12 = (int) 9;
	dsp.zconst13 = (int) 0;
	dsp.joy5_x1 = (int) 0;
	dsp.joy5_y1 = (int) 0;
	dsp.joy5_x2 = (int) 0;
	dsp.joy5_y2 = (int) 0;
	dsp.joy5_x3 = (int) 0;
	dsp.joy5_y3 = (int) 0;
	dsp.joy5_B2 = (int) 0;
	dsp.joy5_B1 = (int) 0;
	dsp.nconst14 = (int) 9;
	dsp.zconst15 = (int) 0;
	dsp.joy6_x1 = (int) 0;
	dsp.joy6_y1 = (int) 0;
	dsp.joy6_x2 = (int) 0;
	dsp.joy6_y2 = (int) 0;
	dsp.joy6_x3 = (int) 0;
	dsp.joy6_y3 = (int) 0;
	dsp.joy6_B2 = (int) 0;
	dsp.joy6_B1 = (int) 0;

	// we MAY need to calculate a checksum if they decide that the DS packet needs one
	// (like it did in the previous) - OR if we decide to support the old protocol on WindRiver

	return &dsp;
}

//
// flattenDSPacket() - take the DS packet structure and flatten to a array of bytes for transmission
//		       Returns the number of bytes in there.
//
int flattenDSPacket(unsigned char *buffer,dsPacket *dsp)
{
	int i = 0;

	buffer[i++] = (unsigned char) dsp->indexm;
	buffer[i++] = (unsigned char) dsp->indexl;
	buffer[i++] = (unsigned char) dsp->zconst1;
	buffer[i++] = (unsigned char) dsp->control;
	buffer[i++] = (unsigned char) dsp->zconst2;
	buffer[i++] = (unsigned char) dsp->posally;
	buffer[i++] = (unsigned char) dsp->nconst3;
	buffer[i++] = (unsigned char) dsp->zconst4;
	buffer[i++] = (unsigned char) dsp->joy1_x1;
	buffer[i++] = (unsigned char) dsp->joy1_y1;
	buffer[i++] = (unsigned char) dsp->joy1_x2;
	buffer[i++] = (unsigned char) dsp->joy1_y2;
	buffer[i++] = (unsigned char) dsp->joy1_x3;
	buffer[i++] = (unsigned char) dsp->joy1_y3;
	buffer[i++] = (unsigned char) dsp->joy1_B2;
	buffer[i++] = (unsigned char) dsp->joy1_B1;
	buffer[i++] = (unsigned char) dsp->nconst6;
	buffer[i++] = (unsigned char) dsp->zconst7;
	buffer[i++] = (unsigned char) dsp->joy2_x1;
	buffer[i++] = (unsigned char) dsp->joy2_y1;
	buffer[i++] = (unsigned char) dsp->joy2_x2;
	buffer[i++] = (unsigned char) dsp->joy2_y2;
	buffer[i++] = (unsigned char) dsp->joy2_x3;
	buffer[i++] = (unsigned char) dsp->joy2_y3;
	buffer[i++] = (unsigned char) dsp->joy2_B2;
	buffer[i++] = (unsigned char) dsp->joy2_B1;
	buffer[i++] = (unsigned char) dsp->nconst8;
	buffer[i++] = (unsigned char) dsp->zconst9;
	buffer[i++] = (unsigned char) dsp->joy3_x1;
	buffer[i++] = (unsigned char) dsp->joy3_y1;
	buffer[i++] = (unsigned char) dsp->joy3_x2;
	buffer[i++] = (unsigned char) dsp->joy3_y2;
	buffer[i++] = (unsigned char) dsp->joy3_x3;
	buffer[i++] = (unsigned char) dsp->joy3_y3;
	buffer[i++] = (unsigned char) dsp->joy3_B2;
	buffer[i++] = (unsigned char) dsp->joy3_B1;
	buffer[i++] = (unsigned char) dsp->nconst10;
	buffer[i++] = (unsigned char) dsp->zconst11;
	buffer[i++] = (unsigned char) dsp->joy4_x1;
	buffer[i++] = (unsigned char) dsp->joy4_y1;
	buffer[i++] = (unsigned char) dsp->joy4_x2;
	buffer[i++] = (unsigned char) dsp->joy4_y2;
	buffer[i++] = (unsigned char) dsp->joy4_x3;
	buffer[i++] = (unsigned char) dsp->joy4_y3;
	buffer[i++] = (unsigned char) dsp->joy4_B2;
	buffer[i++] = (unsigned char) dsp->joy4_B1;
	buffer[i++] = (unsigned char) dsp->nconst12;
	buffer[i++] = (unsigned char) dsp->zconst13;
	buffer[i++] = (unsigned char) dsp->joy5_x1;
	buffer[i++] = (unsigned char) dsp->joy5_y1;
	buffer[i++] = (unsigned char) dsp->joy5_x2;
	buffer[i++] = (unsigned char) dsp->joy5_y2;
	buffer[i++] = (unsigned char) dsp->joy5_x3;
	buffer[i++] = (unsigned char) dsp->joy5_y3;
	buffer[i++] = (unsigned char) dsp->joy5_B2;
	buffer[i++] = (unsigned char) dsp->joy5_B1;
	buffer[i++] = (unsigned char) dsp->nconst14;
	buffer[i++] = (unsigned char) dsp->zconst15;
	buffer[i++] = (unsigned char) dsp->joy6_x1;
	buffer[i++] = (unsigned char) dsp->joy6_y1;
	buffer[i++] = (unsigned char) dsp->joy6_x2;
	buffer[i++] = (unsigned char) dsp->joy6_y2;
	buffer[i++] = (unsigned char) dsp->joy6_x3;
	buffer[i++] = (unsigned char) dsp->joy6_y3;
	buffer[i++] = (unsigned char) dsp->joy6_B2;
	buffer[i++] = (unsigned char) dsp->joy6_B1;

	return i;
}


// sendPacket() - given the open socket, send a packet to it
void sendPacket(int sd, struct hostent *hp, dsPacket *dsp)
{
	unsigned char buffer[1024];
	int		theSize;
	int		port = 1110;
	

	struct sockaddr_in	dest_addr;

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	memcpy((void *)&dest_addr.sin_addr, hp->h_addr_list[0], hp->h_length);

	theSize = flattenDSPacket(buffer,dsp);

	if(sendto(sd, (void *)buffer, (size_t)theSize, 0, (struct sockaddr *)&dest_addr,sizeof(dest_addr)) < 0) {
		errorMsg("Yikes");
	} else {
		printf(".");
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
// openSocket() - returns an open socket for sending networking packets to the loopback address (127.0.0.1)
//			Code taken from http://www.thegeekstuff.com/2011/12/c-socket-programming/
//
int openSocket(char *address, int port)
{
	int fd;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		errorMsg("cannot create socket");
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
	char *ports[2] = {"/dev/ttyUSB0","/dev/ttyUSB1"}; // TODO - add S1???
	int i;
	int fd;
	struct termios t;
	
	t.c_iflag = IGNBRK | IGNPAR;
	t.c_cflag = CS8 | CREAD | CLOCAL | B38400;

	while (1){
		for (i = 0; i < 2; i++){
			// check if ttyUSB0 or 1 is available
			// TODO - check if it has FirePlug connected
			if (stat(ports[i], &buf) == 0){
				debug_string("opened port:", ports[i]);
				fd = open(ports[i], O_RDONLY);				
				tcsetattr(fd, TCSANOW, &t);
				return fd;
			}			
		}
		// loop every x seconds, waiting patiently
		sleep(10);
	}
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
        /* Daemon-specific initialization goes here */
#ifndef DEBUG
	pid = daemonize();
#endif

	struct hostent *hp = findMe();

	// get a socket connection to the roboRIO process that is
	//   listening for driver's station packets

	char *address = "127.0.0.1";
	int  port = 1110;
	int  sd;

	sd = openSocket(address,port);

	// while (!present) {
	// check for ttyUSB0 (or 1)
        // check for the FirePlug
	// }
	// open it for reading
        /* The Big Loop */
        while (1) {
		int fd = -1;
		while (fd == -1){
			fd = openUSBPort();
			debug_int("fd thingy: ", fd);	
		}
		while (1){
			chapRPacket *cp;
			dsPacket *dsp;
			/* Do some task here ... */
			// check for ERROR, break and close if possible
			// translate ChapR packet
			cp = readChapRPacket(fd);
			if (cp == NULL){
			  break; // the USB port was closed somehow
			}		
			dsp = translateChapRPacket(cp);
			if (dsp != NULL){
				sendPacket(sd,hp,dsp);
			}
		}	
		close(fd);
		debug_int("just closed: ", fd);
		sleep(10);
	}

	exit(EXIT_SUCCESS);
}
