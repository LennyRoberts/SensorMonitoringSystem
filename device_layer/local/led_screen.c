#include "led_screen.h"

led_controller *led_controller_ptr;

/*LED屏初始化程序*/
void LedScreenInitialize( )
{

}

/*LED屏控制器信息初始化*/
bool LEDControllerInitialize(led_controller *led)
{
	if(led == NULL)
		return FALSE;
	led_controller *ptr_led = (led_controller *)malloc(sizeof(led_controller));
	if(ptr_led == NULL){
		printf("Init Para LED Screen malloc failed!!!\n");
		return FALSE;
	}
	ptr_led->turn_page_time = led->turn_page_time;
	ptr_led->updata_time = led->updata_time;
	led_controller_ptr = ptr_led;
}

led_controller* LedContronllerPtr(){
	return led_controller_ptr;
}

/*LED屏数据接收器*/
void LED_ScreenDataReceiver( )
{

}