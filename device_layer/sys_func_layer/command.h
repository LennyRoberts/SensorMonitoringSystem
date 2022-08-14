#ifndef COMMAND_H
#define COMMAND_H
/*======================================================================================================================
 * 指令分管器程序：主要完成对本地指令或应用层指令的分类、解析、存储和转发
 * 指令格式：头部标识(1) + 口令(2) + 指令数量(1) + 索引(1) + 标识码/地址(4) +   数据(4)  +  CRC校验(2) + 尾部标识(1) = 总长16byte
 *            ××        ** **　　   ××　　      ××       ** ** ** **   ** ** ** ** 　  ×× ××        ××
 * 例：     0x03 0x12 0x34 0x10 0x01 0x00 0x02 0x00 0x01 0x00 0x00 0x00 0x0A 0x78 0x8F 0x7B
 * 头部标识:         用来告知接收端该指令的类型与作用
 * 口令:            口令密码，防止未定义的指令对系统进行修改
 * 指令数量&&索引:   指令数量是告知接收端共要连续接收多少条指令，索引表示现在接收的是第几条指令
 * 标识码/地址:      用来指定内部已封装的指令，确定要执行的程序
 * 数据字段:         指令的数据字段统一使用IEEE_754标准的浮点数
 * CRC Module：     modbus_CRC16 + low byte first
 * 尾部标识:         标记本条指令的结束位置
 * ======================================================================================================================*/
#include <sys/types.h>
#include <sys/socket.h>

#include "../device_info.h"
#include "../local/local_manage.h"
#include "./execute.h"

#define BUSINESS   'B'
#define ISPC       'I'

/*指令缓存器，缓存接收到的多条组合指令*/
typedef struct CmdInfo{
    unit_8 module_addr;            //模块地址
    unit_8 func_code;              //功能码
    unsigned short s_addr_1;       //主标识码
    unsigned short s_addr_2;       //从标识码
    float data;                    //数据
    bool flag;                     //状态位
}cmd_info;

bool CMDManageInit();                                                 /*指令管理系统初始化*/
unit_16 CmdManage(unit_8 *cmd, int fd, char type);                    /*指令分管器*/
unit_16 ReciveAgain(unit_16 num_th, int fd, char type);               /*请求再次接收指令*/
int  SendData(int sock_fd, const char *buf, int len);                 /*发送设备层unix_socket服务端数据至服务层unix_socket客户端*/
int  RecvCmd(int sock_fd, unit_8 *buf, int len);                      /*接收服务层unix_socket客户端数据*/
unit_32 GetCmdCode(unit_8 *cmd);                                      /*解析指令中的标识码字段*/
bool CmdCRCCheck(unit_8 *cmd, int len_cmd);                           /*指令校验*/
bool CmdTempCache(unit_8 *cmd, cmd_info *contents[]);                 /*指令缓存至顺序表中，接收全部指令后再依次执行*/
void AscendingSortCmd(cmd_info *contents[], int start, int len);      /*对指令进行排序*/
bool DeleteCmdCache();                                                /*销毁指令缓存*/
func_cmd CmdModulePartition(unit_8 module_addr,unit_8 func_code);     /*模块地址分区*/

func_cmd CmdFuncCodeModuleDevice(unit_8 func_code);                   /*核心设备模块指令类型分类检查*/
bool ReplayData(unsigned short addr1, unsigned short addr2, float data, int fd);       /*数据回复*/
int GetAllSensorDataJson(int fd);                                                      /*获取全部数据*/
bool ReplyModbusData(unsigned short addr1, unsigned short addr2, float data, int fd);  /*获取modbus数据*/

func_cmd CmdFuncCodeModuleLocal(unit_8 func_code);                    /*本地服务模块指令类型分类检查*/

func_cmd CmdFuncCodeModuleLCD(unit_8 func_code);                      /*触摸屏模块指令类型分类检查*/



/*外部函数*/
extern void printStr(char *str);
extern void FlushFdIn(int fd);                                        /*清除正收到的数据，且不会读取出来*/
extern void FlushFdOut(int fd);                                       /*清除正写入的数据，且不会发送至终端*/
extern void FlushFdInOut(int fd);                                     /*清除所有正在发生的I/O数据*/
extern int  GetJsonPackage(char *str);                                /*获取Json格式数据*/
extern int  GetModbusData(char *str);                                 /*获取封装的modbus数据*/
extern sensor_type *TypeLinkHead();
extern sensor_type *TypeNext(sensor_type **type);
extern sensor_item *ItemNext(sensor_item **item);
extern bool CRC_Check(unit_16 crc_name, unit_16 storage_mode, unit_8 *data, unit_32 len);
extern bool FuncCtrEntry(unsigned short addr1, unsigned short addr2, float data, int fd_sock);
extern bool LocalModuleModify(unit_8 *cmd);                          /*处理来自服务层的本地服务模块指令*/
#endif // !COMMAND_H
