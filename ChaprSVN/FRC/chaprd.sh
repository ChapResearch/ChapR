#!/bin/sh
#
#
### BEGIN INIT INFO
# Provides: chaprd
# Default-Start:  5
# Default-Stop:   0
# Short-Description: run ChapR daemon
# Description: translates ChapR packets (coming in through USB or Serial) to FRC
#              packets (sent by Serial loopback) 
### END INIT INFO

CHAPRD=/usr/sbin/chaprd
#CHAPRD=/usr/sbin/cdebug

[ -x $CHAPRD ] || exit 1

case "$1" in
	start)
		echo -n "Starting chaprd... "
		start-stop-daemon -S -b -n $(basename ${CHAPRD}) -a $CHAPRD
		echo "done."
	;;
	stop)
		echo -n "Stopping chaprd... "
		start-stop-daemon -K -n $(basename ${CHAPRD})
		echo "done."
	;;
	restart)
		echo -n "Restarting chaprd... "
	 	$0 stop
		$0 start
	;;
	*)
	echo "Usage: $0 {start|stop|restart}"
	exit 1
	;;
esac

exit 0
