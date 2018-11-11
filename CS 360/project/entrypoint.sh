echo "*** STARTING DB ***"
/usr/bin/mysqld_safe &
sleep 3

# import database
echo "*** IMPORTING DB ***"
mysql < /tmp/setup.sql
mysql < /tmp/tables.sql
mysql < /tmp/data.sql

echo "*** KILLING DB **"
kill $!

# finally run the supervisord
echo "*** STARTING supervisord ***"
/usr/bin/supervisord -n -c /etc/supervisord.conf
