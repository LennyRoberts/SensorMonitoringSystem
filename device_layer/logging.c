#include "logging.h"

loggingBuff *buff_1 = NULL;
loggingBuff *buff_2 = NULL;
loggingBuff *pull_buff = NULL;
loggingBuff *push_buff = NULL;

/* 对外函数：记录日志 */
void WriteLogging(struct timespec out_time, int location, int err_code)
{

}

/* 读取日志记录 */
void ReadLogging( )
{

}


/*日志文件系统初始化*/
void InitLogging()
{
	loggingBuff* log1 = (loggingBuff*)malloc(sizeof(loggingBuff));
	memset(log1->buff, 0, LOGBUFF_SIZE);
	log1->size = 0;
	log1->flag = 1;
	loggingBuff* log2 = (loggingBuff*)malloc(sizeof(loggingBuff));
	memset(log2->buff, 0, LOGBUFF_SIZE);
	log2->size = 0;
	log2->flag = 2;
	
	buff_1 = log1;
	buff_2 = log2;
	push_buff = buff_1;
	pull_buff = buff_2;
}

/*前台日志缓存是否存满*/
int IsFullLogBuff()
{
	if(push_buff->size == LOGBUFF_SIZE)
		return 1;
	else
		return 0;
}

int ExchangeBuff(loggingBuff* push)
{
	if(push->flag == 1){
		push_buff = buff_2;
		pull_buff = buff_1;
	}
	else if(push->flag == 2){
		push_buff = buff_1;
		pull_buff = buff_2;
	}

	/*检查交换是否成功*/
	if(push->flag == 1){
		if(push_buff == buff_2)
			return 1;
		else
			return 0;
	}
	else if(push->flag == 2){
		if(push_buff == buff_1)
			return 1;
		else
			return 0;
	}
}

/*写入日志文件*/
int WriteInLoggingFile()
{

}

/*日志文件系统管理程序*/
void LoggingManage()
{
	while(1){
		if(IsFullLogBuff()){
			int ex_state = ExchangeBuff(push_buff);
			if(ex_state){
				int num = WriteInLoggingFile();
				memset(pull_buff->buff, 0, LOGBUFF_SIZE);
				pull_buff->size = 0;
			}
		}
		sleep(1);
	}
}