#include "optimize.h"

int cycle_buff_num = 3;
float data_buff[MAX_BUFFER_SIZE];

/*获取参考数据及参数*/
void GetNavigationValue( )
{

}

/*依据标识码来获取传感器数据*/
void GetDataByCode(float buff[], unit_16 code, int level)
{
	int index = 0;
	while(level < cycle_buff_num){
		index = GetBuffIndexByCode(code, level);
		buff[level++] = data_buff[index];
	}
}

/*升序排序*/
void SortAscending(float buff[], int start, int len)
{
    int newtag=0, i=0;
	float tmp=0;
	if((start+len) > MAX_ITEM_SIZE)
		len = MAX_ITEM_SIZE-start;
	do{
        newtag=0;
		for(i=start+1; i<(start+len); i++){
			if(buff[i] < buff[i-1]){
				tmp = buff[i-1];
				buff[i-1] = buff[i];
				buff[i] = tmp;
                newtag=i;
            }
		}
		len = newtag;
	}while(newtag!=0);
}

/*降序排序*/
void SortDescending(float buff[], int start, int len)
{
	int newtag=0, i=0;
	float tmp=0;
	if((start+len) > MAX_ITEM_SIZE)
		len = MAX_ITEM_SIZE-start;
	do{
		newtag=0;
		for(i=start+1; i<(start+len); i++){
			if(buff[i] > buff[i-1]){
				tmp = buff[i-1];
				buff[i-1] = buff[i];
				buff[i] = tmp;
                newtag=i;
            }
		}
		len = newtag;
	}while(newtag!=0);
}


/*算术平均值*/
float ArithmeticAverage(sensor_item *item, float buff[])
{
	printf("[ %s ] current Use Algo is: Arithmetic_Average\n",item->item_name);
	SortAscending(buff, 0, cycle_buff_num);
	float result = 0;
	int i=0, start=0, len=cycle_buff_num;
	if(len >= 3){	//剔除最大值和最小值
		len--;start++;
	}
	for(i=start; i<len; i++){
		result += buff[i];
	}
	result /= len-start;
	return result;
}

/*获取最小值*/
float GetMinValue(sensor_item *item, float buff[])
{
	printf("[ %s ] current Use Algo is: Minimum_Value\n",item->item_name);
	SortAscending(buff, 0, cycle_buff_num);
	return buff[0];
}

/*获取最大值*/
float GetMaxValue(sensor_item *item, float buff[])
{
	printf("[ %s ] current Use Algo is: Maximum_Value\n",item->item_name);
	SortDescending(buff, 0, cycle_buff_num);
	return buff[0];
}

/*获取中位数*/
float GetMedianValue(sensor_item *item, float buff[])
{
	printf("[ %s ] current Use Algo is: Median_Value\n",item->item_name);
	SortAscending(buff, 0, cycle_buff_num);
	float result = buff[cycle_buff_num / 2];
	return result;
}


/*=================================算法优化三级级标识码(编号)分类器=================================*/
//0x33**
algo_func Optimize_A(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x34**
algo_func Optimize_B(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x35**
algo_func Optimize_C(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x36**
algo_func Optimize_D(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x37**
algo_func Optimize_E(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x38**
algo_func Optimize_F(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x39**
algo_func Optimize_G(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x40**
algo_func Optimize_H(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x41**
algo_func Optimize_I(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x42**
algo_func Optimize_J(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x43**
algo_func Optimize_K(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x44**
algo_func Optimize_L(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x45**
algo_func Optimize_M(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x46**
algo_func Optimize_N(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

//0x47**
algo_func Optimize_X(unit_16 order)
{
	switch(order){
		case ARITHMETIC_AVERAGE: //0x4701
			return ArithmeticAverage;
		case MINIMUM_VALUE:  //0x4702
			return GetMinValue;
		case MAXIMUM_VALUE:  //0x4703
			return GetMaxValue;
		case MEDIAN_VALUE:   //0x4704
			return GetMedianValue;
		default:
			return NULL;
	}
}

//0x48**
algo_func Optimize_Y(unit_16 order)
{
	switch(order){
		default:
			return NULL;
	}
}

/*----------------------------------------------------------------------------------------*/
/*算法优化二级标识码(类别)分类器*/
algo_func AlgoCodeClassify(unit_16 algo_code)
{
	int pre_algo_code = (int)(algo_code & 0xFF00);
	switch(pre_algo_code){
		case OPTIMIZE_A&0xFF00:
			return Optimize_A(algo_code);
		case OPTIMIZE_B&0xFF00:
			return Optimize_B(algo_code);
		case OPTIMIZE_C&0xFF00:
			return Optimize_C(algo_code);
		case OPTIMIZE_D&0xFF00:
			return Optimize_D(algo_code);
		case OPTIMIZE_E&0xFF00:
			return Optimize_E(algo_code);
		case OPTIMIZE_F&0xFF00:
			return Optimize_F(algo_code);
		case OPTIMIZE_G&0xFF00:
			return Optimize_G(algo_code);
		case OPTIMIZE_H&0xFF00:
			return Optimize_H(algo_code);
		case OPTIMIZE_I&0xFF00:
			return Optimize_I(algo_code);
		case OPTIMIZE_J&0xFF00:
			return Optimize_J(algo_code);
		case OPTIMIZE_K&0xFF00:
			return Optimize_K(algo_code);
		case OPTIMIZE_L&0xFF00:
			return Optimize_L(algo_code);
		case OPTIMIZE_M&0xFF00:
			return Optimize_M(algo_code);
		case OPTIMIZE_N&0xFF00:
			return Optimize_N(algo_code);
		case OPTIMIZE_X&0xFF00:
			return Optimize_X(algo_code);
		case OPTIMIZE_Y&0xFF00:
			return Optimize_Y(algo_code);
		default:
			return NULL;
	}
}

/*算法优化入口函数*/
float AlgoHandling(sensor_item *item, float *buff)
{
	sensor_item *ptr_item = item;
	float result = 0.0;
	int i = 0;
	for(i=0; i < ptr_item->num_algo_code; i++){
		algo_func algo_hand = NULL;
		algo_hand = AlgoCodeClassify(ptr_item->algo_code[i]);
		if(algo_hand == NULL) 
			continue;
		else{
			result = algo_hand(ptr_item, buff);
		}
	}
	return result;
}

/*数据单位转换*/
float UnitConversion(float result, float radio)
{
	result *= radio;
	return result;
}

/*限定值的范围*/
float LimitValue(float result, float min, float max)
{
	if(result > max)
		result = max;
	else if(result < min)
		result = min;
	return result;
}

/* 数据处理入口函数:
 * 1.对双缓存数据进行提取
 * 2.对数据进行数据算法优化
 * 3.将处理结果转存到最终缓存result_buff中*/
void DataItemHandling(sensor_item *item_head, float *buff)
{
	sensor_item *ptr_item = item_head;
	float tmp_buff[MAX_ITEM_SIZE];
	memset(tmp_buff, 0, MAX_ITEM_SIZE);        //缓存每次待计算的传感器子项数据
	memset(data_buff, 0, MAX_BUFFER_SIZE);
	memcpy(data_buff, buff, MAX_BUFFER_SIZE);  //缓存当前所有传感器子项数据
	printf("--------------------------------------------------\n");
	while(ptr_item != NULL){
		printf("[ %s ] origin data: ", ptr_item->item_name);
		int level=0, index=0;
		while(level < cycle_buff_num){
			index = GetBuffIndex(ptr_item->buff_ith,level);
			tmp_buff[level] = data_buff[index];
			printf("%s[%d]=%f; ", ptr_item->item_name, level, tmp_buff[level]);
			level++;
		}
		printf("\n");
		float result = 0.0;
		result = AlgoHandling(ptr_item, tmp_buff);
		result = UnitConversion(result,ptr_item->unit_ratio);
		result = LimitValue(result, ptr_item->minimum, ptr_item->maximum);
		printf("[ %s ] optimize result is : %.3f %s\n", ptr_item->item_name, result, ptr_item->unit_trans);
		SaveResult(ptr_item->buff_ith, result);
		ItemNext(&ptr_item);
	}
	printf("--------------------------------------------------\n");
}

/*--------------------------------------------#数据打包#--------------------------------------------*/
/*获取通信模块的各项状态信息,返回字节数*/
int GetGPRSInfo(unit_8 **src)
{
	int size = 0;
	return size;
}

/*获取GNSS各项状态信息，返回字节数*/
int GetGNSSInfo(unit_8 **src)
{
	int size = 0;
	return size;
}

/*获取电量等相关状态信息，返回字节数*/
int GetPowerInfo(unit_8 **src)
{
	int size = 0;
	return size;
}

/*获取采样时间*/
int GetMeasureTime(char *buff, time_t m_time, unit_16 time_response)
{
	time_t respnse = (time_t)time_response;
	m_time -= respnse;
	struct tm *timeval;
	timeval = localtime(&m_time);
	int len = 0;
	len += sprintf(buff,"%04d%02d%02d%02d%02d%02d",
	1900+timeval->tm_year,1+timeval->tm_mon,timeval->tm_mday,timeval->tm_hour,timeval->tm_min,timeval->tm_sec);
	return len;
}

/*将测量处理后的数据封装成自定义CJSON数据包*/
int JsonDataPackage(time_t *m_time)
{
	int i = 1;
	char node[4];
	sensor_type *ptr_type = TypeLinkHead();
	cJSON *obj_json = cJSON_CreateObject();
	char buff_devNo[32];
	memset(buff_devNo, 0, 32);
	GetDeviceNo(buff_devNo);
	cJSON_AddStringToObject(obj_json, DEVICE_NO, (const char *)(char *)buff_devNo);
	char buff_time[16];
	while(ptr_type != NULL){
		sensor_item *ptr_item = ptr_type->item_head;
		while(ptr_item != NULL){
			memset(node, 0, 4);
			snprintf(node, 4, "%s%02d", "s", i);
			cJSON *sensor_json = cJSON_CreateObject();
			cJSON_AddNumberToObject(sensor_json, SENSOR_CODE, ptr_item->sensor_code);
			float result = GetResult(ptr_item->buff_ith);
			cJSON_AddNumberToObject(sensor_json, SENSOR_DATA, (double)result);
			cJSON_AddStringToObject(sensor_json, SENSOR_FLAG, (const char*)(char*)&ptr_type->state);
			cJSON_AddStringToObject(sensor_json, SENSOR_UNIT, (const char*)(char*)&ptr_item->unit_trans);
			cJSON_AddItemToObject(obj_json, (const char *)node, sensor_json);
			ItemNext(&ptr_item);
		}
		TypeNext(&ptr_type);
		i++;
	}
	char *str = cJSON_Print(obj_json);
	SaveJsonData(str);
	cJSON_Delete(obj_json);
	return strlen(str);
}

/*modbus协议指令数据封装*/
int ModbusPacket()
{
	unit_8 *buff_modbus = (unit_8 *)malloc(sizeof(unit_8)*128);
	memset(buff_modbus, 0, 64);
	int len = 0;
	buff_modbus[len++] = CMD_HEAD;
	buff_modbus[len++] = DEV_FUNC_ISPC;
	int lenght = ItemTotalNum();
	buff_modbus[len++] = lenght * sizeof(int);
	sensor_type *ptr_type = TypeLinkHead();
	while(ptr_type != NULL){
		sensor_item *ptr_item = ptr_type->item_head;
		while(ptr_item != NULL){
			float value_f = GetResult(ptr_item->buff_ith);
			int value = (int)(value_f*DATA_DEVIDE);
			unit_8 bit24 = (unit_8)(value >> 24) & 0xFF;
			unit_8 bit16 = (unit_8)(value >> 16) & 0xFF;
			unit_8 bit8 = (unit_8)(value >> 8) & 0xFF;
			unit_8 bit0 = (unit_8)value & 0xFF;
			buff_modbus[len++] = bit24;
			buff_modbus[len++] = bit16;
			buff_modbus[len++] = bit8;
			buff_modbus[len++] = bit0;
			ItemNext(&ptr_item);
		}
		TypeNext(&ptr_type);
	}
	CRC_Generator(CRC16_MODBUS,LOW_BIT_FIRST,buff_modbus,len+2);
	SaveModbusData(buff_modbus,len+2);
	free(buff_modbus);
	return len;
}

/*算法优化程序初始化*/
void InitOptimize()
{
	memset(data_buff, 0, MAX_BUFFER_SIZE);
	cycle_buff_num = CycleBuffNum();
}