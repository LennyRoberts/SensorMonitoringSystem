#ifndef SELF_TEST_H
#define SELF_TEST_H
/*===================================================================
* 该程序主要功能：
* 1.在系统初始化完成后开启设备及系统自检
* 2.在设备配置及自检完成后，使用简单通用测试用例，对系统功能进程测试
* ==================================================================*/
#include "../device_info.h"
#include "../logging.h"

bool SelfTestCaseInitialize( );	/*自检&测试_系统-初始化*/

extern void printStr(char *str);
#endif // !SELF_TEST_H