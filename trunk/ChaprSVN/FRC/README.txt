------------ NEW INFORMATION --------------

A new Makefile has been created that will build the chaprd for you!  It has a few cool
targets you can use:

	$ make cdebug

This command will build a debug version of the chaprd.  It does this by setting the
DEBUG definition for you.

	$ make chaprd

This command will build a daemonized version of the chaprd.  Note that it will still
have the debugging symbols in it for your convenience.

	$ make install_chaprd

This is a wonderful command.  It will create an e-mailable install file that can be
loaded to a roboRIO, and run, automagically installing the chaprd and setting up the
init files.  It strips the chaprd before creating the file so it is as small as
possible.  On the roboRIO you simple do "sh ./install_chaprd" and it does it for you.

Note that if you just do:

	$ make

that the default is to create the install_chaprd file.

One last thing, you really should do a:

	$ make clean

before you do a bunch of anything else.  It will remove all of the files that are
automagically generated.

For your convenience, you can also do:

	$ make realclean

This will do a clean, plus it will remove the backup emacs files that tend to pepper
a directory.


------------ OLD INFORMATION ------------
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
