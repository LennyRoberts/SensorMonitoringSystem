#include "spl_manage.h"

unit_8 buff_reply[ MAX_REPLAY_CMD_SIZE ];       /*传感器/模块应答指令数据包缓存*/
int time_wait = 0;                              /*传感器读间隔时间*/

void PLManageInit( )
{
	memset(buff_reply, 0, 64);
	time_wait = 2 * 1000 / TypeTotalNum();
}

/*==============================================================================================*/
/*传感器感知层管理程序*/
void *PerceiveLayerManage(void *arg)
{
	pthread_t tid;
	tid = pthread_self( );
	printf("pthread for perceive Layer: tid= %lld (0x%llX)\n", (unit_64)tid, (unit_64)tid);

	printStr("Enter The Perceive Layer Manage Programme");
	SensorHandlingInitialize( );
	PLManageInit( );	
	pthread_mutex_lock(&lock_PL);
	wait_PL_status = TRUE;
	while(IsWait(wait_PL_status))
		pthread_cond_wait(&pthread_cond_PL, &lock_PL);
	wait_PL_status = FALSE;
	pthread_mutex_unlock(&lock_PL);
	sleep(1);
	
	sensor_type *ptr_type = TypeLinkHead();
	int32_t read_result = FALSE;
	int level = 0;	
	while(1){
		if(IsEmptyType(ptr_type)){
			TypeHead(&ptr_type);
			level++;
			if(IsOverBuffer(level)){
				pthread_mutex_trylock(&lock_AL);
				level = 0;
				if(IsWait(wait_AL_status)){
					wait_AL_status = FALSE;
					pthread_cond_signal(&pthread_cond_AL);
					printStr("Buffer full && wake up pthread_AL!");
				}else
					printStr("Pthread_AL timeout wake up!");
				pthread_mutex_unlock(&lock_AL);
			}
		}
		read_result = Digital_RW_Controller(ptr_type, buff_reply, READ, ptr_type->read_mode);
		if(read_result != -1){
			print_str(TypeName(ptr_type));
			printHexStr("Reply Command Packet=", buff_reply, read_result);
			emergency_cmd *ptr_cmdSys = EmergencyCmdComparator(ptr_type, buff_reply);
			if(ptr_cmdSys != NULL)
				SensorEmergencyHandling(ptr_type, ptr_cmdSys->emer_code);
			dual_buff* buffer_push = BuffPush();
			CmdDataParser(buffer_push, buff_reply, ptr_type, level);
		}else
			printStrStr(TypeName(ptr_type), "Unable to get sensor data!!!");
		TypeNext(&ptr_type);
		sleep(TIME_INTERVAL);
	}
}
