#include <termios.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <stdio.h>
#include <sys/mount.h>

void debug_string(char *msg, char *arg)
{
  printf("DEBUG: %s%s\n", msg, arg);
}

void debug_int(char *msg, int arg)
{
  printf("DEBUG: %s%i\n", msg, arg);
}

int openUSBStick()
{
  char *ports[2] = {"/dev/sda1","/dev/sda2"}; 
  int i;
  for (i = 0; i < 2; i++){
    if (access("/dev/sda1", F_OK) == 0){
      mount("/dev/sda1", "/tmp/USB", "vfat", 0, 0);
      printf("Found USB stick.\n");
      fflush(stdout);
      return 1;
    }
  }
  return 0;
}

void writeTargetID(char *btAddr)
{
  printf("Checking for USB stick...\n");
  fflush(stdout); // make sure the message is printed immediately

  // create a directory to mount the USB to
  mkdir("/tmp/USB");

  while(1){
    if (openUSBStick() != 0){
      break;
    }
    sleep(3);
    printf("Please enter a USB stick.\n");
  }

  FILE *fd = fopen("/tmp/USB/targetID.txt", "w+");
  fprintf(fd, "%s", btAddr);
  fclose(fd);
  printf("Bluetooth address written.\n");
}

int setBaud(int fd, char *baudRate)
{
  char *buf;
  int length;

  if (baudRate == "38400"){
    buf = "AT configUART 38400\r";
    length = 20;
  } else if (baudRate == "115200"){
    buf = "AT configUART 115200\r";
    length = 21;
  }    

  sleep(1);

  tcflush(fd, TCIFLUSH);
  int bytesWritten = write(fd, buf, length);

  sleep(1);

  char readBuf[15];

  read(fd, &readBuf, 15);

  if (strcmp(readBuf, "-> ConfigUartOk") == 0){
    return 0; // baud was set successfully
  }

  return 1;
}

int getBtAddr(int fd, char *btAddr)
{
  char readBuf[50];

  sleep(1);

  tcflush(fd, TCIFLUSH);
  int bytesWritten = write(fd, "AT BTAddr\r", 10);

  sleep(1);

  read(fd, &readBuf, 50);

  int index;
  for (index = 3; index < 20; index++){
    *btAddr = readBuf[index];
    btAddr++;
  }

  return 0;
}

/* openUSBPort() - opens the appropriate USB port (the one with the KC4134). Returns an open file descriptor (read/write) for the USB port. Blocks while waiting for an appropriate USB port.
*/
int openUSBPort(int baudRate)
{
  struct stat buf;
  char *ports[2] = {"/dev/ttyUSB0","/dev/ttyUSB1"}; 
  int i;
  int fd;
  struct termios t;
	
  t.c_iflag = IGNBRK | IGNPAR;
  if (baudRate == 115200){
    t.c_cflag = CS8 | CREAD | CLOCAL | B115200;
  } else if (baudRate == 38400){
    t.c_cflag = CS8 | CREAD | CLOCAL | B38400;
  }

  while (1){
    for (i = 0; i < 2; i++){
      // check if ttyUSB0 or 1 is available
      if (stat(ports[i], &buf) == 0){
	fd = open(ports[i], O_RDWR, O_NOCTTY);				
	tcsetattr(fd, TCSANOW, &t);
	printf("Opened KC4134.\n");
	return fd;
      }			
    }
    // loop every x seconds, waiting patiently
    printf("Checking for KC4134...\n");
    sleep(5);
  }
}

int main(void)
{
  printf("This program will walk you through pairing your ChapR to your Bluetooth dongle. You will need a powered roboRIO and ChapR, as well as a flash drive and the KC4134 Bluetooth USB Serial Adapter.\n");
  fflush(stdout); // make sure the message is printed immediately
  sleep(2);

  // stop the ChapR daemon (if running)
  system("/etc/init.d/chaprd.sh stop");

  printf("Please plug in the KC4134 Bluetooth USB Serial Adapter\n");

  // begin communications w/ KC4134
  int fd;

  fd = openUSBPort(115200);
  printf("Trying 38400 baud....\n");
  setBaud(fd, "38400");
  sleep(1);
  fd = openUSBPort(38400);
  sleep(1);
  if (setBaud(fd, "38400") != 0){
    printf("Error setting baud to 38400 (may already be set).\n");
  }

  sleep(1);

  char btAddr[12]; // Bluetooth addresses are defined to be 12 characters long
  // get the BT address
  getBtAddr(fd, btAddr);
  printf("Bluetooth Address: %s", btAddr);
  fflush(stdout);


  printf("Please plug in a flash drive (the KC4134 can be removed if necessary).\n");
  // create a file on the flash drive w/ BT address
  writeTargetID(btAddr);

  sleep(2); // give user time to read messages

  printf("Now remove the USB stick from the roboRIO and insert it into the second USB port of the ChapR (be sure the ChapR is on and has sufficient battery.)\n");
  printf("The ChapR should make a confirm tone. If not, you are using the wrong USB port.\n");
  printf("Now reboot the ChapR and the roboRIO, plugging the KC4134 back in if necessary.\n");
  printf("The ChapR should now connect automatically. You will hear a noise, and the blue light on the ChapR will turn solid after several seconds.\n");
  printf("If this pairing process was unsuccessful, please visit our website, www.TheChapR.com for troubleshooting tips.\n");
}
