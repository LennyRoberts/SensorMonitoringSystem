#include "execute.h"

/*控制器开关:控制器名称(十六进制短整型)，开关选项(CTRL_TURN_ON/CTRL_CUT_OFF)*/
bool RelayControl(unit_16 ctrl_ith, char option)
{
	unit_8 str_cmd[64];
	memset(str_cmd, 0, 64);
	bool result = FALSE;
	ctrl *ptr_ctrl = DeviceParaPtr()->ctrl_head;
	while(ptr_ctrl != NULL){
		if(ptr_ctrl->ctrl_ON == ctrl_ith){
			if(option == ON){                               
				if(ptr_ctrl->fd_port != INVALID_FD)
					write(ptr_ctrl->fd_port, ptr_ctrl->open_order, ptr_ctrl->len_open_order);   //控制器开
				else return FALSE;
			}
			else if(option == OFF){                                                     
				if(ptr_ctrl->fd_port != INVALID_FD)
					write(ptr_ctrl->fd_port, ptr_ctrl->close_order, ptr_ctrl->len_close_order); //控制器关
				else return FALSE;
			}
		}
		ptr_ctrl = ptr_ctrl->next;
	}
	return TRUE;
}

/*控制器组与继电器组初始化*/
bool ControllerInit()
{
	bool result = FALSE;
	ctrl *ptr_ctrl = DeviceParaPtr()->ctrl_head;
	while(ptr_ctrl != NULL){
		if (ptr_ctrl->initial_state == OFF)
			result = RelayControl(ptr_ctrl->ctrl_ON, OFF);
		else if (ptr_ctrl->initial_state == ON)
			result = RelayControl(ptr_ctrl->ctrl_ON, ON);
		sleep(1);
		ptr_ctrl = ptr_ctrl->next;
	}
	return result;
}

/*设备系统功能的实现与处理程序初始化*/
void ExecuteInit( )
{
	printStr("Function Realization and Processing Initializing...");
	bool result = ControllerInit();
	if(result == FALSE)
		printf("Controller Init failed!!!\n");
	else
		printStr("Function Realization and Processing Initialize Finish!\n");
}

//传感器应急保护机制执行程序
void SensorEmergencyExecution( )
{

}

//传感器超时过载保护执行程序
void OverTimeOverLoadExecution( )
{

}

//传感器自校准保护执行程序
void SelfCalibrationExecution( )
{

}

//设备管路清理保护执行程序
void PipeCleaningExecution()
{

}

//联动功能执行程序
void ContactInteractionExecution( )
{

}

/*设备系统功能的处理响应函数*/
void ProcessingResponse( )
{

}

/*传感器自校准保护程序*/
void SelfCalibrationProtection( )
{

}

/*设备管路清理保护程序*/
void PipeLineCleaning( )
{

}

/*设备联动程序*/
void contackInteraction( )
{

}

/*设备功能执行函数入口*/
bool FuncCtrEntry(unsigned short addr1, unsigned short addr2, float data, int fd_sock)
{
	
}






