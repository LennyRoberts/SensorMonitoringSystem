#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H
/*================================================================================
* 这是可供全局访问的文件，框架代码里的所有文件均可共享访问该文件内的变量和函数，
* 其他文件里的变量和函数仅在该本文件中可访问，跨文件访问则通过函数传参的方式进行，
* 在修改必要的全局变量时，请调用已定义函数的方式进行，拒绝直接修改全局变量，访问则不受限。
* ================================================================================*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<pthread.h>
#include	<errno.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/poll.h>
#include	<termios.h>
#include	<semaphore.h>
#include	<math.h>

#include    "./lib/global_info.h"
#include    "./lib/crc/crc_lib.h"
#include    "./lib/cJSON/cJSON.h"


typedef signed char             int8_t;     /*80~7F = -128~127*/
typedef short                   int16_t;    /*8000~7FFF = -32768~32767*/
typedef int                     int32_t;    /*8000 0000~7FFF FFFF = -2147483648~2147483647*/
// typedef long long               int64_t;     /*8000 0000 0000 0000~7FFF FFFF FFFF FFFF*/ /*使用64位linux的GCC下已被定义*/
typedef unsigned char           unit_8;     /* 00~FF = 0~255 */
typedef unsigned short          unit_16;    /* 0000~FFFF = 0~65535 */
typedef unsigned int            unit_32;    /* 0000 0000~FFFF FFFF = 0~42 9496 7295 */
typedef unsigned long long      unit_64;    /* 0000 0000 0000 0000~FFFF FFFF FFFF FFFF */
typedef char                    bool;
typedef unit_8                  status_rw; //读写状态
typedef unit_8                  model_r;   //主/被动数据获取模式

typedef bool (*func_cmd)(unsigned short, unsigned short, float, int);

typedef void (*hex)(unsigned char *str, const unsigned char *buf, unit_32 len);
/*===============================================#全局宏定义#========================================================*/
#define TRUE                     1	    // bool true
#define FALSE                   -1	    // bool false
#define INVALID_FD              -1      // 无效文件描述符

#define DEVICE_VERSION          0x0A    // 系统软件版本JoMiDev1.0

#if 1
#define SERVICES_PATH          "./arm_business.o"   // 服务层程序路径
#define SERVICES_NAME          "arm_business.o"     // 服务层程序名
#else
#define SERVICES_PATH          "./x86_business.o"   // 服务层程序路径
#define SERVICES_NAME          "x86_business.o"     // 服务层程序名
#endif

#define PRINT_ALL               1       // 全部信息打印函数封装

#define SIGNED_SHORT            0x81    //有符号短整型
#define UNSIGNED_SHORT          0xF1    //无符号短整型
#define SIGNED_INT              0x82    //有符号整数型数据
#define UNSIGNED_INT            0xF2    //无符号整数型
#define FLOATING                0x83    //浮点型数据

#define ORDER_ABCD              0xABCD
#define ORDER_CDAB              0xCDAB
#define ORDER_BADC              0xBADC
#define ORDER_DCBA              0xDCBA

#define READ                    0xFE    //读指令状态
#define WRITE                   0xFA    //写指令状态
#define READ_PASSIVE            0x01    //读串口被动模式
#define READ_ACTIVE             0x02    //读串口主动模式
#define TIME_S_FD_NOT_AVAILABLE 0		//s
#define TIME_FD_NOT_AVAILABLE   500000  //us
#define POLL_OVERTIME_MS        200     //I/O多路复用poll超时检测时间
#define TIME_INTERVAL           1       //相邻传感器读取时间间隔(s)

#define SERIAL_POER_NUM         16      //最大支持串口数量
#define MAX_ITEM_SIZE           32      //传感器内部识别码缓存大小
#define MIN_CYCLE_NUM           3       //传感器子项最小轮询储存次数
#define MAX_BUFFER_SIZE         MAX_ITEM_SIZE * MIN_CYCLE_NUM //(双缓存机制)数据缓存区大小(*4byte[float])/一次能缓存的传感器子项数据个数
#define MAX_ALGO_CODE_SIZE      16      //传感器子项所含算法个数容量
#define MAX_CJSON_BUFFER_SIZE   4096    //CJSON格式数据包缓存大小
#define MAX_MODBUS_BUFFER_SIZE  MAX_ITEM_SIZE*4   //ISPC缓存大小

/*==========================#结构体信息#=================================*/
/*打印输出函数集合*/
typedef struct Print{
	hex printHex;
}put_print;

/*(双缓存机制)缓冲区结构*/
typedef struct DualBuff{
	int    flag;
	float  buff[ MAX_BUFFER_SIZE ];
	time_t m_time[ MAX_ITEM_SIZE ];
	struct DualBuff *next;
}dual_buff;

/*应急指令*/
typedef struct emergencyCmd{
	unit_8  emer_name[32]; //应急指令名称
	unit_8  cmd[32];       //指令
	unit_16 len_cmd;       //指令长度
	unit_16 start_byte;    //指令比较起始位位置
	unit_16 len_byte;      //指令比较长度
	unit_16 emer_code;     //应急功能识别码
	struct  emergencyCmd *next;
}emergency_cmd;

/*传感器子项参数信息(参数：即被监测物质的名称，如PM2.5)*/
typedef struct sensorItem{
	unit_8  item_name[32];    //参数名称
	unit_8  unit_origin[16];  //传感器单位
	unit_8  unit_trans[16];   //传感器单位
	unit_16 byte_order;       //字节顺序:ABCD,CDAB,BADC,DCBA
	char    value_type;       //数值类型:
	float   a;                //y=ax+b a:缩放因子
	float   b;                //y=ax+b b:数据偏移
	float   unit_ratio;       //单位转换系数
	float   minimum;          //最小值限定
	float   maximum;          //最大值限定
	unit_32 start_byte;       //解析起始字节位置
	unit_32 len_byte;         //解析数据长度(字节)
	unit_32 buff_ith;         //储存序号
	unit_16 sensor_code;      //传感器子项内部统一标识码
	unit_16 algo_code[MAX_ALGO_CODE_SIZE];//传感器子项算法统一识别码
	unit_16 num_algo_code;    //传感器子项使用的算法个数
	struct sensorItem *next;
}sensor_item;

/*传感器类型信息(一条应答指令中可包含多个传感器参数的数据，如：多合一气象5参)*/
typedef struct sensorType{
	unit_8    type_name[32];    //传感器类型名称
	unit_8    read_cmd[32];     //读取指令
	unit_32   len_cmd;          //读取指令的长度(字节)
	unit_32   len_data;         //指令回复长度(字节)
	unit_16   len_startflag;    //头部标志长度(字节)
	unit_16   crc_module;       //CRC校验模式
	unit_16   crc_turn;         //CRC码翻转(高位/低位在前)
	unit_16   sensor_link;      //传感器关联码
	char      state;            //传感器状态
	unit_8    read_mode;        //读取模式：被动模式0x02,主动模式0x01
	unit_16   time_response;    //传感器响应时间
	unit_8    start_flag[8];    //回复数据的头部标志
	int32_t   port_ith;         //使用第i个串口
	int32_t   fd_port;          //第i个串口的文件描述符
	sensor_item   *item_head;   //指向传感器内部各参数信息头结点的指针(单链表)
	emergency_cmd *emer_head;   //应急指令头结点的指针(单链表)
	struct sensorType *next;
}sensor_type;

/*使用串口的指令执行*/
typedef struct InterSerialPortCommunication{
	unit_8    name[32];        //串口间通信名称
	char      enable;          //使能位
	char      monopoly;        //是否独占
	int32_t   port_ith;        //串口编号
	int32_t   fd_port;
	struct InterSerialPortCommunication *next;
}ispc;

/*控制器结构体信息*/
typedef struct Controller{
	unit_8    ctrl_name[32];    //控制器名称
	unit_32   len_open_order;   //指令长度
	unit_16   ctrl_ON;          //控制器模块编号
	int8_t    ctrl_type;        //控制器类型(继电器或控制器)
	int8_t    initial_state;    //初始状态
	unit_8    open_order[64];   //控制器指令
	unit_8    close_order[64];  //控制器指令
	unit_32   len_close_order;  //指令长度
	int32_t   port_ith;         //控制器使用串口的编号
	int32_t   fd_port;          //使用串口文件描述符
	struct Controller *next;
}ctrl;

/*设备功能开关量*/
typedef struct funcSwitch{
	char action;            //联动
	char video_overlay;     //视频叠加
	char video_capture;     //视频抓拍
	char DTU_trans;         //DTU数据通信
	char net_port_trans;    //网口数据通信
	char beeper_alarm;      //蜂鸣器报警
	char text_screen;       //文本屏
	char calibration;       //文本屏校准
}func_switch;

/*设备信息*/
typedef struct Version{
	unit_8 dev_module[32];       //设备型号
	unit_8 dev_no[32];           //设备编号
	unit_8 production_date[32];  //出厂日期
	unit_8 hardware_version[16]; //硬件版本
	unit_8 software_version[16]; //软件版本
	unit_8 kernel_version[16];   //内核版本
	unit_8 manufacture_name[16]; //厂商信息
}version;

/*设备基本配置信息*/
typedef struct devicePara{
	unit_8  IP[16];             //IP地址
	unit_32 port;               //端口号
	unit_8  mask[16];           //子网掩码
	unit_8  gateway[16];        //默认网关
	unit_16 heart_beat_time;    //心跳包时长
	unit_16 num_type;           //传感器种类数量
	unit_16 num_item;           //传感器参数总数
	unit_16 cycle_buff_num;     //传感器轮询存储次数(批处理方式：读串口存储n次才处理一次数据)
	func_switch *func;          //设备功能开关
	version *version;           //设备信息
	ctrl    *ctrl_head;         //控制器信息
	ispc    *ispc_head;         //串口交互,串行端口间通信
}device_para;

/*串口属性信息*/
typedef struct serialPort{
	unit_8          portName[ 32 ];  //串口别名
	unit_8          devName[ 32 ];   //串口设备名
	unit_32         speed;           //串口波特率	
	int32_t         fd_port;         //第i个串口打开的文件描述符
	int32_t         port_ith;        //设备第i个串口
	unit_16         dataBit;         //数据位
	unit_16         stopBit;         //停止位
	char            parity;          //校验位
	char            state;           //串口状态：Y=可用；N=不可用；U=未初始化;M=被独占
	unit_8          obj_state[16];   //状态对象
	struct serialPort *next;
}serial_port;

/*===========================================================#全局共享变量#==================================================================*/
/*线程条件变量与互斥量*/
extern pthread_cond_t pthread_cond_PL;    //感知层线程
extern pthread_mutex_t lock_PL;
extern bool wait_PL_status; 
extern pthread_cond_t pthread_cond_IPC1;  //进程间通信1线程
extern pthread_mutex_t lock_IPC1;
extern bool wait_IPC1_status;
extern pthread_cond_t pthread_cond_AL;    //系统层数据处理线程
extern pthread_mutex_t lock_AL;
extern bool wait_AL_status;
extern pthread_cond_t pthread_cond_LM;    //本地模块线程
extern pthread_mutex_t lock_LM;
extern bool wait_LM_status;
extern pthread_cond_t pthread_cond_ISPC;  //串口间通信线程
extern pthread_mutex_t lock_ISPC;
extern bool wait_ISPC_status;

extern float result_buff[MAX_ITEM_SIZE];            // 设备层最终计算数据缓存
extern char cjson_buff[MAX_CJSON_BUFFER_SIZE];      // CJSON格式数据包缓存

/*=============================================================#全局函数#===================================================================*/
void printStyle(char *buf, int data);
void printStylef(char *buf, float data);
void printStr(char *str);                                                              /*传入要打印字符串首地址 >> 以字符串形式输出*/
void print_str(char *str);
void printStrStr(char *str1, char *str2);
void printHex(const unsigned char *buf, unit_32 len);                                  /*传入要打印内容buf + 指定长度len >> 以十六进制形式输出*/
void printHexStr(unsigned char *str, const unsigned char *buf, unit_32 len);
void printDec(char *str, int value);
void printFloat(char *str, float value);
void printDec_unit_8(const char *buf, unit_16 len);                                    /*传入要打印内容buf + 指定长度len >> 以十进制形式输出*/
void Perror(const char *str);

char *Int2Net(char *net_buff, int net_int);                                            /*字符串转IP地址*/
int  int2baud(int baud);                                                               /*整型转波特率型(speed_t)*/
unsigned int HexCharToUnsignedLong(unit_8 *buff, char type, unit_16 byte_order);       /*无符号char型数据解析*/
int HexCharToSignedLong(unit_8 *buff, char type, unit_16 byte_order);                  /*有符号char型数据解析*/
float HexToFloatPoint(unit_8 *buff);                                                   /*IEEE_754 浮点型数据解析器*/

bool commandExecute(char *cmd, char *isRead);                                          /*终端指令执行函数:指令cmd + 标志位 >> 指令写入终端*/
void Reboot();                                                                         /*设备重启*/
void FlushFdIn(int fd);                                                                /*清除正收到的数据，且不会读取出来*/
void FlushFdOut(int fd);                                                               /*清除正写入的数据，且不会发送至终端*/
void FlushFdInOut(int fd);                                                             /*清除所有正在发生的I/O数据*/
bool CRC_Check(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len);     /*CRC校验检查*/
void CRC_Generator(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len); /*校验值生成器*/
int  CheckFdStatus(int fd, status_rw rw_status, int32_t overtime_ms);                  /*I/O多路复用poll，超时检测当前文件描述符是否准备好*/
int32_t WriteCMD(unit_8 *name, int fd, unit_8 *cmd, int len_cmd, unit_16 CRC_module, unit_16 CRC_turn); /*写入指令*/
int IsOverBuffer(int hash_level);                                                      /*双缓存机制中push是否满*/
int GetBuffIndex(int buff_ith, int level);                                             /*获取数据缓存的索引*/
int GetBuffIndexByCode(unit_16 sensor_code, int level);                                /*通过标识码获取数据缓存的索引*/
int IsWait(bool wait_status);                                                          /*线程是否处于阻塞状态*/
int IsEmptyType(sensor_type *type);                                                    /*判断传感器类型结构是否为空*/
int IsEmptyItem(sensor_item *item);                                                    /*判断参数信息结构是否为空*/

/*结构体外置头部节点信息*/
sensor_type* TypeLinkHead();                         /*传感器信息链表头节点*/
device_para* DeviceParaPtr();                        /*设备信息结构体*/
serial_port** SerialPort();                          /*串口信息链表头节点*/

/*全局参量信息*/
dual_buff* BuffPtrOne();                             /*双缓存机制缓存1*/
dual_buff* BuffPtrTwo();                             /*双缓存机制缓存2*/
dual_buff* BuffPush();                               /*双缓存机制中面向数据处理的地址*/
dual_buff* BuffPull();                               /*双缓存机制中面向传感器转储的地址*/
int TypeTotalNum();                                  /*传感器类别总数*/
int ItemTotalNum();                                  /*传感器项目总数*/
int CycleBuffNum();                                  /*获取双缓存机制传感器存储次数*/
int ExchangeOverTime();                              /*双缓存机制交换超时时间*/

/*结构体内部信息*/
sensor_type *TypeHead(sensor_type **type);           /*指向传感器类别链表头节点*/
sensor_type *TypeNext(sensor_type **type);           /*指向传感器类别链表下一节点*/
unit_8* TypeName(sensor_type *type);
unit_8* ReadCmd(sensor_type *type);
unit_32 LenCmd(sensor_type *type);
unit_32 LenData(sensor_type *type);
unit_8* DataStartFlag(sensor_type *type);
unit_32 LenStartFlag(sensor_type *type);
unit_32 PortIth(sensor_type *type);
int     FdPort(sensor_type *type);
unit_32 NumItem(sensor_type *type);
unit_16 CRCModule(sensor_type *type);
unit_16 CRCTurn(sensor_type *type);
unit_32 SensorState(sensor_type *type);
sensor_item* ItemHead(sensor_type *type);            /*获取当前传感器类别中传感器项目链表的头节点*/
emergency_cmd* EmergencyCmd(sensor_type *type);      /*获取当前传感器类别中应急指令链表的头节点*/

unit_8 *ItemName(sensor_item *item);
unit_8 *ItemLabel(sensor_item *item);
unit_8 *ItemUnit(sensor_item *item);
int ItemDataCode(sensor_item *item);
unit_16* ItemAlgoCode(sensor_item *item);
sensor_item *ItemNext(sensor_item **item);           /*指向传感器项目的下一个节点*/

void GetDeviceNo(char *buff);                        /*获取设备编号*/

/*Please use function mode to modify the necessary global variables*/
void InitGlobalValue();                                             /*全局变量初始化*/
bool SetPortLinkHead(serial_port **ptr_src);                        /*设置串口信息链表的头节点*/
bool SetTypeLinkHead(sensor_type **ptr_src);                        /*设置传感器类别链表的头节点*/
bool SetDeviceParaPtr(device_para **ptr_src);                       /*设置设备信息的结构地址*/

void SetBuffOne(dual_buff* ptr_src);                                /*设置双缓存机制中缓存器1的入口*/
void SetBuffTwo(dual_buff* ptr_src);                                /*设置双缓存机制中缓存器2的入口*/
void ChangeBuffPush(dual_buff *ptr_src);                            /*改变双缓存机制的Push缓存指向*/
void ChangeBuffPull(dual_buff *ptr_src);                            /*改变双缓存机制的Pull缓存指向*/
void SetTypeTotalNum(int src);                                      /*设置传感器种类数量*/
void SetItemTotalNum(int src);                                      /*设置传感器总数数量*/
void SetExchangeOverTime(int src);                                  /*设置双缓存机制交换超时时间*/
float GetResult(int buff_pos);                                      /*存储最终值*/
void SaveResult(int buff_ith, float result);                        /*获取最终值*/
void InitResultBuff();                                              /*重置最终缓存函数*/
void SaveJsonData(char *str);                                       /*保存CJSON格式数据*/
int  GetJsonPackage(char *str);                                     /*获取Json格式数据*/
void SaveModbusData(char *str,int len);                             /*储存封装的modbus指令数据*/
int  GetModbusData(char *str);                                      /*获取封装的modbus数据*/

/*设置传感器类别Type结构内的信息*/
void SetTypeName(sensor_type* type, unit_8 *name);
void SetSensorReadCmd(sensor_type* type, unit_8 cmd);
void SetLenReadCmd(sensor_type *type, unit_32 len_cmd);
void SetLenResponData(sensor_type *type, int len_data);
void SetStartFlag(sensor_type *type, unit_8 *s_flag);
void SetLenStartFlag(sensor_type *type, unit_32 len_sflag);
void SetPortOrder(sensor_type *type, unit_32 port_i);
void SetUseFdPort(sensor_type *type, int fd);
void SetTypeItemNum(sensor_type *type, unit_32 num_item);
void SetCRCModule(sensor_type *type, unit_16 crc_module);
void SetCRCTurn(sensor_type *type, unit_16 crc_turn);
void SetSensorState(sensor_type *type, unit_32 state);              /*设置传感器的工作状态*/
void SetSnenrItem(sensor_type *type, sensor_item *item);
void SetSensorEmerCmd(sensor_type *type, emergency_cmd *emer_cmd);

/*设置传感器项目Item结构内的信息*/
void SetItemName(sensor_item *item, unit_8 *name);
void SetItemLabel(sensor_item *item, unit_8 *label);
void SetItemUnit(sensor_item *item, unit_8 *label);
void SetDataStartByte(sensor_item *item, unit_16 s_byte);
void SetLenDataSByte(sensor_item *item, uint16_t len_s_byte);
void SetSensorValueType(sensor_item *item, unit_8 v_type);
void SetDecPointPlace(sensor_item *item, unit_8 dec_place);
void SetMagnModule(sensor_item *item, unit_8 magn_modle);
void SetMagnValue(sensor_item *item, float magn_value);
void SetSensorCode(sensor_item *item, unit_32 sensor_code);
void SetAlgoCode(sensor_item *item, unit_32 algo_code);

/*外部函数*/
extern int __CRC_Check(unsigned short crc_name, unsigned short storage_mode, unsigned char *data, unsigned int len);         /*CRC校验检查执行函数*/
extern void __CRC_Generator(unsigned short crc_model, unsigned short storage_mode, unsigned char **data, unsigned int len);
/*=================================================================================================================================================*/
#endif // !DEVICE_INFO_H
