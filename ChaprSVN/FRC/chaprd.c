#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <termios.h>

#define CHAPR_PCKT_SIZE 33
#define DEBUG

typedef struct chapRPacket{
  int cmd;
  int digitalIn;
  int zero;
  int analog1; // two bytes
  int joy1_TH;
  int analog2; // two bytes
  int joy1_B1;
  int analog3; // two bytes
  int joy1_B2;
  int analog4; // two bytes
  int zero;
  int joy1_x1;
  int joy1_y1;
  int zero;
  int joy1_x2;
  int joy1_y2;
  int joy2_B1;
  int joy1_x3;
  int joy1_y3;
  int zero;
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
  int index;
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

void debug_string(char *msg, char *arg)
{
#ifdef DEBUG 
	printf("DEBUG: %s%i\n", msg, arg);
#endif
}

void debug_int(char *msg, int arg)
{
#ifdef DEBUG 
	printf("DEBUG: %s%i\n", msg, arg);
#endif
}

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
		int error = read(fd, (void *) &rawData, 1);
		if (error < 0){
			debug_int("read error:", error);
			exit (1);
			return (chapRPacket *) NULL;
		}
		if (error == 0){
			continue;
		}
		debug_int("data:", rawData);
		switch (state){
		case 0:
 		case 1:
		case 2:
			if (rawData == 0xff){
				state++;
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
			  cp.analog1 = (buf[3]<<8)|buf[4]; // TODO
			  cp.joy1_TH = (int) buf[5];
			  cp.analog2 = buf[6] & buf[7]; // TODO
			  cp.joy1_B1 = (int) buf[8];
			  cp.analog3 = buf[9] & buf[10];
			  cp.joy1_B2 = (int) buf[11];
			  cp.analog4 = buf[12] & buf[13];
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
				state = 0;
			}
			break;
		}
	}
}

// 
// translateChapRPacket() - 
//
dsPacket *translateChapRPacket(chapRPacket *cp)
{
	static dsPacket dsp;
	static index = 0;
	dsp.index = index++;
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
	dsp.nconst6 = (int) 0;
	dsp.zconst7 = (int) 9;
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
	return &dsp;
}

void sendPacket(dsPacket *dsp)
{

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
	t.c_cflag = CS8 | CREAD | CLOCAL | B115200;

	while (1){
		for (i = 0; i < 2; i++){
			// check if ttyUSB0 or 1 is available
			// TODO - check if it has FirePlug connected
			if (stat(ports[i], &buf) == 0){
				debug_string("opened port: ", ports[i]);
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
				sendPacket(dsp);
			}
			// serial loopback?
		}	
	}

	exit(EXIT_SUCCESS);
}
