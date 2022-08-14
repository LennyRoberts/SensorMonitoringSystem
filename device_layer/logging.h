#ifndef LOGGING_H
#define LOGGING_H
/*=======================================================================
* 多线程异步日志系统：双缓冲机制
* 1.记录下设备各项系统&设备状态，记录操作记录
* 2.读取日志记录并进行格式转换，以便于显示给用户
* 3.前台线程将运行时产生的日志信息存入buff_front
* 4.后台线程将buff_back中的日志信息写入日志文件中
* 5.控制机制：
*	当buff_front存满后唤醒后台线程，然后与buff_back交换(因为后台线程
*	在睡眠前就将buff_back中的内容写入日志文件，所以此时的buff_back是空的),
*	后台线程将交换后buff_back内容写入到日志文件中，然后休眠。
*	若长时间buff_front未写满则设定超时机制，例如每3秒无论是否写满
*	都要进行唤醒后台线程。交换并写入日志文件。
* ======================================================================*/
#include "device_info.h"

#define LOGBUFF_SIZE    256

/*-------------------------------------------------------------------
日志文件系统中使用8Byte double型出错码进行记录
出错码：0x00 00 00 00 00 00 00 00
码结构：0x00 00        0x00 00       0x00 00 00 00
      2Byte位置分类 + 2Byte具体位置 + 4Byte出错信息
存储格式(32字节)： 时间 + 出错码 = "HH:mm:ss:mms:0x0000000000000000;"
存储与释放规则：每月日志为一个文件夹，每天日志为一个文件，
            每月最后一天自动删除上一个月的日志文件，仅保留近一个月的日志
---------------------------------------------------------------------*/

typedef struct{
    unit_8 buff[LOGBUFF_SIZE];
    int size;
    int flag;
}loggingBuff;

extern loggingBuff *buff_1;
extern loggingBuff *buff_2;
extern loggingBuff *pull_buff;
extern loggingBuff *push_buff;

void LoggingManage();
void InitLogging();
void WriteLogging(struct timespec out_time, int location, int err_code);	/*写入日志记录*/
void ReadLogging();					            /*读取日志记录*/
int IsFullLogBuff();
int WriteInLoggingFile();
int ExchangeBuff(loggingBuff* push);

extern void printStr(char *str);
#endif // !LOGGING_H