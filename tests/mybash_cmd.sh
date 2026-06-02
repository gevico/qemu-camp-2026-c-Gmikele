#!/bin/bash
# Test commands for mybash - writes a temp script then runs mybash on it
cd /home/ruilun_yang/QEMU_C/qemu-camp-2026-c-Gmikele/exercises/20_mybash

# Create a temporary script that mybash will execute
cat > /tmp/mybash_test_script.txt << 'SCRIPT'
myfile bin/mybash
myfile obj/myfile/myfile.o
mysed s/unix/linux/ "unix is opensource. unix is free os."
mytrans src/mytrans/text.txt
mywc src/mytrans/text.txt
SCRIPT

./bin/mybash /tmp/mybash_test_script.txt
