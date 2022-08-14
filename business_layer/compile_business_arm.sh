#!/bin/bash

echo "Compile Start..."
if [ -f "./arm_business.o" ]; then
	sudo rm ./arm_business.o
fi
if [ -f "/home/lenny/nfs/arm_business.o" ]; then
	sudo rm /home/lenny/nfs/arm_business.o
fi

echo "arm-linux-gnueabihf-g++ *.cpp -lpthread -lm -o arm_business.o"
aarch64-linux-gnu-g++ *.cpp ./lib/JSON/*.cpp ./lib/JSON/*.c -lpthread -std=c++11 -lm -o arm_business.o
if [ -f "./arm_business.o" ]; then
	echo "Compile Success with arm_business.o"
	sudo cp arm_business.o /home/lenny/nfs
else
	echo "Mian program compilation Failed!!!"
fi