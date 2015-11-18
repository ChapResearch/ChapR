#
# Makefile for chaprd deamon
#
#  These targets are supported:
#	clean - cleans up temporary files
#	cdebug - creates a debugging version of chaprd, which is named "cdebug"
#	chaprd - creates the REAL version of the chaprd
#	install - creates the installation file
#

#
# Standard C settings and flags that we are using
#
CC=arm-frc-linux-gnueabi-gcc
STRIP=arm-frc-linux-gnueabi-strip
LIBS=-lrt
#LIBS=/usr/arm-frc-linux-gnueabi/usr/lib/librt.so

all: install_chaprd

chaprd: chaprd.c
	$(CC) -o chaprd $(LIBS) chaprd.c

chaprd.stripped: chaprd
	cp -f chaprd chaprd.stripped
	$(STRIP) chaprd.stripped

cdebug: chaprd.c
	$(CC) -DDEBUG -o cdebug $(LIBS) chaprd.c

readUSB: read_USB.c
	$(CC) -o readUSB read_USB.c

resetUSB: reset_USB.c
	$(CC) -o resetUSB reset_USB.c

timerTest: timer_test.c
	$(CC) -o timerTest $(LIBS) timer_test.c

install_chaprd:	inst_template.sh chaprd.sh.uu chaprd.uu
	sed -e '1,/^---- cut here ----/d' \
	    -e '/\[CHAPRBINARY\]/ r chaprd.uu' \
	    -e '/\[CHAPRINIT\]/ r chaprd.sh.uu' \
	    -e '/\[CHAPRBINARY\]/d' \
	    -e '/\[CHAPRINIT\]/d' < inst_template.sh > install_chaprd

chaprd.uu: chaprd.stripped
	uuencode chaprd < chaprd.stripped > chaprd.uu

chaprd.sh.uu: chaprd.sh
	uuencode chaprd.sh < chaprd.sh > chaprd.sh.uu

chaprd-magic: chaprd-transfer chaprd-remotestart

chaprd-transfer: chaprd chaprd-remotekill
	sshpass -p "" scp chaprd admin@172.22.11.2:/tmp

chaprd-remotestart: 
	sshpass -p "" ssh admin@172.22.11.2 "cd /tmp; ./chaprd"

chaprd-remotekill: 
	-sshpass -p "" ssh admin@172.22.11.2 killall chaprd

cdebug-transfer: cdebug
	sshpass -p "" scp cdebug admin@172.22.11.2:/tmp

readUSB-transfer: readUSB
	sshpass -p "" scp readUSB admin@172.22.11.2:/tmp

resetUSB-transfer: resetUSB
	sshpass -p "" scp resetUSB admin@172.22.11.2:/tmp

timerTest-transfer: timerTest
	sshpass -p "" scp timerTest admin@172.22.11.2:/tmp

clean:
	rm -f chaprd cdebug install_chaprd chaprd.uu chaprd.sh.uu chaprd.stripped install_chaprd

realclean: clean
	rm -f *~
