#!/bin/bash

echo "Compile Start..."
if [ -f "./x86_business.o" ]; then
	sudo rm ./x86_business.o
fi

echo "arm-linux-gnueabihf-gcc *.cpp -lpthread -lm -o x86_business.o"
# g++ *.cpp ../lib/cJSON/*.c -lpthread -lm -o x86_business.o.o
g++ *.cpp ./lib/JSON/*.cpp ./lib/JSON/*.c -lpthread -std=c++11 -lm -o x86_business.o
if [ -f "./x86_business.o" ]; then
	echo "Compile Success with x86_business.o"
else
	echo "Mian program compilation Failed!!!"
fi