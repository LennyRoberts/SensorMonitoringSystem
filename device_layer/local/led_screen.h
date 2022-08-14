#ifndef LED_SCREEN_H
#define LED_SCREEN_H
/*==================================================================
* LED屏显示管理程序：主要完成对LED显示样式的串口配置以及数据显示
* ==================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "local_info.h"

/*LED控制器信息*/
typedef struct ledController{
	
	unsigned int	turn_page_time;                    //换页时间间隔
	unsigned int	updata_time;                       //数据更新时间间隔
}led_controller;

extern led_controller *led_controller_ptr;

void LedScreenInitialize( );                          /*LED屏初始化程序*/
void LED_ScreenDataReceiver( );                       /*LED屏数据接收*/
bool LEDControllerInitialize(led_controller *led);    /*LED屏控制器信息初始化*/
led_controller* LedContronllerPtr();

#endif // !LED_SCREEN_H