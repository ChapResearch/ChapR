#include <signal.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>

void signalHandler(int signal)
{
  syslog(LOG_INFO, "received signal: %d\n", signal);
}

void intHandler(int signal)
{
  syslog(LOG_INFO, "INTERRUPT");
}

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
			if (stat(ports[i], &buf) == 0){
			  syslog(LOG_INFO, "opened port: %s\n",ports[i]);
			  fd = open(ports[i], O_RDONLY);				
			  syslog(LOG_INFO, "opened fd: %d\n",fd);
			  tcsetattr(fd, TCSANOW, &t);
			  return fd;
			}			
		}
		syslog(LOG_INFO, "checked for USB...");
		sleep(10);
	}
}

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

int readData(int fd){

  unsigned char rawData;
  static struct timeval begin, end, diff; // used for timeout of connection
  static struct timespec sleepTime;
  sleepTime.tv_sec = 0;
  sleepTime.tv_nsec = 200000L; // operate at 5K BAUD
  struct timespec timeLeft; // not used, but still needed as a parameter
  static int connected = 0; 
  static int firstZero = 0;

  while (1){
    int rval = read(fd, (void *) &rawData, 1);
    if (rval < 0){
      syslog(LOG_CRIT, "read failed (rval < 0, errno %d)...exiting", errno);
      closelog();
      exit(1);
    } else if (rval == 0){
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
	    return -1;
	  }
	}
      }
    } else {
      printf("VAL: %d\n", rawData);
      firstZero = 0; // looks like there isn't a problem after all
      if (connected == 0){
	syslog(LOG_INFO, "just connected");
	connected = 1;
      }
    }
    nanosleep(&sleepTime, &timeLeft);
  }
}

int main(void) {

  openlog("Read USB", LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO, "readUSB running");


  while(1) {
    int fd = -1;
    while(1){
      fd = -1;
      while (fd == -1){
	fd = openUSBPort();
      }
      int rval = readData(fd);
      if (rval == -1){
	break;
      }
      sleep(1);
    }
    close(fd);
    syslog(LOG_INFO, "just closed port :%d\n", fd);
    sleep(5);
  }

}
