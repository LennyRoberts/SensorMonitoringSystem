#ifndef SFL_MANAGE_H
#define SFL_MANAGE_H
/*==============================================================================
* 系统功能层的管理程序，主程序通过管理该管理程序从而实现对系统功能层的管理
* =============================================================================*/
#include <strings.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>
#include <signal.h>
#include <time.h>

#include "../logging.h"
#include "../device_info.h"
#include "optimize.h"
#include "execute.h"
#include "command.h"

#define	IPC_BUSINESS   "business.sock"
#define	IPC_DEVICE     "device.sock"

/*外部变量*/
extern pthread_cond_t pthread_cond_PL;   //感知层线程
extern pthread_mutex_t lock_PL;
extern bool wait_PL_status;
extern pthread_cond_t pthread_cond_IPC1; //进程间通信1线程
extern pthread_mutex_t lock_IPC1;
extern bool wait_IPC1_status;
extern pthread_cond_t pthread_cond_AL;   //系统层数据处理线程
extern pthread_mutex_t lock_AL;
extern bool wait_AL_status;
extern pthread_cond_t pthread_cond_ISPC; //串口间通信线程
extern pthread_mutex_t lock_ISPC;
extern bool wait_ISPC_status;

void *IPCOne(void *arg);		                            /*系统层管理程序*/
void* ISPCManage(void *arg);                                /*串口间通信*/
void *AlgorithmManage(void *arg);			                /*数据优化算法管理程序*/
int  UnixSockeIPCInit();                                    /*Unix本地套接字IPC初始化(作为客户端)*/
int  UnixSockServerInit();                                  /*Unix本地套接字IPC初始化(作为服务端)*/
int  UnixSockAccept(int lfd);                               /*接受客户端连接请求*/
bool ExchangeBuffer(dual_buff *buff);                       /*交换双缓存*/
int  UploadToSevice(int fd, char buf[]);                    /*上传至服务层*/
char *DownloadFromService(int fd, char *buf, int recv_len); /*接收服务层下发的指令或数据*/
int  UplaodModbusDataToPort();                              /*数据发送到指定串口*/

/*外部函数*/
extern void printStr(char *str);
extern void FlushFdInOut(int fd);
extern device_para* DeviceParaPtr();                        /*设备信息结构体*/
extern sensor_type* TypeHead(sensor_type** type);
extern sensor_type* TypeNext(sensor_type **type);
extern int  IsEmptyType(sensor_type *type);
extern int  IsWait(bool wait_status);
extern void DataPacketTime(unit_8 *data_pack);
extern void DataItemHandling(sensor_item *item_head, float *buff);
extern int  JsonDataPackage(time_t *m_time);                 /*将测量处理后的数据封装成自定义CJSON数据包*/
extern int  ModbusPacket();                                  /*modbus协议指令数据封装*/
extern int  GetModbusData(char *str);                        /*获取封装的modbus数据*/
serial_port **SerialPort();
extern sensor_item *ItemHead(sensor_type *type);
extern int CmdHeadCheak(unit_8 *cmd_buff);
extern int ExchangeOverTime();
extern dual_buff* BuffPtrOne();
extern dual_buff* BuffPtrTwo();
extern dual_buff* BuffPush();
extern dual_buff* BuffPull();
extern void ChangeBuffPush(dual_buff *ptr_src);
extern void ChangeBuffPull(dual_buff *ptr_src);
extern unit_16 CmdManage(unit_8 *cmd, int fd, char type);
extern void InitResultBuff();                               /*重置最终值函数*/
extern int  RecvCmd(int sock_fd, unit_8 *buf, int len);     /*接收服务层unix socket客户端数据*/
extern bool DeleteCmdCache( );                              /*销毁指令缓存*/
extern bool CMDManageInit();                                /*指令管理系统初始化*/
extern void ExecuteInit( );                                 /*设备系统功能的实现与处理程序初始化*/
extern void InitOptimize();                                 /*算法优化程序初始化*/
#endif // !SFL_MANAGE_H
