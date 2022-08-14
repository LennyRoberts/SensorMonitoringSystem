#ifndef GLOBAL_INFO_H
#define GLOBAL_INFO_H

/*====================================#系统内部统一识别码#=====================================*/
/*标识码使用一个unsigned short表示：高8位(分类码)+低8位(功能地址码)*/
// 1.一级分类(组别)：划分高八位的每16个为一组，用来表示设备不同组别的功能
// 2.二级分类(类别)：每个一级分类下的每一个高八位表示一个类别，用来表示改组别下的该类别
// 3.三级分类(编号)：指令标识码的低八位，在不同组别类别的不同编号指向最终的表示地址
/*----------------------------------------
| 0x01** ~ 0x16** | 传感器主类(16) |
| 0x17** ~ 0x32** | 传感器副类(16) |
| 0x33** ~ 0x48** | 算法实现类(16) |
| 0x49** ~ 0x64** | 设备功能类(16) |
| 0x65** ~ 0x80** | 外部实现类(16) |
| 0x81** ~ 0x96** | 屏幕显示类(16) |
| 0x97** ~ 0x99** | 保留(3)       |
-----------------------------------------*/
/*--------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!-------------------------------*/
// 在设备维护过程中出现对指令标识码的增、删、改等操作时请遵循以下规则：
// 1.每个一级分类(组别)开头要定义起始位置(*_START)和结束位置(*_END)，确保使用和定义的标识码在这个组别/类别中！
// 2.一级分类(组别)保持互斥，二级分类(类别)保持互斥，三级分类(编号)在同一类别分类下保持互斥!
// 3.每个二级分类(类别)开头要定义该类别的高八位类别号，以及在低八位表示该类别中所含三级分类标识码的个数！
// 4.算法标识码具有两重优先级，系统依据优先级别先后执行不同的算法。
// 5.算法标识码优先级规则：类别优先级>编号优先级;数值越小优先级越高
/*==========================================================================================*/

/*--------------------#传感器主类内部参数统一识别码#---------------------*/
#define SENSOR_ONE_START                  0x0100
#define SENSIOR_ONE_END                   0x1600

/*Sensor_A*/
#define SENSOR_A                          0x0101
#define NOISE                             0x0101
/*Sensor_B*/
#define SENSOR_B                          0x0200
/*Sensor_C*/
#define SENSOR_C                          0x03000
/*Sensor_D*/
#define SENSOR_D                          0x0400
/*Sensor_E*/
#define SENSOR_E                          0x0500
/*Sensor_F*/
#define SENSOR_F                          0x0600
/*Sensor_G*/
#define SENSOR_G                          0x0700
/*Sensor_H*/
#define SENSOR_H                          0x0800
/*Sensor_I*/
#define SENSOR_I                          0x0900
/*Sensor_J*/
#define SENSOR_J                          0x1000
/*Sensor_K*/
#define SENSOR_K                          0x1100
/*Sensor_L*/
#define SENSOR_L                          0x1200
/*Sensor_M*/
#define SENSOR_M                          0x1300
/*Sensor_N*/
#define SENSOR_N                          0x1400
/*Sensor_X*/
#define SENSOR_X                          0x1500
/*Sensor_Y*/
#define SENSOR_Y                          0x1600

/*--------------------#传感器副类内部参数统一识别码#---------------------*/
#define SENSOR_TWO_START                  0x1700
#define SENSIOR_TWO_END                   0x3200

/*Sensor_A2*/
#define SENSOR_A2                         0x1700
/*Sensor_B2*/
#define SENSOR_B2                         0x1800
/*Sensor_C2*/
#define SENSOR_C2                         0x1900
/*Sensor_D2*/
#define SENSOR_D2                         0x2000
/*SENSOR_E2*/
#define SENSOR_E2                         0x2100
/*Sensor_F2*/
#define SENSOR_F2                         0x220
/*Sensor_G2*/
#define SENSOR_G2                         0x2300
/*Sensor_H2*/
#define SENSOR_H2                         0x2400
/*Sensor_I2*/
#define SENSOR_I2                         0x2500
/*Sensor_J2*/
#define SENSOR_J2                         0x2600
/*Sensor_K2*/ 
#define SENSOR_K2                         0x2700
/*Sensor_L2*/ 
#define SENSOR_L2                         0x2800
/*Sensor_M2*/ 
#define SENSOR_M2                         0x2900
/*Sensor_N2*/
#define SENSOR_N2                         0x3000
/*Sensor_X2*/
#define SENSOR_X2                         0x3100
/*Sensor_Y2*/
#define SENSOR_Y2                         0x3200


// /*---------------------#数据优化类内部统一识别码#-----------------------*/
#define OPTIMIZE_START                    0x3300
#define OPTIMIZE_END                      0x4800

/*optimize_A*/
#define OPTIMIZE_A                        0x3300
/*optimize_B*/
#define OPTIMIZE_B                        0x3400
/*optimize_C*/
#define OPTIMIZE_C                        0x3500
/*optimize_D*/
#define OPTIMIZE_D                        0x3600
/*optimize_E*/
#define OPTIMIZE_E                        0x3700
/*optimize_F*/
#define OPTIMIZE_F                        0x3800
/*optimize_G*/
#define OPTIMIZE_G                        0x3900
/*optimize_H*/
#define OPTIMIZE_H                        0x4000
/*optimize_I*/
#define OPTIMIZE_I                        0x4100
/*optimize_J*/
#define OPTIMIZE_J                        0x4200
/*optimize_K*/
#define OPTIMIZE_K                        0x4300
/*optimize_L*/
#define OPTIMIZE_L                        0x4400
/*optimize_M*/
#define OPTIMIZE_M                        0x4500
/*optimize_N*/
#define OPTIMIZE_N                        0x4600
/*optimize_X*/
#define OPTIMIZE_X                        0x4704
#define ARITHMETIC_AVERAGE                0x4701
#define MINIMUM_VALUE                     0x4702
#define MAXIMUM_VALUE                     0x4703
#define MEDIAN_VALUE                      0x4704
/*optimize_Y*/
#define OPTIMIZE_Y                        0x4800

/*----------------------------------------------------------------*/
#define DEVICE_START                      0x4900
#define DEVICE_END                        0x6400

/*device_A*/
#define DEVICE_A                          0x4907
#define NO_ANY_ERROR                      0x4901  /*无传感器错误*/
#define HARDWARE_ERROR                    0x4902  /*硬件错误*/
#define DATA_ERROR                        0x4903  /*数据错误*/
#define EXCEED_MAX_RANGE                  0x4904  /*超出最大量程*/
#define ABNORMAL_COMMUNICATION            0x4905  /*通信异常*/
#define CALIBRATION_ZERO                  0x4906  /*传感器校准(零点)*/
#define CALIBRATION_SPAN                  0x4907  /*传感器校准(Span点)*/
/*device_B*/
#define DEVICE_B                          0x6003
#define PIPE_CLEANING                     0x6001  /*管道清洁*/
#define INTELLIGENT_ALARM                 0x6002  /*智能报警*/
#define DEVICE_CALIBRATION                0x6003  /*使用设备装置校零*/
/*device_C*/
#define DEVICE_C                          0x3500
/*device_D*/
#define DEVICE_D                          0x3600
/*device_E*/
#define DEVICE_E                          0x3700
/*device_F*/
#define DEVICE_F                          0x3800
/*device_G*/
#define DEVICE_G                          0x3900
/*device_H*/
#define DEVICE_H                          0x4000
/*device_I*/
#define DEVICE_I                          0x4100
/*device_J*/
#define DEVICE_J                          0x4200
/*device_K*/
#define DEVICE_K                          0x4300
/*device_L*/
#define DEVICE_L                          0x4400
/*device_M*/
#define DEVICE_M                          0x4500
/*device_N*/
#define DEVICE_N                          0x4600
/*device_X*/
#define DEVICE_X                          0x4700
/*device_Y*/
#define DEVICE_Y                          0x4800

/*----------------------------------------------------------------*/
#define OUTSIDE_START                     0x6500
#define OUTSIDE_END                       0x8000

/*outside_A*/
#define OUTSIDE_A                         0x6500
/*outside_B*/
#define OUTSIDE_B                         0x6600
/*outside_C*/
#define OUTSIDE_C                         0x6700
/*outside_D*/
#define OUTSIDE_D                         0x6800
/*outside_E*/
#define OUTSIDE_E                         0x6900
/*outside_F*/
#define OUTSIDE_F                         0x7000
/*outside_G*/
#define OUTSIDE_G                         0x7100
/*outside_H*/
#define OUTSIDE_H                         0x7200
/*outside_I*/
#define OUTSIDE_I                         0x7300
/*outside_J*/
#define OUTSIDE_J                         0x7400
/*outside_K*/
#define OUTSIDE_K                         0x7500
/*outside_L*/
#define OUTSIDE_L                         0x7600
/*outside_M*/
#define OUTSIDE_M                         0x7700
/*outside_N*/
#define OUTSIDE_N                         0x7800
/*outside_X*/
#define OUTSIDE_X                         0x7900
/*outside_Y*/
#define OUTSIDE_Y                         0x8000

/*----------------------------------------------------------------*/
#define SCREEN_START                      0x8100
#define SCREEN_END                        0x9600

/*screen_A*/
#define SCREEN_A                          0x8107
/*screen_B*/
#define SCREEN_B                          0x8203
/*screen_C*/
#define SCREEN_C                          0x8300
/*screen_D*/
#define SCREEN_D                          0x8400
/*screen_E*/
#define SCREEN_E                          0x8500
/*screen_F*/
#define SCREEN_F                          0x8600
/*screen_G*/
#define SCREEN_G                          0x8700
/*screen_H*/
#define SCREEN_H                          0x8800
/*screen_I*/
#define SCREEN_I                          0x8900
/*screen_J*/
#define SCREEN_J                          0x9000
/*screen_K*/
#define SCREEN_K                          0x9100
/*screen_L*/
#define SCREEN_L                          0x9200
/*screen_M*/
#define SCREEN_M                          0x9300
/*screen_N*/
#define SCREEN_N                          0x9400
/*screen_X*/
#define SCREEN_X                          0x9500
/*screen_Y*/
#define SCREEN_Y                          0x9600

/*------------------------------#预留#-----------------------------*/
#define RESERVE_START                     0x9700
#define RESERVE_END                       0x9600

/*====================================!cJSON解析标签!=======================================*/
#define DEVICE_NO                         "devno"     // 设备编号
#define MEASURE_TIME                      "time"     // 测量时间(修正后)
#define SENSOR_CODE                       "code"     // 传感器标识码
#define SENSOR_DATA                       "data"     // 传感器测量数据
#define SENSOR_UNIT                       "unit"     // 数据单位
#define SENSOR_FLAG                       "flag"     // 传感器状态

/*=================================!设备及传感器状态标识!=====================================*/
#define NORMAL                            'N'   //可用
#define UNUSABLE                          'U'   //不可用
#define UNINITIALIZED                     'Z'   //未初始化
#define MONOPOLY                          'M'   //被独占
#define DATA_OVERFLOW                     'O'   //数据溢出
#define EARLY_ALARM                       'W'   //预警
#define ALARM                             'A'   //报警

#define ON                                0xFE        //open
#define OFF                               0x01        //close

/*==================================================!通讯指令规则!=========================================================*/
/* 类ModBus协议的指令规则:
 * 头部(1) + 版本号(1) + 模块地址(1) + 功能码(1) + 长度(1) + 主标识码(2) + 从标识码(2) + 数据(4) + CRC16(2) + 尾部(1) = 总长16byte
 * 头部标识:         用来告知接收端该指令的起始位置
 * 版本号:           指定代码的版本，用来区别新旧程序
 * 模块地址:         用来指名当前指令的执行对象：设备，业务管理，本地模块，触摸屏等
 * 功能码:           确定要执行的指令功能种类
 * 主标识地址码:      用来指定内部已封装的指令，确定要执行的程序
 * 副标识地址码:      用来指定内部已封装的指令，确定要执行的程序
 * 数据字段:         为方便计算指令的数据字段使用4字节有符号int来表示，最终数据默认除以1000来保持三位小数
 * CRC Module：     modbus_CRC16 + Low byte first
 * 尾部标识:         标记本条指令的结束位置

 * 1.每次下发的指令不超过32条;
 * 2.指令具有执行优先级，功能码越小优先级越高;
 * 3.同一条指令中分为主地址标识码和副地址标识码，执行指令时默认先执行主地址标识码，若不需要副地址标识可为空;
 *----------------------------------------------------------------------------------------------------------------------*/
#define CMD_SIZE                          16          // 指令长度(*Byte)

#define HEAD_POS                          0
#define HEAD_SIZE                         1
#define VERSION_POS                       1
#define VERSION_SIZE                      1
#define MODULE_POS                        2
#define MODULE_SIZE                       1
#define FUNC_POS                          3
#define FUNC_SIZE                         1
#define LENGHT_POS                        4
#define LENGHT_SIZE                       1
#define ADDR_POS_1                        5
#define ADDR_1_SIZE                       2
#define ADDR_POS_2                        7
#define ADDR_2_SIZE                       2
#define DATA_POS                          9
#define DATA_SIZE                         4
#define CRC_POS                           13
#define CRC_SIZE                          2
#define TAIL_POS                          15
#define TAIL_SIZE                         1

#define CMD_HEAD                          0x7B        // 指令的头部标识
#define VERSION                           0x0A        // 版本号

#define MODULE_DEVICE                     0x0F        // 核心设备模块
#define DEV_FUNC_CALIBRATE                0x01        // 数据校对
#define DEV_FUNC_DATA                     0x02        // 数据请求
#define DEV_FUNC_ISPC                     0x03        // 串口间通信
#define DEV_DUNC_EXEUTE                   0x04        // 功能执行

#define MODULE_BUSINESS                   0x1F        // 业务层模块

#define MODULE_LOCAL                      0x2F        // 本地服务模块

#define MODULE_LCD                        0x3F        // 触摸屏模块

#define LENGHT_DATA                       0x08        // 数据长度
#define GET_ALL_DATA                      0xFEFE      // 标识码字段(请求所有数据)

#define CMD_TAIL                          0x7C        // 指令尾部标识                    

#define MAX_CMD_NUMBER                    32          // 一次缓存最大指令数
#define DATA_DEVIDE                       1000        // 数据字段的解析倍率

#define CMD_ERROR_NULL                    0x0101      // 指令错误(传输指令为空或不完整)
#define CMD_VERSION_ERROR                 0x0102      // 版本号出错
#define CMD_ERROR_CRC                     0x0103      // 指令错误(CRC校验错误))
#define CMD_ERROR_CACHE                   0x0104      // 指令缓存出错
#define CMD_ERROR_MODULE_FUNC             0x0105      // 模块地址或功能码出错出错
#define CMD_RETRANS                       0x0120      // 指令重传
#define CMD_FUNC_FAILE                    0x0140      // 功能或参数执行失败
#define CMD_SUCCESS                       0x0180      // 指令执行成功



#endif //!GLOBAL_INFO_H 