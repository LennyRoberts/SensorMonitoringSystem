#!/bin/bash

echo "Compile Start..."
echo "*========================================*"
if [ -f "./x86_device.o" ]; then
	sudo rm ./x86_device.o
fi
if [ -f "./x86_busineaa.o" ]; then
	sudo rm ./x86_business.o
fi
if [ -f "./dev_info.csv" ]; then
	sudo rm ./dev_info.csv
fi
if [ -f "./ser_info.csv" ]; then
	sudo rm ./ser_info.csv
fi

echo "* Main program is building...            *"
gcc ./device_layer/*.c ./device_layer/sys_func_layer/*.c ./device_layer/sensor_perceive_layer/*.c \
	./device_layer/local/*.c ./device_layer/lib/cJSON/*.c ./device_layer/lib/crc/*.c ./device_layer/init/*.c \
	 -lpthread -lm -o x86_device.o
if [ -f "./x86_device.o" ]; then
	echo "* Compile Success with x86_device.o      *"
	sudo cp ./device_layer/dev_info.csv ./
else
	echo "* Mian program compilation Failed!!!     *"
fi

echo "*----------------------------------------*"
echo "* Service program is building...         *"
g++ ./business_layer/*.cpp ./business_layer/lib/JSON/*.cpp ./business_layer/lib/JSON/*.c -lpthread -std=c++11 -lm -o x86_business.o
if [ -f "./x86_business.o" ];then
	echo "* Compile Success with x86_business.o    *"
	sudo cp ./business_layer/ser_info.csv ./
else
	echo "* business Layer compilation Failed!!!   *"
fi

echo "*========================================*"
echo "Compile End!"