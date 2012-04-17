#! /bin/sh
### BEGIN INIT INFO
# Provides:		swrodfish
# Required-Start:	$syslog
# Required-Stop:	$syslog
# Should-Start:		$local_fs
# Should-Stop:		$local_fs
# Default-Start:	2 3 4 5
# Default-Stop:		0 1 6
# Short-Description:	Start swordfish daemon
# Description:		Start up swordfish, a high-performance memory caching daemon :)
### END INIT INFO


PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
DAEMON=/usr/sbin/swordfish
NAME=swordfish-testfiller
DESC=swordfish-testfiller
PIDFILE=/var/run/$NAME.pid
ENABLE_SWORDFISH=yes
CONFIG=/etc/swordfish/config.example.ini
USER=nobody

test -x $DAEMON || exit 0

set -e


case "$1" in
  start)
	echo -n "Starting $DESC: "
  if [ $ENABLE_SWORDFISH = yes ]; then
	start-stop-daemon --start --chuid $USER --make-pidfile --pidfile $PIDFILE --background --exec $DAEMON -- -c $CONFIG
	echo "$NAME."
	else
		echo "$NAME disabled"
	fi
	;;
  stop)
	echo -n "Stopping $DESC: "
	start-stop-daemon --stop  --oknodo --pidfile $PIDFILE --signal QUIT
	echo "$NAME."
	rm -f $PIDFILE
	;;
  status)
	status_of_proc $DAEMON $NAME
	;;
  *)
	N=$0
	echo "Usage: $N {start|stop|restart|force-reload|status}" >&2
	exit 1
	;;
esac

exit 0
