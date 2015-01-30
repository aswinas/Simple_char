# Simple_char
Simple char driver

just go through simple_char.c and load files
Use command line "sh load" to load the driver

use echo "data">/dev/simple_char to write data to driver
dmesg
or
cat filename>/dev/simple_char to wrtite an entire file
dmesg.

dmesg -c to clear printks
to read use cat /dev/simple_char
dmesg
