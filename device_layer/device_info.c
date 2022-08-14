#include "device_info.h"

/*用于控制各个线程的变量*/
/*--------------------------------------------------*/
pthread_cond_t pthread_cond_PL; //感知层线程
pthread_mutex_t lock_PL;
bool wait_PL_status;
pthread_cond_t pthread_cond_IPC1; //进程间通信1线程
pthread_mutex_t lock_IPC1;
bool wait_IPC1_status;
pthread_cond_t pthread_cond_AL; //系统层数据处理线程
pthread_mutex_t lock_AL;
bool wait_AL_status;
pthread_cond_t pthread_cond_LM; //本地模块线程
pthread_mutex_t lock_LM;
bool wait_LM_status;
pthread_cond_t pthread_cond_ISPC; //串口间通信线程
pthread_mutex_t lock_ISPC;
bool wait_ISPC_status;


/*以下全局变量保存设备运行的各项信息，仅可通过函数方法进行访问或通过函数方法进行部分修改*/
/*------------------------------------------------------------------------*/
device_para *device_para_ptr = NULL;              // 设备信息节点入口
sensor_type *type_link_head = NULL;               // 传感器类别信息链表入口
serial_port* s_port[SERIAL_POER_NUM];             // 串口信息

dual_buff *buff_ptr_one = NULL;                   // 双缓存机制缓存器1入口
dual_buff *buff_ptr_two = NULL;                   // 双缓存机制缓存器2入口
dual_buff *buff_push = NULL;                      // 向缓冲区推送数据(双缓存机制)
dual_buff *buff_pull = NULL;                      // 拉取缓冲区内数据(双缓存机制)
float result_buff[MAX_ITEM_SIZE];                 // 设备层最终计算数据缓存
char  cjson_buff[MAX_CJSON_BUFFER_SIZE];          // CJSON格式数据包缓存
unit_8 modbus_buff[MAX_MODBUS_BUFFER_SIZE];       // ISPC缓存
int exchange_overtime = 0;                        // 交换缓冲区超时时间
put_print out;

/*=========================================#Get Global Value#=====================================================*/
/*全局变量初始化*/
void InitGlobalValue(){
	device_para_ptr = NULL;
	type_link_head = NULL;
	buff_ptr_one = NULL;
	buff_ptr_two = NULL;
	buff_push = NULL;
	buff_pull = NULL;
	exchange_overtime = 0;
	int i = 0;
	for(i=0; i<SERIAL_POER_NUM; i++)
		s_port[i] = NULL;
	memset(result_buff, 0.0,MAX_BUFFER_SIZE);
	memset(cjson_buff, 0, MAX_CJSON_BUFFER_SIZE);
	memset(modbus_buff, 0, MAX_MODBUS_BUFFER_SIZE);
}

device_para *DeviceParaPtr() { return device_para_ptr; }                               /*设备信息结构体*/
sensor_type *TypeLinkHead() { return type_link_head; }                                 /*传感器信息链表头节点*/
serial_port **SerialPort() { return s_port; }                                          /*串口信息链表头节点*/

dual_buff *BuffPtrOne() { return buff_ptr_one; }                           /*双缓存机制缓存2*/
dual_buff *BuffPtrTwo() { return buff_ptr_two; }                           /*双缓存机制缓存2*/
dual_buff *BuffPush() { return buff_push; }                                /*双缓存机制中面向数据处理的地址*/
dual_buff *BuffPull() { return buff_pull; }                                /*双缓存机制中面向传感器转储的地址*/
int TypeTotalNum() { return DeviceParaPtr()->num_type; }                   /*传感器类别总数*/
int ItemTotalNum() { return DeviceParaPtr()->num_item; }                   /*传感器项目总数*/
int ExchangeOverTime() { return exchange_overtime; }                       /*双缓存机制交换超时时间*/
int CycleBuffNum() { return device_para_ptr->cycle_buff_num; }             /*获取双缓存机制传感器存储次数*/

sensor_type* TypeHead(sensor_type **type){*type = type_link_head;return (*type);}      /*指向传感器类别链表头节点*/
sensor_type* TypeNext(sensor_type **type){*type = (*type)->next;return (*type);}       /*指向传感器类别链表下一节点*/
unit_8* TypeName(sensor_type *type) { return type->type_name; }
unit_8* ReadCmd(sensor_type *type) { return type->read_cmd; }
unit_32 LenCmd(sensor_type *type) { return type->len_cmd; }
unit_32 LenData(sensor_type *type) { return type->len_data; }
unit_8* DataStartFlag(sensor_type *type) { return type->start_flag; }
unit_32 LenStartFlag(sensor_type *type) { return type->len_startflag; }
unit_32 PortIth(sensor_type *type) { return type->port_ith; }
int     FdPort(sensor_type *type) { return type->fd_port; }
unit_16 CRCModule(sensor_type *type) { return type->crc_module; }
unit_16 CRCTurn(sensor_type *type) { return type->crc_turn; }
unit_32 SensorState(sensor_type *type) { return type->state; }
sensor_item* ItemHead(sensor_type *type) { if(type != NULL)return type->item_head;else return NULL; }
emergency_cmd* EmergencyCmd(sensor_type *type) { return type->emer_head; }

unit_8 *ItemName(sensor_item *item) { return item->item_name; }
unit_8 *ItemUnit(sensor_item *item) { return item->unit_trans; }
int ItemDataCode(sensor_item *item) { return item->sensor_code; }
unit_16* ItemAlgoCode(sensor_item *item) { return item->algo_code; }
sensor_item *ItemNext(sensor_item **item){*item = (*item)->next;return (*item); }

void GetDeviceNo(char *buff){unit_8 *str_no=device_para_ptr->version->dev_no; memcpy(buff,str_no,strlen(str_no));} /*获取设备编号*/

/*=======================================#Change Global Value#==============================================*/
// bool SetPortLinkHead(serial_port **ptr_src) { port_link_head = *ptr_src; if(port_link_head == NULL)return FALSE;else return TRUE;}    /*设置串口信息链表的头节点*/
bool SetTypeLinkHead(sensor_type **ptr_src) { type_link_head = *ptr_src; if(type_link_head == NULL)return FALSE;else return TRUE;}    /*设置传感器类别链表的头节点*/
bool SetDeviceParaPtr(device_para **ptr_src) { device_para_ptr = *ptr_src; if(device_para_ptr == NULL)return FALSE;else return TRUE;} /*设置设备信息的结构地址*/

void SetBuffOne(dual_buff *ptr_src) { buff_ptr_one = ptr_src; }           /*设置双缓存机制中缓存器1的入口*/
void SetBuffTwo(dual_buff *ptr_src) { buff_ptr_two = ptr_src; }           /*设置双缓存机制中缓存器2的入口*/
void ChangeBuffPush(dual_buff *ptr_src) { buff_push = ptr_src; }          /*改变双缓存机制的Push缓存指向*/
void ChangeBuffPull(dual_buff *ptr_src) { buff_pull = ptr_src; }          /*改变双缓存机制的Pull缓存指向*/
void SetTypeTotalNum(int src) { device_para_ptr->num_type = src; }        /*设置传感器种类数量*/
void SetItemTotalNum(int src) { device_para_ptr->num_item = src; }        /*设置传感器总数数量*/
void SetExchangeOverTime(int src) { exchange_overtime = src; }            /*设置双缓存机制交换超时时间*/
void SetSensorState(sensor_type *type, unit_32 state){                    /*设置传感器的工作状态*/
	if(state == HARDWARE_ERROR) type->state = 1;
	else if(state == DATA_ERROR) type->state = 2;
	else if(state == EXCEED_MAX_RANGE) type->state = 3;
}

/*===============================================================================*/
#if PRINT_ALL	/* Custom system debug print function wrapper */
/*-------------------------------------------------------------------------------*/
void printStylef(char *buf, float data){printf(buf, data);}
void printStyle(char *buf, int data){printf(buf, data);}

void printStr(char *str){printf("%s\n", str);}
void printHex(const unsigned char *buf, unit_32 len){
	unit_32 i;
	printf("Hex[ ");
	for(i = 0; i < len; i++) printf("%02X ", buf[ i ]);
	printf("]\n");
}

void printHexStr(unsigned char *str, const unsigned char *buf, unit_32 len){
	unit_32 i;
	printf("%s:Hex[ ", str);
	for(i = 0; i < len; i++) printf("%02X ", buf[ i ]);
	printf("]\n");
}

void printDec(char *str, int value){printf("%s:%d\n", str, value);}
void printFloat(char *str, float value) {printf("%s:%f\n", str, value);}
void printDec_unit_8(const char *buf, unit_16 len){
	unit_16 i;
	for(i = 0; i < len; i++) 
		printf("%d ", buf[ i ]);
	printf("\n");
}

void print_str(char *str){printf("%s: ", str);}
void printStrStr(char *str1, char *str2){printf("%s:%s", str1,str2);}
void Perror(const char *str){perror(str);}

#endif /*PRINT*/

/*===============================================================================*/
/*指令+标志 =管道=> 写入内核*/
bool commandExecute(char *cmd, char *isRead)
{
	FILE *fp; bool result;
	fp = popen(cmd, isRead);
	if(fp == NULL) result = FALSE;
	else{ pclose(fp); result = TRUE; }
	return result;
}

void Reboot(){commandExecute("reboot -f", "r");}/*设备重启*/

/*整型转为IP点分十进制显示*/
char *Int2Net(char* net_buff, int net_int)
{
	memset(net_buff, 0, sizeof(net_buff));
	int net = 0, str_len = 0;
	net = (net_int >> 24) & 0x000000FF;
	str_len += snprintf(net_buff, 4 + 1, "%d%c", net, '.');
	net = (net_int >> 16) & 0x000000FF;
	str_len += snprintf(net_buff + str_len, 4 + 1, "%d%c", net, '.');
	net = (net_int >> 8) & 0x000000FF;
	str_len += snprintf(net_buff + str_len, 4 + 1, "%d%c", net, '.');
	net = net_int & 0x000000FF;
	str_len += snprintf(net_buff + str_len, 3 + 1, "%d", net);
	return net_buff;
}

/*无符号char型数据解析*/
unsigned int HexCharToUnsignedLong(unit_8 *buff, char type, unit_16 byte_order)
{
	unit_8 *ptr_buff = buff;
	unsigned int result = 0;
	if(ptr_buff == NULL){
		printStr("The buffer data is empty!!!");
		return result;
	}
	int num_byte = 0;
	if(type == UNSIGNED_SHORT || type == SIGNED_SHORT)
		num_byte = 2;
	else if(type == UNSIGNED_INT || type == SIGNED_INT)
		num_byte = 4;

	unit_16 order = byte_order;
	result = *ptr_buff;
	int i = 0;
	for(i = 1; i < num_byte; i++){
		result = result << 8;
		ptr_buff++;
		result |= *ptr_buff;
	}

	unsigned int byte_D = result & 0x000000FF; //D
	unsigned int byte_C = result >> 8;
	byte_C &= 0x000000FF;                      //C
	unsigned int byte_B = result >> (8*2);
	byte_B &= 0x000000FF;                      //B
	unsigned int byte_A = result >> (8*3);
	byte_A &= 0x000000FF;                      //A
	if(type == UNSIGNED_INT || type == SIGNED_INT){
		if(order == ORDER_ABCD)
			return result;
		else if (order == ORDER_CDAB){
			result = 0;
			result |= byte_C << (8*3);
			result |= byte_D << (8*2);
			result |= byte_A << 8;
			result |= byte_B;
			return result;
		}
		else if (order == ORDER_BADC){
			result = 0;
			result |= byte_B << (8*3);
			result |= byte_A << (8*2);
			result |= byte_D << 8;
			result |= byte_C;
			return result;
		}
		else if (order == ORDER_DCBA){
			result = 0;
			result |= byte_D << (8*3);
			result |= byte_C << (8*2);
			result |= byte_B << 8;
			result |= byte_A;
			return result;
		}
	}
	else if(type == UNSIGNED_SHORT || type == SIGNED_SHORT){
		if(order == ORDER_ABCD)
			return result;
		else{
			result = 0;
			result |= byte_D << 8;
			result |= byte_C;
			return result;
		}
	}
	return result;
}

/*有符号char型数据解析*/
int HexCharToSignedLong(unit_8 *buff, char type, unit_16 byte_order)
{
	unit_8 *ptr_buff = buff;
	unsigned int value = HexCharToUnsignedLong(buff, type, byte_order);
	if(type == SIGNED_SHORT){
		if(value >= 0x8000)
			value = 0 - (value & 0x7FFF);
	}
	else if(type == SIGNED_INT){
		if(value >= 0x80000000)
			value = 0 - (value & 0x7FFFFFFF);
	}
	int result = (int)value;
	return result;
}

union FLOAT
{
	unsigned char byte[4];
	float f_data;
};

/*IEEE_754 浮点型数据解析器*/
float HexToFloatPoint(unit_8 *buff)
{
	// unsigned int number = HexCharToUnsignedLong(buff, UNSIGNED_INT, ORDER_ABCD);
	// int sign = (number & 0x80000000) ? -1 : 1;
	// int exponent = ((number >> 23) & 0xFF) - 127;
	// float val = (float)(number & 0x7FFFFF);
	// float mantissa = 1 + (val / 0x7FFFFF);					/*二进制小数转化为十进制小数*/
	// float result = sign * mantissa * pow(2, exponent);
	union FLOAT value;
	value.byte[3] = buff[0];
	value.byte[2] = buff[1];
	value.byte[1] = buff[2];
	value.byte[0] = buff[3];
	float result = value.f_data;
	return result;
}

void FlushFdIn(int fd) { tcflush(fd, TCIFLUSH); }           /*清空串口输入队列*/
void FlushFdOut(int fd) { tcflush(fd, TCOFLUSH); }          /*清空串口输出队列*/
void FlushFdInOut(int fd) { tcflush(fd, TCIOFLUSH); }       /*清空串口输入输出队列*/

/*CRC校验检查*/
bool CRC_Check(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len){
	int result = __CRC_Check(crc_name, storage_mode, data, len);
	if(result == 1) return TRUE;
	else if(result == -1)return FALSE;
	else printf("Crc check function error!!!");
}

/* Description: CRC校验值产生函数，修改原指令的校验值
 * 参数：crc_name[校验模型名称]，storage_mode[校验值存储方式(高位在前/低位在前)]，
 * *data[指向要修改的指令指针]，len[指令长度]*/
void CRC_Generator(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len){
	__CRC_Generator(crc_name, storage_mode, &data, len);
}

/*I/O多路复用poll，超时检测当前文件描述符是否准备好*/
int CheckFdStatus(int fd, status_rw rw_status, int32_t overtime_ms)
{
	int ret = 0;
	struct pollfd fds[1];
	if(fd == INVALID_FD)
		return FALSE;
	fds[0].fd = fd;                    /*仅监听当前串口*/
	if(rw_status == WRITE)
		fds[0].events = POLLOUT;
	else
		fds[0].events = POLLIN;
	ret = poll(fds, 1, overtime_ms);   /*I/O多路复用*/
	if(ret == FALSE)
		perror("Poll");
	else if(ret == 0){
		ret = FALSE;
		printf("Poll error or timeout, Fd status does not change!!!\n");
	}
	else if(ret > 0){
		if(rw_status == WRITE){
			if((fds[0].revents & POLLOUT) == POLLOUT)
				ret = TRUE;
		}
		else{
			if((fds[0].revents & POLLIN) == POLLIN)
				ret = TRUE;
		}
	}
	return ret;
}

/*I/O多路复用select，超时检测文件描述符是否准备好*/
// int CheckFdStatus(int fd, status_rw rw_status, int32_t overtime_us)
// {
// 	int max_fd = -1;
// 	int retv = FALSE;
// 	fd_set fdSet;
// 	struct timeval ts;
// 	ts.tv_sec  = TIME_S_FD_NOT_AVAILABLE;             //TIME_S_FD_NOT_AVAILABLE=0
// 	ts.tv_usec = TIME_FD_NOT_AVAILABLE * overtime_us; //TIME_FD_NOT_AVAILABLE=500000( 0.5*n S)
// 	sensor_type *ptr_type = TypeLinkHead();
// 	FD_ZERO(&fdSet);
// 	while(ptr_type != NULL){
// 		if(ptr_type->fd_port != INVALID_FD){
// 			FD_SET(ptr_type->fd_port, &fdSet);
// 			if(ptr_type->fd_port > max_fd)
// 				max_fd = ptr_type->fd_port;
// 		}
// 		TypeNext(&ptr_type);
// 	}
// REDO_SELECT:
// 	if(rw_status == READ)
// 		retv = select(max_fd + 1, &fdSet, NULL, NULL, &ts);    //监视读
// 	else
// 		retv = select(max_fd + 1, NULL, &fdSet, NULL, &ts);    //监视写
// 	if(retv <= 0){
// 		if(errno == EINTR){                                    //达到超时时间等就不会出现中断信号了
// 			errno = 0; goto REDO_SELECT;
// 		}
// 		else if(retv == FALSE)
// 			perror("I/O Select error");
// 		retv = FALSE;
// 	}
// 	else{
// 		ptr_type = TypeLinkHead();
// 		while(ptr_type != NULL){
// 			if(FD_ISSET(fd, &fdSet) != 0){
// 				retv = ptr_type->port_ith;
// 				break;
// 			}else
// 				retv = FALSE;
// 			TypeNext(&ptr_type);
// 		}
// 	}
// 	return retv;
// }


/*写入指令*/
int32_t WriteCMD(unit_8 *name, int fd, unit_8 *cmd, int len_cmd, unit_16 CRC_module, unit_16 CRC_turn)
{
	if(fd == INVALID_FD){
		printf("Invalid fd!!!\n");
		return -1;
	}
	int result = 0;
	unit_8 cmd_buff[32];
	memset(cmd_buff, 0, 32);
	if(CheckFdStatus(fd, WRITE, 1000) != FALSE){
		memcpy(cmd_buff, cmd, len_cmd);
		CRC_Generator(CRC_module, CRC_turn, cmd_buff, len_cmd);
		FlushFdInOut(fd);
		result = write(fd, cmd_buff, len_cmd);
	}else
		printf("%s: Fd not ready!!!\n", name);
	if(result == len_cmd){
		printf("%s write in %dByte cmd: ",name, result);
		printHex(cmd, len_cmd);
	}
	if(result == -1 || result != len_cmd){
		result = -1;
		printf("%s write cmd failed!!!\n", name);
	}
	return result;
}

int IsEmptyType(sensor_type *type) {if(type == NULL) return 1;else return 0;}         /*判断传感器类型结构是否为空*/
int IsEmptyItem(sensor_item *item) {if(item == NULL) return 1;else return 0;}         /*判断参数信息结构是否为空*/
int IsOverBuffer(int hash_level){if((hash_level+1) > CycleBuffNum()) return 1;else return 0;} /*缓存越界判断，双缓存机制中是否存满*/

/*获取数据缓存的索引*/
int GetBuffIndex(int buff_ith, int level)
{
	int index = 0;
	int item_num = ItemTotalNum();
	index = buff_ith;
	if((level+1) * item_num <= MAX_BUFFER_SIZE)
		index += level * item_num;
	return index;
}

/*通过标识码获取数据缓存的索引,返回-1时表示找不到该标识码，在调用时注意检查*/
int GetBuffIndexByCode(unit_16 sensor_code, int level)
{
	int index = -1;
	sensor_type *ptr_type = TypeLinkHead();
	while(ptr_type != NULL){
		sensor_item *ptr_item = ptr_type->item_head;
		while(ptr_item != NULL){
			if(ptr_item->sensor_code == sensor_code){
				index = GetBuffIndex(ptr_item->buff_ith, level);
				return index;
			}
			ItemNext(&ptr_item);
		}
		TypeNext(&ptr_type);
	}
	return index;
}

float GetResult(int buff_pos){int index = GetBuffIndex(buff_pos, 0);return result_buff[index];}  /*获取最终值*/
void  SaveResult(int buff_ith, float result){int index = GetBuffIndex(buff_ith,0);result_buff[index] = result;}  /*存储最终值*/
int   GetJsonPackage(char *str){if(cjson_buff != NULL){ memcpy(str, cjson_buff,strlen(cjson_buff));return strlen(cjson_buff);}else return 0;} /*获取Json格式数据*/
void  SaveJsonData(char *str){if(str != NULL){memset(cjson_buff,0,MAX_CJSON_BUFFER_SIZE); memcpy(cjson_buff,str,strlen(str));}}               /*保存CJSON格式数据*/
int   GetModbusData(char *str){if(modbus_buff!=NULL){int len=(int)modbus_buff[2]+5; memcpy(str,modbus_buff,len);return len;}else return 0;}   /*获取封装的modbus数据*/
void  SaveModbusData(char *str,int len){if(str!=NULL){memset(modbus_buff,0,MAX_MODBUS_BUFFER_SIZE);memcpy(modbus_buff,str,len);}}             /*储存封装的modbus指令数据*/
void  InitResultBuff(){memset(result_buff, 0, MAX_BUFFER_SIZE);}                 /*重置化最终值缓存*/
int   IsWait(bool wait_status){if(wait_status == TRUE) return 1;else return 0;}  /*线程是否处于阻塞状态*/