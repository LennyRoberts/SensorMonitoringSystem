#include "command.h"

cmd_info *cmd_contents[MAX_CMD_NUMBER];  //指令缓存目录表

/*接收服务层unix socket客户端数据*/ 
int RecvCmd(int sock_fd, unit_8 *buf, int len)
{
	int size = 0;
	if(sock_fd != INVALID_FD)
		size = (int)recv(sock_fd, buf, len, 0);
	return size;
}

/*发送设备层unix socket服务端数据至服务层unix socket客户端*/
int SendData(int sock_fd, const char *buf, int len)
{
	int size = 0;
	if(buf != NULL){
		if(sock_fd != INVALID_FD)
			size = (int)send(sock_fd, buf, len, 0);
	}else
		printf("Send data is empty,can not send to!!!\n");
	return size;
}

/*指令校验*/
bool CmdCRCCheck(unit_8 *cmd, int len_cmd)
{
	unit_8 buf[15];
	memset(buf, 0, 15);
	len_cmd = len_cmd - 1; // 剔除尾部
	memcpy(buf, cmd, len_cmd);
	bool result = CRC_Check(CRC16_MODBUS, LOW_BIT_FIRST, buf, len_cmd);
	return result;
}

/*请求再次接收指令*/
unit_16 ReciveAgain(unit_16 num_th, int fd, char type)
{
	unit_16 tmp = 0;
	switch (num_th){
		case CMD_ERROR_NULL:
			tmp = CMD_ERROR_NULL; break;
		case CMD_VERSION_ERROR:
			tmp = CMD_VERSION_ERROR;break;
		case CMD_ERROR_CRC:
			tmp = CMD_ERROR_CRC; break;
		case CMD_ERROR_MODULE_FUNC:
			tmp = CMD_ERROR_MODULE_FUNC; break;
		case CMD_RETRANS:
			tmp = CMD_RETRANS; break;
		case CMD_FUNC_FAILE:
			tmp = CMD_FUNC_FAILE; break;
		case CMD_SUCCESS:
			tmp = CMD_SUCCESS; break;
		default:
			return CMD_ERROR_NULL;
	}
	char buff[2];
	buff[0] = (char)(tmp >> 8);
	buff[1] = (char)tmp & 0xFF;
	if(type == BUSINESS)
		SendData(fd, buff, 2);
	else{
		if(tmp != CMD_SUCCESS){
			FlushFdInOut(fd);
			write(fd, buff, 2);
		}
	}
	return tmp;
}

/*销毁指令缓存*/
bool DeleteCmdCache()
{
	int i = 0;
	for(i = 0; i < MAX_CMD_NUMBER; i++){
		if(cmd_contents[i] != NULL){
			free(cmd_contents[i]);
			cmd_contents[i] = NULL;
		}
	}
	for(i = 0; i < MAX_CMD_NUMBER; i++)
		if(cmd_contents[i] != NULL)
			return FALSE;
	return TRUE;
}

/*对指令进行排序*/
void AscendingSortCmd(cmd_info *contents[], int start, int len)
{
	int newtag = 0, i = 0;
	cmd_info *tmp = NULL;
	if((start+len) > MAX_CMD_NUMBER){
		printf("len overflow,len = MAX_BUFFER_SIZE-start!!!\n");
		len = MAX_BUFFER_SIZE - start;
	}
	do{
        newtag=0;
		for(i=start+1; i<(start+len); i++){
			if(contents[i]->func_code < contents[i-1]->func_code){
				tmp = contents[i-1];
				contents[i-1] = contents[i];
				contents[i] =tmp;
				newtag = i;
			}
		}
		len = newtag;
	}while(newtag!=0);
}

/*------------------------------------------------------------------
* 1.缓存指令至顺序表中,
* 2.仅保留指令中的有效字段(指令类别+地址+数据字段)
* 3.指令缓存在目录表中的位置是依据指令的索引进行存储的,
* 4.意味着指令的传输可以是乱序或跳序的,但不可超过最大指令限制
*-----------------------------------------------------------------*/
bool CmdTempCache(unit_8 *cmd, cmd_info *contents[])
{
	unit_8 *ptr_cmd = cmd;
	int i = 0, j = 0, num = 0;;
	for(i = 0,j=0; i < MAX_CMD_NUMBER; i++, j+=CMD_SIZE){
		if(ptr_cmd[j] == CMD_HEAD){
			if(ptr_cmd[j+TAIL_POS] != CMD_TAIL){
				printf("Incomplete instruction or missing tail!!!\n");
				return FALSE;
			}
			contents[i] = (cmd_info *)malloc(sizeof(cmd_info));
			if(contents[i] == NULL){
				printf("Cmd Cache malloc failed!!!\n");
				return FALSE;
			}
			contents[i]->module_addr = 0;
			contents[i]->func_code = 0;
			contents[i]->s_addr_1 = 0;
			contents[i]->s_addr_2 = 0;
			contents[i]->data = 0;
			contents[i]->flag = TRUE;

			printf("Cmd_Parse: ");
			/*提取模块地址*/
			contents[i]->module_addr = ptr_cmd[MODULE_POS] & 0xFF;
			printf("module_addr=0x%02X; ",contents[i]->module_addr);
			/*提取功能码字段*/
			contents[i]->func_code = ptr_cmd[FUNC_POS] & 0xFF;
			printf("func_code=0x%02X; ",contents[i]->func_code);
			
			int ii = 0;
			/*提取识别地址1*/
			for(ii = 0; ii < ADDR_1_SIZE; ii++)
				contents[i]->s_addr_1 |= (unsigned short)ptr_cmd[ADDR_POS_1+ii] << 8*(ADDR_1_SIZE-ii-1);
			printf("addr1:0x%04X; ",contents[i]->s_addr_1);
			
			/*提取识别地址2*/
			for(ii = 0; ii < ADDR_2_SIZE; ii++)
				contents[i]->s_addr_2 |= (unsigned short)ptr_cmd[ADDR_POS_2+ii] << 8*(ADDR_2_SIZE-ii-1);
			printf("addr2:0x%04X; ",contents[i]->s_addr_2);
			
			/*提取指令数据字段*/
			int tmp_data = 0;
			for(ii = 0; ii < DATA_SIZE; ii++)
				tmp_data |= (int)cmd[ii + DATA_POS] << 8*(DATA_SIZE-ii-1);
			contents[i]->data = (float)tmp_data / DATA_DEVIDE;
			printf("cmd_data=%.3f\n", contents[i]->data);

			if(contents[i] == NULL){
				printf("Cmd cache failed!!!\n");
				return FALSE;
			}
			num++;
		}
	}
	AscendingSortCmd(contents, 0, num);
	return TRUE;
}

/*------------------------------------指令执行函数调用函数------------------------------------------*/
/*查找到该传感器*/
sensor_type *GetItemInfo(unit_32 sensor_code)
{
	sensor_type *ptr_type = TypeLinkHead();
	while(ptr_type != NULL){
		sensor_item *ptr_item = ptr_type->item_head;
		while(ptr_item != NULL){
			if(ptr_item->sensor_code == sensor_code)
				return ptr_type;
			ItemNext(&ptr_item);
		}
		TypeNext(&ptr_type);
	}
	return NULL;
}




/*-------------------------------------指令执行函数-----------------------------------------------*/
/*取消设备即将执行或当前未完成的所有操作*/
bool CancleAllOperation( )
{
	return TRUE;
}

/*获取modbus数据*/
bool ReplyModbusData(unsigned short addr1, unsigned short addr2, float data, int fd)
{
	printf("Get Sensor data with Modbus\n");
	char buff[MAX_MODBUS_BUFFER_SIZE];
	memset(buff,0,MAX_MODBUS_BUFFER_SIZE);
	int len = GetModbusData(buff);
	FlushFdInOut(fd);
	len = write(fd, buff, len);
	if(len <= 0){
		printf("Send data failed!!! please try again...\n");
		return TRUE;
	}
	else{
		printHexStr("Reply data:", buff, len);
		return FALSE;
	}
}

/*获取全部数据:将当前所有传感器数据及信息以传感器为单位发送至服务层和本地服务模块*/
int GetAllSensorDataJson(int fd)
{
	printf("Get all sensor data...\n");
	char buff[MAX_CJSON_BUFFER_SIZE];
	memset(buff,0,MAX_CJSON_BUFFER_SIZE);
	int len = 0;
	len = GetJsonPackage(buff);
	len = SendData(fd, buff, len);
	if(len <= 0)
		printf("Send Json data failed!!! please try again...\n");
	return len;
}

/*---------------------------------------------------------------------------------*/
/*数据回复*/
bool ReplayData(unsigned short addr1, unsigned short addr2, float data, int fd)
{
	if(addr1 == GET_ALL_DATA || addr2 == GET_ALL_DATA)
		GetAllSensorDataJson(fd);
}

/*---------------------------------------------------------------------------------*/

/*核心设备模块指令类型分类检查*/
func_cmd CmdFuncCodeModuleDevice(unit_8 func_code)
{
	switch(func_code){
		case DEV_FUNC_CALIBRATE:                  /*数据校正类型*/
			return NULL;
		case DEV_FUNC_DATA:                       /*数据请求类型*/
			return &ReplayData;
		case DEV_FUNC_ISPC:                       /*串口间通信*/
			return &ReplyModbusData;
		case DEV_DUNC_EXEUTE:                     /*设备执行类型*/
			return NULL;
		default:
			printf("Invalid Cmd Function Byte!!!\n");
			return NULL;
 	};
}

/*本地服务模块指令类型分类检查*/
func_cmd CmdFuncCodeModuleLocal(unit_8 func_code)
{
	switch(func_code){
		
		default:
			printf("Invalid Cmd Function Byte!!!\n");
			return NULL;
	}
}

/*触摸屏模块指令类型分类检查*/
func_cmd CmdFuncCodeModuleLCD(unit_8 func_code)
{
	switch(func_code){
		
		default:
			printf("Invalid Cmd Function Byte!!!\n");
			return NULL;
	}
}

/*模块地址分区*/
func_cmd CmdModulePartition(unit_8 module_addr, unit_8 func_code)
{
	switch(module_addr){
		case MODULE_DEVICE:
			return CmdFuncCodeModuleDevice(func_code);
		case MODULE_LOCAL:
			return CmdFuncCodeModuleLocal(func_code);
		case MODULE_LCD:
			return CmdFuncCodeModuleLCD(func_code);
		default:
			printf("Invalid Cmd module partition!!!\n");
			return NULL;
	}
}

/*指令分管器*/
unit_16 CmdManage(unit_8 *cmd, int fd, char type)
{
	unit_8 *cmd_ptr = cmd;
	bool result = FALSE;

	if(cmd_ptr == NULL)
		return ReciveAgain(CMD_ERROR_NULL, fd, type);

	if(cmd_ptr[VERSION_POS] != DEVICE_VERSION)
		return ReciveAgain(CMD_VERSION_ERROR,fd,type);

	if(type == ISPC){
		result = CmdCRCCheck(cmd_ptr, 16);
		if(result != TRUE){    /*指令校验错误则需要重新接收*/
			printf("CMD CRC cheack failed!!!\n");
			return ReciveAgain(CMD_ERROR_CRC, fd, type);
		}
	}

	result = CmdTempCache(cmd_ptr, cmd_contents);
	if(result != TRUE){
		printf("CMD temp cache error!!!\n");
		return ReciveAgain(CMD_ERROR_CACHE, fd, type);
	}

	func_cmd func_ptr = NULL;	
	int i = 0;
	for(i = 0; i < MAX_CMD_NUMBER; i++){
		if(cmd_contents[i] != NULL){
			func_ptr = CmdModulePartition(cmd_contents[i]->module_addr, cmd_contents[i]->func_code);
			if(func_ptr != NULL){
				result = func_ptr(cmd_contents[i]->s_addr_1,cmd_contents[i]->s_addr_2,cmd_contents[i]->data, fd);
				if(result == FALSE)
					return ReciveAgain(CMD_FUNC_FAILE, fd, type);
				else{
					cmd_contents[i]->flag = FALSE;    /*本条指令已执行标记*/
					return ReciveAgain(CMD_SUCCESS, fd, type);
				}
			}
			else{    /*当组合指令中的一条指令执行出错时，立即放弃该组合指令的执行，并要求重传指令*/
				CancleAllOperation();
				return ReciveAgain(CMD_ERROR_MODULE_FUNC, fd, type);
			}
		}
	}
	return result;
}

/*指令管理系统初始化*/
bool CMDManageInit()
{
	int i = 0;
	for(i = 0; i < MAX_CMD_NUMBER; i++)
		cmd_contents[i] = NULL;
}