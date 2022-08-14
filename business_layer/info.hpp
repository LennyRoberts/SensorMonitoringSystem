#ifndef INFO_HPP
#define INFO_HPP

/*============================================================
*
* ============================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <ctime>
#include <time.h>
#include <bits/stdc++.h>
#include <sstream>     // std::stringstream
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>

#include "./lib/global_info.hpp"

#define INVALID_FD             -1       // 无效的文件描述符

/*----------------------数据包格式编码------------------------*/
#define SURFACE_WATER_QUALITY       21  //地表水质量监测
#define AIR_QUALITY                 22  //空气质量监测
#define ACOUSTIC_QUALITY            23  //声环境质量监测
#define GROUND_WATER_QUALITY        24  //地下水质量监测
#define SOIL_QUALITY                25  //土壤质量监测
#define SEA_WATER_QUALITY           26  //海水质量监测
#define VOLATILE_ORGANIC_COMPOUNDS  27  //挥发性有机物监测
#define ATMOSPHERIC_POLLUTION       31  //大气环境污染源
#define SURFACE_WATER_POLLUTION     32  //地表水体环境污染源
#define GROUND_WATER_POLLUTION      33  //地下水体环境污染源
#define MARINE_POLLUTION            34  //海洋环境污染源
#define SOIL_POLLUTION              35  //土壤环境污染源
#define ACOUSTIC_POLLUTION          36  //声环境污染源
#define VIBRATION_POLLUTION         37  //振动环境污染源
#define RADIO_ACTIVE_POLLUTION      38  //放射性环境污染源
#define CONSTRUCT_SITEDUST          39  //工地扬尘污染源
#define ELECTROMAGNETIC_POLLUTION   41  //电磁环境污染源
#define FUME_EMISSION               51  //烟气排放过程监控
#define SEWAGE_DISCHARGE            52  //污水排放过程监控

/*---------------------- 时段标志-------------------------*/
#define MINUTE_ONE             1
#define MINUTE_TWO             2
#define HOUR_ONE               3
#define HOUR_TWO               4
#define DAY_ONE                5
#define DAY_TWO                6

/*--------------------统计值计算方式-----------------------*/
#define ARITHMETIC_MEAN        0x0101  // 算术平均值
#define MOVING_AVERAGE         0x0102  // 滑动平均值
#define CUMULATIVE_VALUE       0x0103  // 累计值

using namespace std;

/*通用基础共享类*/
class JObject
{
public:
    JObject();
    unsigned short crc16_modbus(unsigned char *data, unsigned short length);
    void CrcGenerator(char *data, int len_cmd); // 指令CRC生成
    bool CrcCheck(char *data, int len_cmd);     // 指令CRC校验
    bool CheckFdStatus(int fd, int rw_status, int32_t overtime_ms);
    ~JObject();
};

/*===============================================================================================================
 * 头部标识(1) + 功能码(1) + 口令(3) + 主标识地址码(2) + 副标识地址码(2) + 数据(4) + CRC校验(2) + 尾部标识(1) = 总长16byte
 *---------------------------------------------------------------------------------------------------------------
 * 头部标识:         用来告知接收端该指令的起始位置
 * 功能码:           确定要执行的指令功能种类
 * 口令:            口令密码，防止未定义的指令对系统进行修改
 * 主标识地址码:      用来指定内部已封装的指令，确定要执行的程序
 * 副标识地址码:      用来指定内部已封装的指令，确定要执行的程序
 * 数据字段:         为方便计算指令的数据字段使用4字节有符号int来表示，最终数据默认除以1000来保持三位小数
 * CRC Module：     modbus_CRC16 + low byte first
 * 尾部标识:         标记本条指令的结束位置
 *--------------------------------------------------------------------------------------------------------------
 * 1.每次下发的指令不超过32条;
 * 2.指令具有执行优先级，功能码越小优先级越高;
 * 3.同一条指令中分为主地址标识码和副地址标识码，执行指令时默认先执行主地址标识码，若不需要副地址标识可为空;
 *==============================================================================================================*/
class cmdRegulation : public JObject
{
public:
    cmdRegulation();
    void CmdClassify(char *cmd);
    void CmdModify(char *cmd, char func_code);
    void CmdModify(char *cmd, short addr1, short addr2);
    void CmdModify(char *cmd, float data);

    void CmdGenerator(char *cmd, char version, char module_addr, char func_code, 
                    short addr1, short addr2, float data);   // 指令CRC生成
    void CmdHeadTail(char *cmd);                             // 封装指令头尾
    void CmdVersion(char *cmd, char version);                // 封装版本号
    void CmdModuleAddr(char *cmd, char module);              // 封装模块地址
    void CmdFuncCode(char *cmd, char func_code);             // 封装指令功能位
    void CmdDataLenght(char *cmd);                           // 封装数据长度
    void CmdAddress1(char *cmd, short addr1);                // 封装指令地址１
    void CmdAddress2(char *cmd, short addr2);                // 封装指令地址2
    void CmdData(char *cmd, float data);                     // 封装指令数据字段
    void CmdCrcGenerator(char *data, int len_cmd);           // 指令CRC生成

    ~cmdRegulation();

private:
};

/*统计数据类*/
class Statistics{
public:
    float minimum;
    float maximum;
    double sum;
    unsigned int counter;
};

/*传感器子项信息*/
class JItemInfor{
public:
    string item;                     // 传感器参数名称
    string table;                    // 数据标签
    unsigned short item_code;        // 传感器参数标识码
    unsigned short precision;        // 数据精度
    unsigned short mode_statistics;  // 统计值计算方式(分钟1)
    Statistics minute_one;           // 统计数字(分钟1)
    Statistics minute_two;           // 统计数字(分钟2)
    Statistics hour_one;             // 统计数字(小时1)
    Statistics hour_two;             // 统计数字(小时2)
    Statistics day_one;              // 统计数字(日数据1)
    Statistics day_two;              // 统计数字(日数据2)
    char item_en;                    // 传感器参数使能
public:
    char flag_alarm;                 // 报警状态
    char flag_sleep;                 // 休眠状态
    unsigned short counter_overflow; // 超标值次数
    unsigned short counter_warning;  // 预警值次数
};

/*服务器信息*/
class JServerInfo{
public:  
    string name;                    // 服务器名称
    string type;                    // 服务器类型
    char net_IP[16];                // IP地址
    unsigned int net_port;          // 网络端口号
    unsigned short packet_code;     // 数据包格式
    char alarm_en;                  // 报警上传使能
    char ser_en;                    // 服务器使能
    unsigned short num_item;        // 参数个数
    unsigned short sever_No;        // 传感器编号
    int fd_sock;                    // 通讯套接字文件描述符
    char           real_time;       // 实时值
    unsigned short time_minute_one; // 第1个分钟值
    unsigned short time_minute_two; // 第2个分钟值
    unsigned short time_hour_one;   // 第1个小时值
    unsigned short time_hour_two;   // 第2个小时值
    unsigned short time_day_one;    // 第1个天平均值
    unsigned short time_day_two;    // 第2个天平均值
    vector<string> package_info;    // 数据包参数信息
    vector<JItemInfor> item;        // 传感器参数列表
};

/*功能性参数信息*/
struct Mtime{
    unsigned short min;
    unsigned short hour;
};
class JCtrPara{
public:
    string slot;                      // 时段标志
    struct Mtime DST_start;           // 夏令时起始时间
    struct Mtime DST_end;             // 夏令时结束时间
    struct Mtime ST_start;            // 标准时起始时间
    struct Mtime ST_end;              // 标准时结束时间
    unsigned short item_code;         // 传感器参数标识码
    char  enable;                     // 联动使能
    char  mode;                       // 阈值判断
    float          warning_value;     // 预警值
    float          alarm_value;       // 报警值
    float          hit_rate;          // 命中率
    unsigned short cycle_check;       // 循环检测次数
    unsigned short hold_period;       // 报警持续周期(s)
    unsigned short delay_switching;   // 切换延迟周期(s)
    unsigned short dormant_period;    // 间歇周期(s)
    float          cancel_value;      // 报警关闭值
    unsigned short warning_action;    // 预警行为动作
    unsigned short alarm_action;      // 报警行为动作
    unsigned short cancel_action;     // 解除行为动作
};

/*参数信息即初始化类*/
class JInformation{
public:
    unsigned short num_server;
    unsigned short num_para;
    unsigned int buff_size;
    unsigned int enable_ser_ith;
    vector<JServerInfo> server_para;    // 服务器信息列表
    vector<JCtrPara> ctr_para;          // 控制参数列表
public:
    JInformation();
    bool InitParameters();
    void InitServerInfo(vector<string> res, unsigned short ser_No, ifstream &inFile);
    void BuffCtrParaInfo(vector<JCtrPara> &ctr_buff, vector<string> res);
    void ParaCtlTime(JCtrPara &para_node, string str);
    void InitCtrParaInfo(std::vector<JCtrPara> &ctr);
    void printSeverInfor();
    void printCtrParaInfo(std::vector<JCtrPara> &ctr);
    string FindAvgCalculateMode(unsigned short mode_opt);
    vector<string> split(const string &str, const string &delim);
    vector<string> splitChar(const char *str, int len, const string& delim);
    unsigned short StrToHex(string str);
    ~JInformation();
};

/*========================================================================================================*/
extern void InitGlobalValue();              // 全局变量初始化
extern JInformation jInfo;                  // 参数信息类全局对象
extern pthread_t tid_device;                // 与设备层交互线程与设备层交互线程
extern pthread_t tid_centre;                // 与服务器交互线程
extern pthread_t tid_wincc;                 // 与上位机交互线程
extern pthread_t tid_server;                // 本机作为服务器交互线程

extern pthread_cond_t pthread_cond_Dev;     // 与设备层交互线程
extern pthread_mutex_t lock_Dev;
extern bool wait_Dev_status;
extern pthread_cond_t pthread_cond_Centre;  // 与服务器交互线程
extern pthread_mutex_t lock_Centre;
extern bool wait_Centre_status;
extern pthread_cond_t pthread_cond_Wincc;   // 与上位机交互线程
extern pthread_mutex_t lock_Wincc;
extern bool wait_Wincc_status;
extern pthread_cond_t pthread_cond_Server;  // 本机作为服务器交互线程
extern pthread_mutex_t lock_Server;
extern bool wait_Server_status;
#endif //!INFO_HPP