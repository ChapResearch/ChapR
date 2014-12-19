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

#define CHAPR_PCKT_SIZE 32
#define DEBUG

typedef struct chapRPacket{
	int digitalIn;
	int x1;
	int y1;
// TODO - add the rest
} chapRPacket;

typedef struct dsPacket{
	int x1;
	int y1;
// TODO - add the rest
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
				cp.digitalIn = (int) buf[0];
				// TODO - finish these
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
	dsp.x1 = cp->x1;
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
