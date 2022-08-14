#ifndef INIT_H
#define INIT_H
/*===================================================================
 * 设备初始化:
 * -------------------------------------------------------------------
 * 1.是设备运行的主管理程序（传感器感知层和系统功能层的主管理程序）
 *	组织、协调、调度和沟通两层间的各部分关联
 * 2.在初始化程序中创建线程并进行线程同步机制管理
 * 3.对设备进行初始化操作，导入配置参数，执行自检和测试程序
 * 4.进行Unix域的本地套接字方式的IPC，与应用层服务进行交互
 * 运行参数初始化
 * ------------------------------------------------------------------
 * 1.获取和解析设备配置参数并存储转发
 * 2.获取和解析传感器配置参数并存储转发
 * 3.获取和解析各项读写及执行指令并存储转发
 * 4.对所有配置信息进行存储和转发，使系统设备完成初始化配置操作
 * ==================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "../sensor_perceive_layer/spl_manage.h"
#include "../sys_func_layer/sfl_manage.h"
#include "../sys_func_layer/command.h"
#include "../local/local_manage.h"
#include "../device_info.h"
#include "../logging.h"
#include "self-test.h"

#define DEVICE_INFO_PATH    "./dev_info.csv"     //设备层初始化文件路径
#define INIT_BUFF_SIZE      1024                 //每行读取缓存长度
#define DELIM               ","                  //初始化文件参数分隔符

/*外部变量*/
extern pthread_cond_t pthread_cond_PL;                                            //感知层线程
extern pthread_mutex_t lock_PL;
extern bool wait_PL_status;
extern pthread_cond_t pthread_cond_IPC1;                                          //进程间通信1线程
extern pthread_mutex_t lock_IPC1;
extern bool wait_IPC1_status;
extern pthread_cond_t pthread_cond_AL;                                            //系统层数据处理线程
extern pthread_mutex_t lock_AL;
extern bool wait_AL_status;
extern pthread_cond_t pthread_cond_LM;                                            //本地模块线程
extern pthread_mutex_t lock_LM;
extern bool wait_LM_status;
extern pthread_cond_t pthread_cond_ISPC;                                          //串口间通信线程
extern pthread_mutex_t lock_ISPC;
extern bool wait_ISPC_status;

bool DeviceInit();                                                                /*设备系统主管理程序*/
void CondAndMutexInit();                                                          /*线程条件变量及互斥量初始化*/
void Pthread_Cond_Broadcast();                                                    /*唤醒所有线程*/
void SignalPthread_PL();
void SignalPthread_IPCOne();
void SignalPthread_AL();
void SignalPthread_LM();
void SignalPthread_ISPC();

bool InitParament();                                                               /*设备系统参数初始化*/
bool InitDevicePara(FILE *fp);                                                     /*设备基本信息初始化*/
bool InitDevBaseParaInfor(char buff[]);                                            /*设备基本参数初始化*/
bool InitManuInfor(char buff[]);                                                   /*基本产品信息初始化*/
bool InitDevFuncInfor(char buff[]);                                                /*设备功能开关量初始化*/
bool InitControllerInfor(char buff[],int ith);                                     /*控制器信息初始化*/
bool InitISPCInfor(char buff[]);                                                   /*初始化串口交互信息*/
bool InitSerialPortInfor(char buff[]);                                             /*设备串口初始化*/
bool InitSensorTypeInfor(char buff[]);                                             /*初始化传感器类型信息*/
bool InitSensorEmerInfor(char buff[]);                                             /*初始化传感器应急指令系统的信息*/
bool InitSensorItemInfor(char buff[],unit_32 buff_i);                              /*初始化传感器参数的各项信息*/
bool SensorPortConfigure();                                                        /*串口配置*/
bool DistributePortFd();                                                           /*分配串口文件描述符*/
bool InitDataFBuffer();                                                            /*双缓存初始化*/
bool SetNet();                                                                     /*设置网络*/

bool SaveSerialPortNode(serial_port *src);                                         /*储存串口信息结点*/
bool SaveTypeNode(sensor_type *ptr_type);                                          /*储存传感器类型信息*/
bool SaveDevFuncNode(func_switch *src);                                            /*储存设备功能开关量信息结点*/
bool SaveDevVersionNode(version *src);                                             /*储存设备信息结点*/
bool SaveISPCNode(ispc* src);                                                      /*存储串口交互信息结点*/
bool SaveItemNode(sensor_item *src, unit_32 s_link);                               /*储存传感器参数信息*/
bool SaveEmerNode(emergency_cmd *src, unit_32 s_link);                             /*储存应急指令信息*/
bool SaveCtrlNode(ctrl* src);                                                      /*储存控制器结点信息*/

void SortCodeMinFirst(unit_16 **buff, int start, int len);                         /*标识码排序(从小到大)*/
bool GetTypeNode(sensor_type **src, unit_16 s_link);                               /*获取传感器类型结点信息*/
unit_32 StringTounit32(char *str, int len);                                        /*字符串转数字*/
unit_32 StringToHex(char *str, int len);                                           /*文本字符转到十六进制数*/
unit_32 StringToCmd(unit_8 *src, char *buff, int len);                             /*文本字符转到单字节十六进制无符号指令*/
unit_8 CharToHex(char c);                                                          /*单字符转十六进制*/
bool SetSpeed(int fd, int speed);                                                  /*设置串口波特率*/
bool SetSerialPortAttributes(int32_t fd, int data_bit, int stop_bit, char parity); /*设置串口属性：数据位；停止位；校验位*/
void SetParity(struct termios *opt, char check_bit);                               /*设置校验位*/
void SetStopBit(struct termios *opt, int stop_bit);                                /*设置停止位*/
void SetDataBit(struct termios *opt, int data_bit);                                /*设置数据位*/
int32_t GetSerialPortFd(unit_16 port_ith);                                         /*获取打开串口的fd文件描述符*/
bool ISPCFlag();                                                                   /*串口间通信状态*/

void printSerailPortInfo();                                                        /*打印串口参数信息*/
void printDeviceInfor();                                                           /*打印设备基本配置信息*/
void printSensorInfomation();                                                      /*打印传感器相关信息*/
void printSwithc(func_switch *func);                                               /*打印设备功能开关量*/
void printManuInfo(version *ver);                                                  /*打印设备信息*/
void printContrllerInfo(ctrl *control);                                            /*打印控制器信息*/
void printInteractPortinfor(ispc* src);                                            /*打印串口交互信息*/
void printEmerInfo( );                                                             /*打印传感器出错识别码*/
void printItemInfo( );                                                             /*打印传感器参数信息*/
void GetCRCModule(unit_8 *str, unit_16 module);                                    /*指令校验模式信息*/
void GetCRCTrun(unit_8 *str, unit_16 turn);                                        /*指令校验码翻转*/

/*-------------------------------------------------------#外部函数#-------------------------------------------------------*/
extern void *PerceiveLayerManage(void *arg);    /*传感器感知层管理程序*/
extern void* IPCOne(void *arg);                 /*系统层管理程序*/
extern void* ISPCManage(void *arg);             /*串口间通信*/
extern void *AlgorithmManage(void *arg);        /*数据解析优化算法管理程序*/
extern void *LocalManage(void *arg);            /*本地服务模块处理线程*/

extern void Reboot();                           /*设备重启*/
extern void FlushFdIn(int fd);
extern void FlushFdOut(int fd);
extern void FlushFdInOut(int fd);
extern serial_port** SerialPort();              /*串口信息链表头节点*/
extern sensor_type *TypeLinkHead();
extern device_para *DeviceParaPtr();
extern int IsWait(bool wait_status);
extern int ItemTotalNum();
extern int CycleBuffNum();

extern bool SetPortLinkHead(serial_port **ptr_src);
extern bool SetTypeLinkHead(sensor_type **ptr_src);
extern bool SetDeviceParaPtr(device_para **ptr_src);
extern void SetBuffOne(dual_buff *ptr_src);
extern void SetBuffTwo(dual_buff *ptr_src);
extern void ChangeBuffPush(dual_buff *ptr_src);
extern void ChangeBuffPull(dual_buff *ptr_src);
extern void SetTypeTotalNum(int src);
extern void SetItemTotalNum(int src);
extern void SetExchangeOverTime(int src);
extern void Perror(const char *str);
extern void printStr(char *str);
extern void printStrStr(char *str1, char *str2);
extern void printDec(char *str, int value);
extern void printFloat(char *str, float value);
extern void printHexStr(unsigned char *str, const unsigned char *buf, unit_32 len);

#endif // !INIT_H