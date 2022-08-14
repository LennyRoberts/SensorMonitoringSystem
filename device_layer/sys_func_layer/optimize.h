#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
/*===============================================================================
* 数据算法优化处理程序：
* 对测量数据进行算法的优化，计算与统计等
* ==============================================================================*/
#include "../logging.h"
#include "../device_info.h"

typedef float (*algo_func)(sensor_item *item, float buff[]);/*算法优化函数指针/函数原型*/

void InitOptimize();                                            /*算法优化程序初始化*/
void DataItemHandling(sensor_item *item_head, float *buff);     /*数据处理入口函数*/
float AlgoHandling(sensor_item *item, float *buff);             /*算法优化入口函数*/
algo_func AlgoCodeClassify(unit_16 algo_code);                  /*算法优化二级标识码(类别)分类器*/
void GetNavigationValue( );                                     /*获取参考数据及参数*/
void GetDataByCode(float buff[], unit_16 code, int level);      /*依据标识码来获取传感器数据*/
int  JsonDataPackage(time_t *m_time);                           /*将测量处理后的数据封装成自定义CJSON数据包*/
int ModbusPacket();                                             /*modbus协议指令数据封装*/
int GetMeasureTime(char *buff, time_t m_time, unit_16 time_response);/*获取采样时间*/
float UnitConversion(float result, float radio);                /*数据单位转换*/
float LimitValue(float result, float min, float max);           /*限定值的范围*/

/*算法优化三级级标识码(编号)分类器*/
algo_func Optimize_A(unit_16 order);
algo_func Optimize_B(unit_16 order);
algo_func Optimize_C(unit_16 order);
algo_func Optimize_D(unit_16 order);
algo_func Optimize_E(unit_16 order);
algo_func Optimize_F(unit_16 order);
algo_func Optimize_G(unit_16 order);
algo_func Optimize_H(unit_16 order);
algo_func Optimize_I(unit_16 order);
algo_func Optimize_J(unit_16 order);
algo_func Optimize_K(unit_16 order);
algo_func Optimize_L(unit_16 order);
algo_func Optimize_M(unit_16 order);
algo_func Optimize_N(unit_16 order);
algo_func Optimize_X(unit_16 order);
algo_func Optimize_Y(unit_16 order);

/*具体优化算法函数声明部分*/
void  SortAscending(float buff[], int start, int len);      /*升序排序*/
void  SortDescending(float buff[], int start, int len);     /*降序排序*/
float GetMinValue(sensor_item *item, float buff[]);         /*获取最小值*/
float GetMaxValue(sensor_item *item, float buff[]);         /*获取最大值*/
float GetMedianValue(sensor_item *item, float buff[]);      /*获取中位数*/
float ArithmeticAverage(sensor_item *item, float buff[]);   /*算术平均值*/

/*外部函数*/
extern void printStr(char *str);
extern void printHexStr(unsigned char *str, const unsigned char *buf, unit_32 len);
extern int  CycleBuffNum( );
extern unit_8 *ItemLabel(sensor_item *item);
extern unit_16* ItemAlgoCode(sensor_item *item);
extern sensor_item *ItemNext(sensor_item **item);
extern dual_buff* BuffPull();
extern int ItemDataCode(sensor_item *item);
extern int GetBuffIndex(int buff_ith, int level);
extern int GetBuffIndexByCode(unit_16 sensor_code, int level);   /*通过标识码获取数据缓存的索引*/
extern int ItemTotalNum();                                       /*传感器项目总数*/
extern float GetResult(int buff_pos);                            /*获取最终值*/
extern void SaveResult(int buff_ith, float result);              /*获取最终值*/
extern void SaveJsonData(char *str);                             /*保存CJSON格式数据*/
extern void SaveModbusData(char *str,int len);                   /*储存封装的modbus指令数据*/
extern void CRC_Generator(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len); /*校验值生成器*/
extern void GetDeviceNo(char *buff);
#endif //!OPTIMIZATION_H