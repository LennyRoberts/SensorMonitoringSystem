#ifndef AS_CLINET_HPP
#define AS_CLINET_HPP
/*=================
* 板子作为TCP客户端
* =================*/
#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>      //sockaddr_in (network byte order)
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#include "info.hpp"

using namespace std;

class TCPClient : public cmdRegulation
{
public:
    TCPClient();
    void TCPSockLink();                                      //建立TCP通信
    int  TCPSockConnectSingle(const char *ip, const int port);//单个TCP建立执行函数
    int  TcpSend(JServerInfo &server, const char *str, int len); //数据上传至服务器
    int  TcpRevice(int fd_sock,char *buff);              //接收指令信息
    void CloseTcpLink(JServerInfo &ser);                 //关闭TCP连接
    ~TCPClient();
public:
};


/*=================================================================================*/
extern JInformation jInfo;                  //参数信息类全局对象
extern TCPClient tcp_client;
extern void *InteractCentre(void *arg);

extern pthread_t tid_device;                //与设备层交互线程与设备层交互线程
extern pthread_t tid_centre;                //与服务器交互线程
extern pthread_t tid_wincc;                 //与上位机交互线程
extern pthread_t tid_server;                //本机作为服务器交互线程

extern pthread_cond_t pthread_cond_Dev;     //与设备层交互线程
extern pthread_mutex_t lock_Dev;
extern bool wait_Dev_status;
extern pthread_cond_t pthread_cond_Centre;  //与服务器交互线程
extern pthread_mutex_t lock_Centre;
extern bool wait_Centre_status;
extern pthread_cond_t pthread_cond_Wincc;   //与上位机交互线程
extern pthread_mutex_t lock_Wincc;
extern bool wait_Wincc_status;
extern pthread_cond_t pthread_cond_Server;  //本机作为服务器交互线程
extern pthread_mutex_t lock_Server;
extern bool wait_Server_status;
#endif //!AS_CLIENT_HPP