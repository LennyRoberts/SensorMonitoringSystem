#!/bin/bash

echo "Compile Start..."
echo "*========================================*"
if [ -f "./arm_device.o" ]; then
	sudo rm ./arm_device.o
fi
if [ -f "./arm_business.o" ]; then
	sudo rm ./arm_business.o
fi
 
echo "* Main program is building...            *"
aarch64-linux-gnu-gcc-6.3.1 ./device_layer/*.c ./device_layer/sys_func_layer/*.c ./device_layer/sensor_perceive_layer/*.c \
                        ./device_layer/local/*.c ./device_layer/lib/cJSON/*.c ./device_layer/lib/crc/*.c ./device_layer/init/*.c \
						-lpthread -lm -o arm_device.o
if [ -f "./arm_device.o" ]; then
	echo "* Compile Success with arm_device.o      *"
    sudo cp ./arm_device.o /home/lenny/nfs
else
	echo "* Mian program compilation Failed!!!     *"
fi

echo "*----------------------------------------*"
echo "* Business Layer program is building...  *"
aarch64-linux-gnu-g++ ./business_layer/*.cpp ./business_layer/lib/JSON/*.cpp ./business_layer/lib/JSON/*.c \
						-lpthread -std=c++11 -lm -o arm_business.o
if [ -f "./arm_business.o" ];then
	echo "* Compile Success with arm_business.o    *"
	sudo cp ./arm_business.o /home/lenny/nfs
else
	echo "* Business Layer compilation Failed!!!   *"
fi
	sudo cp ./device_layer/*.csv /home/lenny/nfs
	sudo cp ./business_layer/*.csv /home/lenny/nfs
echo "*========================================*"
echo "Compile End!"