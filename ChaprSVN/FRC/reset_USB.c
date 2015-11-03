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
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

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

int main(void) {

  openlog("Reset USB", LOG_PID, LOG_DAEMON);
  syslog(LOG_INFO, "reset_USB running");

  const char *filename;

  int fd;

  filename = "/dev/bus/usb/001/008";

  fd = open(filename, O_WRONLY);

  printf("fd: %d\n", fd);

  if (ioctl(fd, USBDEVFS_RESET, 0) < 0){
    printf("didn't work!\n");
    printf("errno: %d\n", errno);
  } else {
    printf("made it\n");
  }

  close(fd);

  return;
}
