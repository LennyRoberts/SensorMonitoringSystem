#ifndef DATA_HANDLING_HPP
#define DATA_HANDLING_HPP

#include <iostream>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#include "info.hpp"
#include "as_client.hpp"
#include "./lib/JSON/CJsonObject.hpp"

#define	IPC_BUSINESS   "business.sock"
#define	IPC_DEVICE     "device.sock"

#define BUFF_SIZE               4096       // 数据包接收缓存的大小

/*本地Unix域套接字IPC通信客户端处理类*/
class UnixSock : public cmdRegulation
{
public:
    UnixSock();
    void UnixSockClientLink();                // Unix域套接字IPC客户端连接程序
    void CloseUnixSock();                     // 断开与设备层的本地套接字连接
    int  SendCmd(char *cmd_data, int len);    // 向设备层发送指令信息
    int  RecvData(char *buff_data, int len);  // 接收来自设备层的数据内容
    int  SendCmdToDevice();                   // 封装指令并向设备层发送指令
    bool DeviceJSONDataParser(const char buff[]);// 解析数据并缓存
    void InterCtlManage(struct tm *timeval);  // 智能联动控制
    float GetSensorData(unsigned short code); //获取传感器测量值
    void GetCurrentCtrPara(struct tm *timeval, std::vector<JCtrPara> &ctr); //将当前符合要求的控制参数项取出来并储存以供后面的判断使用
    void UploadDataToServer();                // 数据封装并上传至服务器函数
    ~UnixSock();
public:
    int sock_fd;
private:
};


/*HJ212协议系统编码表*/
typedef struct HJ212_ST{
    const unsigned char surfaceWaterQuality      = 21;  //地表水质量监测
    const unsigned char AirQuality               = 22;  //空气质量监测
    const unsigned char AcousticQuality          = 23;  //声环境质量监测
    const unsigned char GroundwaterQuality       = 24;  //地下水质量监测
    const unsigned char SoilQuality              = 25;  //土壤质量监测
    const unsigned char SeawaterQuality          = 26;  //海水质量监测
    const unsigned char VolatileOrganicCompounds = 27;  //挥发性有机物监测
    const unsigned char AtmosphericPollution     = 31;  //大气环境污染源
    const unsigned char SurfaceWaterPollution    = 32;  //地表水体环境污染源
    const unsigned char GroundWaterPollution     = 33;  //地下水体环境污染源
    const unsigned char MarinePollution          = 34;  //海洋环境污染源
    const unsigned char SoilPollution            = 35;  //土壤环境污染源
    const unsigned char AcousticPollution        = 36;  //声环境污染源
    const unsigned char VibrationPollution       = 37;  //振动环境污染源
    const unsigned char RadioactivePollution     = 38;  //放射性环境污染源
    const unsigned char ConstructSiteDust        = 39;  //工地扬尘污染源
    const unsigned char ElectromagneticPollution = 41;  //电磁环境污染源
    const unsigned char FumeEmission             = 51;  //烟气排放过程监控
    const unsigned char SewageDischarge          = 52;  //污水排放过程监控
}hj212_st;

/*/*HJ212协议命令编码表*/
typedef struct HJ212_CN{
    const unsigned short real_data   = 2011;
    const unsigned short day_data    = 2041;
    const unsigned short minute_data = 2051;
    const unsigned short hour_data   = 2061;
}hj212_cn;

/*HJ212协议传输状态位*/
typedef struct HJ212_Falg{
    const char normal            = 'N';  //在线监控（监测）仪器仪表工作正常 
    const char shoutdown         = 'F';  //在线监控（监测）仪器仪表停运
    const char maintenance       = 'M';  //在线监控（监测）仪器仪表处于维护期间产生的数据
    const char manually          = 'S';  //手工输入的设定值 
    const char instrumentFailure = 'D';  //在线监控（监测）仪器仪表故障 
    const char calibrationState  = 'C';  //在线监控（监测）仪器仪表处于校准状态 
    const char exceedsUpperLimit = 'T';  //在线监控（监测）仪器仪表采样数值超过测量上限 
    const char abnormal          = 'B';  //在线监控（监测）仪器仪表与数采仪通讯异常
}hj212_falg;

/*国标212协议数据包处理类*/
class UnifyHJ212Protocol
{
private:
    hj212_st sysCodeTable;
    hj212_cn cmdCOdeTable;
    hj212_falg flag;
public:
    UnifyHJ212Protocol();
    int GetDataPacketHJ212(char *buff, vector<JItemInfor> *item, int CN, vector<string> *package_info);  // 212数据包封装
    unsigned short HJ212CRCGenerator(unsigned char* puchMsg, unsigned int usDataLen); // HJ212协议CRC校验发生器
    string GetQN();                                        // 获取请求编码
    string GetST(const int st);                            // 获取系统编码
    string GetCN(const unsigned char value_model);         // 获取命令编码
    string GetPW(const int pw);                            // 访问密码PW
    string GetMN();                                        // 设备唯一标识码
    char GetFlag(const int flag_value) const;              // 标识符
    ~UnifyHJ212Protocol();
};


/*JSON格式协议数据包处理类*/
class JsonProtocol{
public:
    JsonProtocol();
    ~JsonProtocol();
};

/*MQTT格式协议数据包处理类*/
class MQTTProtocol{
public:
    MQTTProtocol();
    ~MQTTProtocol();
};

/*设备层传感器数据接收缓存类*/
class DataBuff{
public:
    unsigned short sensor_code;
    float data;
    char status;
    string unit;
};

/*数据处理与封装类*/
class JPackage : public neb::CJsonObject, public cmdRegulation
{
public:
    JPackage();
    void TimeCtrStatistics(struct tm *timeval, JServerInfo *server);          //数据统计时间控制机制
    void StatisticsMeasureValue(JServerInfo *server, int step,int opt, bool &clear_flag);            //统计测量值
    void CalculateStatisticsData(JItemInfor *item, int step, float value, int opt, bool &clear_flag);//统计数据计算
    void ArithmeticMean(JItemInfor *item, int step, float value, int opt, bool &clear_flag);         //计算算术平均数
    int  GetDataPackets(char *buff, unsigned short format, int CN, vector<JItemInfor> *item, vector<string> *package_info);  // 数据封装函数
    string GetDeviceNo();                        // 获取设备号
    string GetMeasureTime();                     // 获取时间
    ~JPackage();
private:
    UnifyHJ212Protocol hj212;
    JsonProtocol json;
    MQTTProtocol mqtt;
public: 
    string deviceNo;                // 当前设备号
    vector<DataBuff> data_buff;     // 设备层传感器数据接收缓存
};

/*====================================================================*/
extern JInformation jInfo;                  // 参数信息类全局对象
extern TCPClient tcp_client;                // 本机客户端对象
extern UnixSock unix_sock;                  // IPC交互对象
extern JPackage package;                    // 数据包封装对象
extern vector<DataBuff> data_buff;          // 设备层传感器数据接收缓存
extern void *InteractDevice(void *arg);     // IPC交互线程入口

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

#endif //!DATA_HANDLING_HPP