#include "sensor.h"



void AddtionalModuleInitialize( )
{
	printStr("Add-on Module Handling Initializing...");
	printStr("Add-on Module Handling Initialize Finish!\n");
}

void RW_ControllerInitialization( )
{

}

void SensorHandlingInitialize( )
{
	printStr("Sensor Handling Initializing...");
	RW_ControllerInitialization( );
	printStr("Sensor Handling Initialize Finish!\n");
}

/*---------------------------------------------------------------------------------------------------------*/
void Module_RW_Controller( )
{

}

/*---------------------------------------------------------------------------------------------------------*/
/*检查数据包头部*/
int CheckStartFlag(sensor_type *type,unit_8 *buff_replay)
{
	unit_8 buff[ 8 ];
	int nread = 0;
	unit_32 i = 0, j = 0, num_failure = 0;
	sensor_type *ptr_type = type;
	if(ptr_type == NULL){
		printStr("The sensor type is Empty!!!");
		return FALSE;
	}
	int fd_status = CheckFdStatus(type->fd_port, READ, 200);
	if(fd_status == FALSE){
		printStrStr(ptr_type->type_name, ":Fd Not Prepared!!!");
		return FALSE;
	}
	while(fd_status != FALSE){
		nread = read(ptr_type->fd_port, &buff_replay[ i ], 1);
		if(nread == FALSE && (errno == EINTR)){                /*errno == EINTR:表示读的过程中，遇到中断read()返回-1，同时errno=EINETR*/
			perror("Start flags");
			errno = 0; num_failure++;
			if(num_failure < NUM_FAILURE)
				continue;
			else{
				printStr("Exceeded the maximum number of failures!!!");
				FlushFdInOut(ptr_type->fd_port);
				return FALSE;
			}
		}
		else{
			if(buff_replay[ i ] == ptr_type->start_flag[ i ]){
				i++;
				if(i == ptr_type->len_startflag)
					return i;
			}
			else{
				j++;
				if(j <= 10){
					i = 0; continue;
				}else
					return FALSE;
			}
		}
	}
}

/*获取传感器回复数据*/
int GetReplyData(sensor_type *type, unit_8 *buff_reply)
{
	sensor_type *ptr_type = type;
	int result = ptr_type->len_startflag;
	int i = ptr_type->len_startflag;
	int nread = 0, num_failure = 0;
	int fd_status = CheckFdStatus(ptr_type->fd_port, WRITE, 200);
	while(fd_status != FALSE){
		nread = read(ptr_type->fd_port, &buff_reply[ i ], 1);
		if(nread <= 0){
			if((errno == EINTR || errno == EAGAIN)){
				//在非阻塞模式下，write或read返回-1，errno=EAGAIN表示相应的操作还没执行完成。
				//在阻塞模式下，设置SO_RCVTIMEO和SO_SNDTIMEO会导致read/write函数返回EAGAIN。
				perror("Start flags");
				errno = 0; num_failure++;
				if(num_failure < NUM_FAILURE) continue;
				else if(errno == EINTR){                                             /*读取过程中被系统调用中断则放弃当前数据*/
					FlushFdInOut(ptr_type->fd_port);
					result = FALSE; break;
				}
				else{
					printf("Exceeded the maximum number of failures!!!\n");
					errno = 0;
					FlushFdInOut(ptr_type->fd_port);
					result = FALSE; break;
				}
			}
			else{
				printf("Read Error!!!\n");
				result = FALSE; break;
			}
		}
		else{
			i += nread;
			result = i;
			if(result >= ptr_type->len_data)
				break;
		}
	}
	return result;
}

/*传感器数字量读写控制器*/
int32_t Digital_RW_Controller(sensor_type *type, unit_8 *buff_reply, status_rw rw_status, model_r r_modle)
{
	int32_t result = -1;
	sensor_type *ptr_type = type;
	int fail_counter = 0;
	if(rw_status == READ){
		if(ptr_type == NULL){
			printStr("Read Sensor Data: The Sensor_Type is Empty!");
			return result;
		}
		if(ptr_type->fd_port != INVALID_FD)
			FlushFdInOut(ptr_type->fd_port);
		else{
			printf("Sensor use fd is invalid!!!\n");
			return -1;
		}
		
		if(r_modle == READ_PASSIVE){	/*被动模式*/
			if(WriteCMD(ptr_type->type_name,ptr_type->fd_port,ptr_type->read_cmd,
						ptr_type->len_cmd,ptr_type->crc_module,ptr_type->crc_turn) == FALSE){
				printf("%s: Write read_cmd failed!!!", ptr_type->type_name);
				return -1;
			}
		}
		else if(r_modle != READ_ACTIVE){
			printf("Invalid read mode!!!\n");
			return -1;
		}

		READ_AGAIN:
		if(CheckStartFlag(ptr_type, buff_reply) != FALSE){
			result = GetReplyData(ptr_type, buff_reply);
			if(result == -1)
				printf("%s: Get reply data Failed!!!\n", ptr_type->type_name);
		}
		else{
			printf("%s: Get data start flag failed!!!\n", ptr_type->type_name);
			fail_counter++;
			if(fail_counter >= NUM_FAILURE){
				printf("Read over max num = %d\n", NUM_FAILURE);
				return -1;
			}
			else{
				printf("%s: Try again...\n", ptr_type->type_name);
				goto READ_AGAIN;
			}
		}

		if(result > 0){
			if(CRC_Check(type->crc_module, type->crc_turn, buff_reply, result) == FALSE){
				printf("%s: CRC check does not meet!!!\n", ptr_type->type_name);
				return -1;
			}
		}else
			return -1;
	}
	else if(rw_status == WRITE)
		WriteCMD(ptr_type->type_name,ptr_type->fd_port,ptr_type->read_cmd,
				ptr_type->len_cmd,ptr_type->crc_module,ptr_type->crc_turn);
	else
		printStr("Invalid read and write status!!!");
	return result;
}

void DigitalPreprocessing( )
{

}

/*----------------------------------------------------------------------------------------------------------*/
/*模拟量读写控制器*/
void Analog_RW_Controller( )
{

}

void AnalogPreprocessing( )
{

}

/*-----------------------------------#应急指令系统#----------------------------------------*/
/*应急指令比较器*/
emergency_cmd *EmergencyCmdComparator(sensor_type *type, unit_8 *buff_reply)
{
	emergency_cmd *ptr_emerResult = NULL;
	if(type == NULL)
		return ptr_emerResult;
	emergency_cmd *ptr_cmdSys = type->emer_head;
	while(ptr_cmdSys != NULL){
		int i = 0;
		for(i = ptr_cmdSys->start_byte; i < ptr_cmdSys->len_byte; i++){
			if(ptr_cmdSys->cmd[ i ] != buff_reply[ i ])
				break;
		}
		if((i - ptr_cmdSys->start_byte) == ptr_cmdSys->len_byte){
			ptr_emerResult = ptr_cmdSys;
			printStr("Capture action or error");
			break;
		}
		else
			ptr_cmdSys = ptr_cmdSys->next;
	}
	return ptr_emerResult;
}

/*硬件错误*/
void HardWareErrorHandling(sensor_type *type)
{
	SetSensorState(type, HARDWARE_ERROR);
	printStrStr(TypeName(type), "Hardware error!!!");
}

void DataErrorHandling(sensor_type *type)
{
	SetSensorState(type, DATA_ERROR);
	printStrStr(TypeName(type), "Data error!!!");
}

void ExceedMaxRangeHandling(sensor_type *type)
{
	SetSensorState(type, EXCEED_MAX_RANGE);
	printStrStr(TypeName(type), "Exceeds the maximum range!!!");
}

/*传感器应急指令处理*/
void SensorEmergencyHandling(sensor_type *type, int32_t emer_code)
{
	switch(emer_code){
		case HARDWARE_ERROR:
			HardWareErrorHandling(type);
			break;
		case DATA_ERROR:
			DataErrorHandling(type);
			break;
		case EXCEED_MAX_RANGE:
			ExceedMaxRangeHandling(type);
			break;
		default:
			printStr("Not emergency instruction!");
			break;
	}
}

/*-----------------------------------#数据处理与转发#--------------------------------------*/
/*缓存数据包解析为数值*/
float GetValue(unit_8 *buff, sensor_item *ptr_item)
{
	float result_float = 0.0;
	if(buff == NULL){
		printStr("The buffer data is empty!!!");
		return result_float;
	}
	if(ptr_item->len_byte > 4)
		printStr("The buffer data has exceeded 4Byte, The High byte will be truncated and discarded");
	char type = ptr_item->value_type;
	if(type == UNSIGNED_SHORT || type == UNSIGNED_INT){  //无符号整型数据解析
		result_float = (float)HexCharToUnsignedLong(buff,ptr_item->value_type,ptr_item->byte_order);
	}
	else if(type == SIGNED_SHORT || type == SIGNED_INT){ //有符号数数据解析
		result_float = (float)HexCharToSignedLong(buff, ptr_item->value_type, ptr_item->byte_order);
	}
	else if(type == FLOATING)
		result_float = HexToFloatPoint(buff);                       //单精度浮点型数据解析
	result_float = ptr_item->a * result_float + ptr_item->b;        //线性转换
	return result_float;
}

/*指令数据解析与储存*/
void CmdDataParser(dual_buff *push, unit_8 *buff, sensor_type *type, int level)
{
	sensor_type *ptr_type = type;
	unit_8 *ptr_buff = buff;
	if(ptr_type == NULL){
		printStr("Cmd data Parser: The Sensor_Type is empty!!!");
		return;
	}
	if(ptr_buff == NULL){
		printStr("Cmd data Parser: The Cmd_buffer is empty!!!");
		return;
	}
	sensor_item *ptr_item = ptr_type->item_head;
	float result = 0.0;
	int index=0, i=0;

	while(ptr_item != NULL){
		unit_8 buff_t[8];
		memcpy(buff_t, &ptr_buff[ ptr_item->start_byte ], ptr_item->len_byte);
		result = GetValue(buff_t, ptr_item);
		index  = GetBuffIndex(ptr_item->buff_ith, level);
		push->buff[index] = result;

		if(level+1 == CycleBuffNum()){
			index = GetBuffIndex(ptr_item->buff_ith, 0);
			push->m_time[index] = time(NULL);
		}
		ItemNext(&ptr_item);
	}
}


/*---------------------------------------------------------------------------------------------------------*/