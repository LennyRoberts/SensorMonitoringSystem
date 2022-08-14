#ifndef SENSOR_PERCEIVE_H
#define SENSOR_PERCEIVE_H
/*========================================
* 传感器数据读写&模块读写及指令处理程序：
* 1.传感器的数据读与指令写
* 2.各模块的数据读与指令写
* 3.测量数据的预处理与比较
* 4.传感器和模块的状态及操作直接通信
* ========================================*/
#include <sys/select.h>

#include "../device_info.h"
#include "../sys_func_layer/execute.h"



#define NUM_FAILURE 2    /*读取串口失败重试次数*/

void SensorHandlingInitialize( );                                                                    /*传感器处理程序初始化*/
void AddtionalModuleInitialize( );                                                                   /*附加模块程序初始化*/
void RW_ControllerInitialization( );                                                                 /*读写指令控制器初始化*/
void Module_RW_Controller( );                                                                        /*附加模块读写控制器*/
int32_t Digital_RW_Controller(sensor_type *type, unit_8 *buff, status_rw rw_status, model_r r_modle);/*数字量读写控制器*/
void DigitalPreprocessing( );                                                                        /*数字量数据预处理*/
void Analog_RW_Controller( );                                                                        /*模拟量读写控制器*/
void AnalogPreprocessing( );                                                                         /*模拟量数据预处理*/
emergency_cmd *EmergencyCmdComparator(sensor_type *type, unit_8 *buff);                              /*传感器及模块应急指令比较器*/
void SensorEmergencyHandling(sensor_type *type, int32_t func_ith);                                   /*传感器应急保护机制*/
void CmdDataParser(dual_buff *push, unit_8 *buff, sensor_type *type, int level);                     /*指令数据解析与储存*/
int  CheckStartFlag(sensor_type *type, unit_8 *buff_replay);                                         /*检查回复指令包头部标志*/
int  GetReplyData(sensor_type *type, unit_8 *buff_reply);                                            /*获取指令回复数据包*/
void HardWareErrorHandling(sensor_type *type);
void DataErrorHandling(sensor_type *type);
void ExceedMaxRangeHandling(sensor_type *type);
float GetValue(unit_8 *buff, sensor_item *ptr_item);

/*外部函数*/
extern void printStr(char *str);
extern void printStrStr(char *str1, char *str2);
extern void printHex(const unsigned char *buf, unit_32 len);
extern void FlushFdIn(int fd);
extern void FlushFdOut(int fd);
extern void FlushFdInOut(int fd);
extern int CycleBuffNum();                                  /*获取双缓存机制传感器存储次数*/
extern sensor_type *TypeLinkHead();
extern sensor_type *TypeNext(sensor_type **type);
extern void CRC_Generator(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len);
extern bool CRC_Check(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len);
extern int CheckFdStatus(int fd, status_rw rw_status, int32_t overtime_ms);                   /*I/O多路复用poll，超时检测当前文件描述符是否准备好*/
extern int32_t WriteCMD(unit_8 *name, int fd, unit_8 *cmd, int len_cmd, unit_16 CRC_module, unit_16 CRC_turn);/*写入读取指令*/
extern int  GetBuffIndex(int buff_ith, int level);                    /*获取数据缓存的索引*/
extern int  ItemDataCode(sensor_item *item);
extern sensor_item *ItemNext(sensor_item **item);
extern unsigned int HexCharToUnsignedLong(unit_8 *buff, char type, unit_16 byte_order); /*无符号char型数据解析*/
extern int HexCharToSignedLong(unit_8 *buff, char type, unit_16 byte_order);            /*有符号char型数据解析*/
extern float HexToFloatPoint(unit_8 *buff);
extern void SensorEmergencyExecution( );                              //传感器应急保护机制执行程序
extern void OverTimeOverLoadExecution( );                             //传感器超时过载保护执行程序
extern void SelfCalibrationExecution( );                              //传感器自校准保护执行程序
extern void PipeCleaningExecution( );                                 //设备管路清理保护执行程序
#endif // !SENSOR_PERCEIVE_H
