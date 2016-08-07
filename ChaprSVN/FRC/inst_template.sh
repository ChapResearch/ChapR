THIS FILE IS A TEMPLATE!  It isn't meant to be run.  It has to be
processed by the Makefile to turn it into an executable script that
can be sent through e-mail.  Once the real file has been composed,
this text won't be here.  In fact, all text up to the first line
that starts with "---- cut here ----" will be deleted.  Because that
previous line didn't START with "---- cut here ----", it won't be
the trigger for the end of deleting. Same with the previous line. :-) 
---- cut here ----
#!/bin/sh
#
#  Install the ChapR Deamon.
#
#  This file was built automagically from a template.
#
#  This file must be run on the roboRIO as the "admin" user.
#

cd /tmp

echo "Creating chaprd binary program..."
uudecode <<""EOF
[CHAPRBINARY]
EOF

echo "Creating chaprd init script..."
uudecode <<""EOF
[CHAPRINIT]
EOF

echo "Moving files to correct locations..."
mv chaprd /usr/sbin
mv chaprd.sh /etc/init.d

echo "Ensuring correct permissions..."
chown admin.administrators /usr/sbin/chaprd /etc/init.d/chaprd.sh
chmod 750 /usr/sbin/chaprd /etc/init.d/chaprd.sh

echo "Creating init links..."
ln -s /etc/init.d/chaprd.sh /etc/rc5.d/S99chaprd

cd /home/admin

echo "Unpacking pairing utility into /home/admin..."
uudecode <<""EOF
[PAIRINGUTIL]
EOF

echo "Running pairing utility..."
chmod 744 configUSB
./configUSB
