#!/bin/bash

echo "Compile Start..."
if [ -f "./x86_device.o" ]; then
	sudo rm x86_device.o
fi

echo "Main program is building..."
gcc *.c ./sys_func_layer/*.c ./sensor_perceive_layer/*.c ./local/*.c \
    ./lib/cJSON/*.c ./lib/crc/*.c ./init/*.c -lpthread -lm -o x86_device.o
#gcc -fPIC *.c ./sys_func_layer/*.c ./sensor_perceive_layer/*.c -lpthread -lm -o target.so -shared

if [ -f "./x86_device.o" ]; then
	echo "Compile Success with x86_device.o"
else
	echo "Compilation Failed!!!"
fi
