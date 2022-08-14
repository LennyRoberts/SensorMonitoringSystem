#ifndef SPL_MANAGE_H
#define SPL_MANAGE_H
/*============================================================================
* 传感器感知层管理程序，主程序通过管理该管理程序从而实现对传感器感知层的管理
* ============================================================================*/
#include "../logging.h"
#include "../device_info.h"
#include "../sys_func_layer/sfl_manage.h"
#include "sensor.h"

#define MAX_REPLAY_CMD_SIZE      256   //响应指令缓存大小

/*外部变量*/
extern pthread_cond_t pthread_cond_PL; //感知层线程
extern pthread_mutex_t lock_PL;
extern bool wait_PL_status;
extern pthread_cond_t pthread_cond_IPC1;//进程间通信1线程
extern pthread_mutex_t lock_IPC1;
extern bool wait_IPC1_status;
extern pthread_cond_t pthread_cond_AL; //系统层数据处理线程
extern pthread_mutex_t lock_AL;
extern bool wait_AL_status;

extern unit_8 buff_reply[ MAX_REPLAY_CMD_SIZE ];      /*传感器/模块应答指令数据包缓存*/
extern int time_wait;

void *PerceiveLayerManage(void *arg);   /*传感器感知层管理程序*/
void PLManageInit( );

/*外部函数*/
extern sensor_type* TypeLinkHead();     //传感器信息链表头节点
extern device_para* DeviceParaPtr();    //设备信息结构体
extern void printStr(char *str);
extern void print_str(char *str);
extern void printHexStr(unsigned char *str, const unsigned char *buf, unit_32 len);
extern void SensorHandlingInitialize( );
extern int IsWait(bool wait_status);
extern int IsOverBuffer(int hash_level);
extern int IsEmptyType(sensor_type *type);
extern sensor_type* TypeHead(sensor_type** type);
extern sensor_type* TypeNext(sensor_type **type);
extern unit_8 *TypeName(sensor_type *type);
extern int32_t Digital_RW_Controller(sensor_type *type, unit_8 *buff_reply, status_rw rw_status, model_r r_modle);
extern emergency_cmd *EmergencyCmdComparator(sensor_type *type, unit_8 *buff_reply);
extern void SensorEmergencyHandling(sensor_type *type, int32_t emer_code);
extern void CmdDataParser(dual_buff *push, unit_8 *buff, sensor_type *type, int level);  /*指令数据解析与储存*/
extern int  TypeTotalNum();
extern dual_buff* BuffPush();
#endif //!SPL_MANAGE_H
