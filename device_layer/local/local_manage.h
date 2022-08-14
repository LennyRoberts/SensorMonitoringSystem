#ifndef LOCAL_MANAGE_H
#define LOCAL_MANAGE_H
/*===============================================================================
* 
* ==============================================================================*/
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <strings.h>
// #include <string.h>
// #include <ctype.h>
// #include <arpa/inet.h>
// #include <sys/un.h>
// #include <stddef.h>

#include "../device_info.h"
#include "../logging.h"
#include "local_info.h"
#include "led_screen.h"

// #define SERVICE_ADDR "service.sock"
// #define DEVICE_ADDR  "device.sock"
// void LocalServiceManage();
// int UnixSockClientLink();
// int RecvFrom(int sock_fd, unsigned char *buf, int len);
// int SendTO(int sock_fd, unsigned char *buf, int len);

extern pthread_cond_t pthread_cond_LM;        //本地模块线程
extern pthread_mutex_t lock_LM;
extern bool wait_LM_status;

void *LocalManage(void *arg);
bool LocalModuleModify(unit_8 *cmd);          /*处理来自服务层的本地服务模块指令*/
void GetSensorData();                         /*获取传感器测量数据*/

/*外部函数*/
extern sensor_type *TypeLinkHead();           /*传感器信息链表头节点*/
extern float GetResult(int buff_pos);         /*获取最终值*/
#endif //!LOCAL_MANAGE_H