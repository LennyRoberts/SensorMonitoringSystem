#include "device_info.h"
#include "logging.h"
#include "./init/init.h"

/*外部函数声明*/
extern void InitGlobalValue();
extern void LoggingManage();
extern void InitLogging();
extern bool commandExecute(char *cmd, char *isRead);
extern bool InitParament();
extern bool DeviceInit();
extern bool SelfTestCaseInitialize( );
extern void printStr(char *str);
extern void Pthread_Cond_Broadcast( );

void HwclockTimeSyncToSystem( )
{
	commandExecute("hwclock -s", "w");
}

/*设备基本硬件初始化*/
void HardwareInitialize( )
{
	printStr("Hardware Initalizing...");
	HwclockTimeSyncToSystem( );
	printStr("Hardware Initalization Finish!\n");
}

void Welcome( )
{
	printStr("\n\t==============================================");
	printStr("  \t=======| Welcome DeMeiDev 1.0 System |========");
	printStr("  \t==============================================\n");
}

bool DevicePlatform()
{
	bool result = FALSE;
	if((result = InitParament()) == TRUE){
		printStr("Parameters Configure Successfully!\n");
		if((result = DeviceInit()) == TRUE){
			printStr("Device Main Manage Controller Initialized Successfully!\n");
			if((result = SelfTestCaseInitialize()) == TRUE){
				printStr("Device Self-Test Successfully\n");
			}
			else{
				printStr("Self-test Initialized failure!!!");
				return FALSE;
			}
		}
		else{
			printStr("Paramate configure Initialized failure!!!");
			return FALSE;
		}
	}
	else{
		printStr("Device Paramater Init failure!!!");
		return FALSE;
	}
	return TRUE;
}

void main(int argc, char *argv[ ])
{
	InitGlobalValue();
	InitLogging();
	HardwareInitialize();
	bool result = DevicePlatform();
	if(result == FALSE)
		exit(0);

	pid_t pid_services;
	pid_services = fork();
	if(pid_services < 0){
		printf("!!!fork service layer progress error!!!");
		commandExecute("reboot","w");
	}
	else if(pid_services == 0){                     //child process(service layer)
		execl(SERVICES_PATH, SERVICES_NAME, NULL);
	}
	else{                                           //parent process(divice layer)
		sleep(5);
		Welcome();
		Pthread_Cond_Broadcast( );                  /*唤醒所有阻塞线程*/
		LoggingManage();                            /*主进程来进行运行日志的转储*/
	}
	return;
}

