To compile the program:

   arm-frc-linux-gnueabi-gcc -o chaprd chaprd.c

This is the cross-compilation command (gcc) that has everything
configured to use the right libraries for compiling the chaprd.c
program for the roborio.

To transfer to the roborio:

   sshpass -p "" scp chaprd admin@169.254.105.70:/tmp

This uses ssh to transfer the chaprd program to the /tmp directory
of the roborio identified by the (ip address).  The "admin@" part
says "run ssh as the user admin" - you'll be asked for the password.
