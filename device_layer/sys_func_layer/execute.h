#ifndef REALIZATION_H
#define REALIZATION_H
/*===============================================================================
* 设备系统功能实现程序：
* 1.对指令解析器发来的指令进行处理并调用执行程序执行
* 2.对依据数据的设备系统功能控制器控制的功能进行反应、处理、并调用处理程序执行
* ==============================================================================*/
#include "../device_info.h"

/*继电器组信息*/
typedef struct GPIORelay{
    unit_8 relay_one[64];
    unit_8 relay_two[64];
    unit_8 relay_three[64];
    unit_8 relay_four[64];
}gpio_relay;


void ExecuteInit( );                              /*设备系统功能的实现与处理程序初始化*/
bool ControllerInit();                            /*控制器初始化*/
bool RelayControl(unit_16 ctrl_ith, char option);   /*继电器开关控制器*/
void SensorEmergencyExecution();                  /*传感器应急保护机制执行程序*/
void OverTimeOverLoadExecution( );                /*传感器超时过载保护执行程序*/
void SelfCalibrationExecution( );                 /*传感器自校准保护执行程序*/
void PipeCleaningExecution( );                    /*设备管路清理保护执行程序*/
void ContactInteractionExecution( );              /*联动功能执行程序*/
void ProcessingResponse( );                       /*设备系统功能的处理响应函数*/
void SelfCalibrationProtection( );                /*传感器自校准保护程序*/
void PipeLineCleaning( );                         /*设备管路清理保护程序*/
void contackInteraction( );                       /*设备联动程序*/
bool FuncCtrEntry(unsigned short addr1, unsigned short addr2, float data, int fd_sock); /*设备功能执行函数入口*/

/*外部函数*/
extern void printStr(char *str);
extern bool commandExecute(char *cmd, char *isRead);       /*终端指令执行函数:指令cmd + 标志位 >> 指令写入终端*/
extern void TurnOnRelayCmd(unit_8 **cmd_str, int sel);     /*获取开启继电器的指令*/
extern void CutOffRelayCmd(unit_8 **cmd_str, int sel);     /*获取切断继电器的指令*/
#endif // !REALIZATION_H