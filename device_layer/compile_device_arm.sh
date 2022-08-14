#!/bin/bash

echo "Compile Start..."
if [ -f "./arm_device.o" ]; then
	sudo rm arm_device.o
fi

if [ -f "/home/lenny/nfs/arm_device.o" ]; then
	sudo rm /home/lenny/nfs/arm_device.o
fi

echo "Main program is building..."
aarch64-linux-gnu-gcc-6.3.1 *.c ./sys_func_layer/*.c ./sensor_perceive_layer/*.c ./local/*.c \
                            ./lib/cJSON/*.c ./lib/crc/*.c ./init/*.c -lpthread -lm -o arm_device.o

#封装成动态库.so
#arm-linux-gnueabihf-gcc -fPIC *.c ./sys_func_layer/*.c ./sensor_perceive_layer/*.c -lpthread -lm -o target.so -shared

if [ -f "./arm_device.o" ]; then
	echo "Compile Success with arm_device.o"
	sudo cp arm_device.o /home/lenny/nfs
	sudo cp dev_info.csv /home/lenny/nfs
else
	echo "Main program compilation Failed!!!"
fi