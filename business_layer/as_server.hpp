#ifndef AS_SERVER_HPP
#define AS_SERVER_HPP
/*=================================================
* 板子作为服务器
* =================================================*/
#include "info.hpp"

using namespace std;

extern void *AaServer(void *arg);

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
#endif //!AS_SERVER_HPP