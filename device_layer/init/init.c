#include "init.h"

/*=========================================================================================================================
                                            运行参数初始化程序部分
--------------------------------------------------------------------------------------------------------------------------*/
/*打印串口参数信息*/
void printSerailPortInfo()
{
    int index = 0;
    serial_port **port = SerialPort();
    printf("\n                      Serial Port Configuration Information\n");
    printf("=====================================================================================\n");
    printf("|No|     Port_Name      |     Device_Name    |SpeedBaud|DataBit|StopBit|Parity|State|\n");
    printf("-------------------------------------------------------------------------------------\n");
    for(index=0; index < SERIAL_POER_NUM; index++){
        if(port[index] != NULL){
            printf(" %-2d ", port[index]->port_ith);
            printf("%-20s ", port[index]->portName);
            printf("%-20s ", port[index]->devName);
            printf("%-9d ", port[index]->speed);
            printf("%-7d ", port[index]->dataBit);
            printf("%-7d ", port[index]->stopBit);
            printf("%-6c ", port[index]->parity);
            printf("%-5c", port[index]->state);
            printf("\n");
        }
    }
    printf("=====================================================================================\n");
}

/*打印设备功能开关量*/
void printSwithc(func_switch *func)
{
    func_switch *ptr_func = func;
    printf("\n   Function Switch\n");
    printf("==========================\n");
    printf("  Function_name    state  \n");
    printf("--------------------------\n");
    if(ptr_func->action == ON)
        printf("         Action     ON   \n");
    if(ptr_func->action == OFF)
        printf("         Action     OFF  \n");

    if(ptr_func->video_overlay == ON)
        printf("  Video_overlay     ON   \n");
    if(ptr_func->video_overlay == OFF)
        printf("  Video_overlay     OFF  \n");

    if(ptr_func->video_capture == ON)
        printf("  Vedio_capture     ON   \n");
    if(ptr_func->video_capture == OFF)
        printf("  Vedio_capture     OFF  \n");

    if(ptr_func->DTU_trans == ON)
        printf("      DTU_trans     ON   \n");
    if(ptr_func->DTU_trans == OFF)
        printf("      DTU_trans     OFF  \n");

    if(ptr_func->net_port_trans == ON)
        printf(" Net_port_trans     ON   \n");
    if(ptr_func->net_port_trans == OFF)
        printf(" Net_port_trans     OFF   \n");

    if(ptr_func->beeper_alarm == ON)
        printf("   Beeper_Alarm     ON   \n");
    if(ptr_func->beeper_alarm == OFF)
        printf("   Beeper_Alarm     OFF   \n");

    if(ptr_func->text_screen == ON)
        printf("    Text_screen     ON   \n");
    if(ptr_func->text_screen == OFF)
        printf("    Text_screen     OFF   \n");

    if(ptr_func->calibration == ON)
        printf("    Calibration     ON   \n");
    if(ptr_func->calibration == OFF)
        printf("    Calibration     OFF   \n");
    printf("==========================\n");
}

/*打印设备信息*/
void printManuInfo(version *ver)
{
    printf("\n            Equipment Information\n");
    printf("==================================================\n");
    printf("       Manual_name      Manual_information \n");
    printf("--------------------------------------------------\n");
    printf("     Device_Module       %s\n",ver->dev_module);
    printf("         Device_No       %s\n",ver->dev_no);
    printf("   Production Date       %s\n",ver->production_date);
    printf("  Headware_Version       %s\n",ver->hardware_version);
    printf(" SofetWare_Version       %s\n",ver->software_version);
    printf("    Kernel_Version       %s\n",ver->kernel_version);
    printf(" Manufacture_Infor       %s\n",ver->manufacture_name);
    printf("==================================================\n");
}

/*打印控制器信息*/
void printContrllerInfo(ctrl *control)
{
    int i = 1;
    int len = 0;
    char buff[1024];
    ctrl *ptr_ctrl = control;
    printf("\n               Controller Parameter Information\n");
    printf("=================================================\n");
    printf("|No|   Ctrl_Name   |       Use_Port       |state|\n");
    printf("-------------------------------------------------\n");
    while(ptr_ctrl != NULL){
        memset(buff, 0, 1024);
        len = 0;
        len += sprintf(buff+len, " %-2d ", ptr_ctrl->ctrl_ON);
        len += sprintf(buff+len, "%-15s ", ptr_ctrl->ctrl_name);
        serial_port **port = SerialPort();
        len += sprintf(buff+len, "%-22s ", port[ptr_ctrl->port_ith]->portName);
        if(ptr_ctrl->initial_state == ON)
            len += sprintf(buff + len, "ON");
        else if(ptr_ctrl->initial_state == OFF)
            len += sprintf(buff + len, "OFF");
        len += sprintf(buff+len, "\n%-9s: ", "ON_cmd");
        int j=0;
        for(j=0; j<ptr_ctrl->len_open_order; j++)
            len += sprintf(buff+len, "0x%02X ", ptr_ctrl->open_order[j]);
        len += sprintf(buff+len, "\n%-9s: ", "OFF_cmd");
        for(j=0; j<ptr_ctrl->len_close_order; j++)
            len += sprintf(buff+len, "0x%02X ", ptr_ctrl->close_order[j]);
        ptr_ctrl = ptr_ctrl->next;
        i++;
        printf("%s\n",buff);
        printf("-------------------------------------------------\n");
    }
    printf("=================================================\n");
}

/*打印串口交互信息*/
void printInteractPortinfor(ispc* src)
{
    ispc *ptr_ispc = src;
    if(ptr_ispc == NULL){
        printf("Serial port interact is empty!!!\n");
        return;
    }
    int i = 1;
    printf("\n    Inter serial port communication informations\n");
    printf("=========================================================\n");
    printf("|NO|   ISPC_Name    |Enabel|Monopoly|      Use_port     |\n");
    printf("---------------------------------------------------------\n");
    while(ptr_ispc != NULL){
        printf(" %-2d ", i);
        printf("%-16s ", ptr_ispc->name);
        if(ptr_ispc->enable == ON) printf("ON%-5s"," ");
        else if(ptr_ispc->enable == OFF) printf("OFF%-4s"," ");
        if(ptr_ispc->monopoly == ON) printf("ON%-7s"," ");
        else if(ptr_ispc->monopoly == OFF) printf("OFF%-6s"," ");
        serial_port **port = SerialPort();
        printf("%-20s \n", port[ptr_ispc->port_ith]->portName);
        ptr_ispc = ptr_ispc->next;
        i++;
    }
    printf("=========================================================\n");
}

/*打印设备基本配置信息*/
void printDeviceInfor()
{
    device_para *ptr_dev = DeviceParaPtr();
    if(ptr_dev == NULL){
        printf("Device Infor if Empty!!!\n");
        return;
    }
    printf("\n                    Device Network Information\n");
    printf("====================================================================\n");
    printf("|   Device_IP    |IpPort|    Net_Mask    |   Net_Gateway  |cycleNum|\n");
    printf("--------------------------------------------------------------------\n");
    printf(" %-16s ",ptr_dev->IP);
    printf("%-6d ", ptr_dev->port);
    printf("%-16s ", ptr_dev->mask);
    printf("%-16s ", ptr_dev->gateway);
    printf("%-8d ",ptr_dev->cycle_buff_num);
    printf("\n");
    printf("====================================================================\n");
    printManuInfo(ptr_dev->version);
    printSwithc(ptr_dev->func);
    printInteractPortinfor(ptr_dev->ispc_head);
    printContrllerInfo(ptr_dev->ctrl_head);
}

/*打印传感器出错识别码*/
void printEmerInfo()
{
    sensor_type *ptr_type = TypeLinkHead();
    int i = 1;
    printf("\n                         Emergency Informations\n");
    printf("======================================================================================\n");
    printf("|NO| Type_Name  | Emer_Name  |Start|Len|EmerCode|           Emergency_Cmd            |\n");
    printf("--------------------------------------------------------------------------------------\n");
    while (ptr_type != NULL){
        emergency_cmd *ptr_emer = ptr_type->emer_head;
        while(ptr_emer != NULL){
            printf(" %-2d ", i);
            printf("%-12s ",ptr_type->type_name);
            printf("%-12s ", ptr_emer->emer_name);
            printf("%-5d ", ptr_emer->start_byte);
            printf("%-3d ", ptr_emer->len_byte);
            printf("0x%04X%-2s ",ptr_emer->emer_code," ");
            printf("0x[ ");
            int j = 0;
            for(j = 0; j < ptr_emer->len_cmd; j++)
                printf("%02X ", ptr_emer->cmd[j]);
            printf("]\n");
            ptr_emer = ptr_emer->next;
            i++;
        }
        ptr_type = ptr_type->next;
    }
    printf("======================================================================================\n");
}

/*打印传感器参数信息*/
void printItemInfo()
{
    int i = 1, j = 0;
    sensor_type *ptr_type = TypeLinkHead();
    printf("\n                                       Sensor Item Informations\n");
    printf("=============================================================================================================================================================\n");
    printf("|NO| Type_Name  | Item_Name  |Start|Len|DataType |byteOrder|  Data_Parser  |buffpos|ItemCode|UnitOrigin|Ratio |UnitTrans| Min |Maximum |AlgoNum| Algo_Code...\n");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    while (ptr_type != NULL){
        sensor_item *ptr_item = ptr_type->item_head;
        while(ptr_item != NULL){
            printf(" %-2d ", i);
            printf("%-12s ", ptr_type->type_name);
            printf("%-12s ", ptr_item->item_name);
            printf("%-5d ", ptr_item->start_byte);
            printf("%-3d ", ptr_item->len_byte);
            if(ptr_item->value_type == SIGNED_SHORT)
                printf("%-10s","short");
            else if(ptr_item->value_type == UNSIGNED_SHORT)
                printf("%-10s","un_short");
            else if(ptr_item->value_type == SIGNED_INT)
                printf("%-10s","int");
            else if(ptr_item->value_type == UNSIGNED_INT)
                printf("%-10s","un_int");
            else if(ptr_item->value_type == FLOATING)
                printf("%-10s ","float");
            else
                printf("%-10s","error");

            if(ptr_item->byte_order == ORDER_ABCD)
                printf("%-9s ","ABCD");
            else if(ptr_item->byte_order == ORDER_CDAB)
                printf("%-9s ","CDAB");
            else if(ptr_item->byte_order == ORDER_BADC)
                printf("%-9s ","BADC");
            else if(ptr_item->byte_order == ORDER_DCBA)
                printf("%-9s ","DCBA");
            else    
                printf("%-9s ","error");

            printf("y=%.3f*x+%.3f ", ptr_item->a, ptr_item->b);
            printf("%-7d ", ptr_item->buff_ith);
            printf("0x%04X%-2s ", ptr_item->sensor_code, " ");
            printf("%-10s ",ptr_item->unit_origin);
            printf("%.3f  ",ptr_item->unit_ratio);
            printf("%-9s ",ptr_item->unit_trans);
            printf("%.3f ",ptr_item->minimum);
            printf("%.3f ",ptr_item->maximum);
            printf("%-7d ", ptr_item->num_algo_code);
            for(j=0; j<ptr_item->num_algo_code; j++)
                printf("0x%04X; ", ptr_item->algo_code[j]);
            printf("\n");
            ptr_item = ptr_item->next;
            i++;
        }
        ptr_type = ptr_type->next;
    }
    printf("=============================================================================================================================================================\n");
}

/*指令校验模式信息*/
void GetCRCModule(unit_8 *str, unit_16 module)
{
    memset(str,0,32);
    switch(module){
        case CRC16_MODBUS:
            memcpy(str,"CRC16_MODBUS",13);break;
        case CRC16_IBM:
            memcpy(str,"CRC16_IBM",10);break;
        case CRC16_CCITT:
            memcpy(str,"CRC16_CCITT",11);break;
        case CRC16_CCITT_FALSE:
            memcpy(str,"CRC16_CCITT_FALSE",18);break;
        case CRC16_DNP:
            memcpy(str,"CRC16_DNP",9);break;
        case CRC16_MAXIM:
            memcpy(str,"CRC16_MAXIM",12);break;
        case CRC16_USB:
            memcpy(str,"CRC16_USB",9);break;
        case CRC16_X25:
            memcpy(str,"CRC16_X25",9);break;
        case CRC16_XMODEM:
            memcpy(str,"CRC16_XMODEM",12);break;
        default:
            printf("Not found CRC check module!!!\n");
            break;
    }
}

/*指令校验码翻转*/
void GetCRCTurn(unit_8 *str, unit_16 turn)
{
    memset(str, 0, 32);
    switch(turn){
    case HIGH_BIT_FIRST:
        memcpy(str,"HighByteFirst",13);break;
    case LOW_BIT_FIRST:
        memcpy(str,"LowByteFirst",12);break;
    default:
        printf("Not found CRC turn module!!!\n");
        break;
    }
}

/*打印传感器相关信息*/
void printSensorInfomation()
{
    sensor_type *ptr_type = TypeLinkHead();
    if(ptr_type == NULL){
        printf("Sensor information is Empty!!!\n");
        return;
    }
    unit_8 *str = (unit_8 *)malloc(32);
    if(str == NULL){
        printf("Malloc failed when print sensor infor!!!\n");
        return;
    }
    printf("\n                                                      Sensor Type Informations\n");
    printf("==========================================================================================================================================================\n");
    printf("|NO|  TypeName  |readMode|      Use_Port      |     CRC_Module     |   CRC_Turn  |LenReply|response|       Read_Sensor_Type_Cmd       | Reply_Start_Flag |\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    int i = 1;
    while(ptr_type != NULL){
        printf(" %-2d ",i);
        printf("%-12s ",ptr_type->type_name);
        if(ptr_type->read_mode == READ_PASSIVE)
            printf("%-8s ","passive");
        else if(ptr_type->read_mode == READ_ACTIVE)
            printf("%-8s ","active");
        else
            printf("%-8s ","erro_mode");
        serial_port **port = SerialPort();
        printf("%-20s ", port[ptr_type->port_ith]->portName);
        GetCRCModule(str, ptr_type->crc_module);
        printf("%-20s ",str);
        GetCRCTurn(str, ptr_type->crc_turn);
        printf("%-13s ",str);
        printf("%-8d ",ptr_type->len_data);
        printf("%-8d ",ptr_type->time_response);
        int i = 0;
        printf("0x[ ");
        for(i = 0; i< ptr_type->len_cmd; i++) 
            printf("%02X ", ptr_type->read_cmd[i]);
        printf("]%-4s"," ");
        printf("0x[ ");
        for(i = 0; i< ptr_type->len_startflag; i++) 
            printf("%02X ", ptr_type->start_flag[i]);
        printf("]\n");
        ptr_type = ptr_type->next;
        i++;
    }
    printf("==========================================================================================================================================================\n");
    free(str);
    printEmerInfo();
    printItemInfo();
}


/*================================================#初始化功能性函数#================================================*/
/*整型转波特率*/
int int2baud(int baud)
{
    switch(baud){
        case 9600:  return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200:return B115200;
        case 4800:  return B4800;
        case 2400:  return B2400;
        case 1800:  return B1800;
        case 1200:  return B1200;
        case 600:   return B600;
        case 300:   return B300;
        case 200:   return B200;
        case 150:   return B150;
        case 134:   return B134;
        case 75:    return B75;
        case 50:    return B50;
        case 0:     return B0;
        default:    printStr("Invalid Speed,Default Baud: B9600");return B9600;
    }
}

/*设置数据位*/
void SetDataBit(struct termios *termptr, int data_bit)
{
    termptr->c_cflag &= ~CSIZE;
    switch(data_bit){
        case 8: termptr->c_cflag |= CS8;break;
        case 7: termptr->c_cflag |= CS7;break;
        case 6: termptr->c_cflag |= CS6;break;
        case 5: termptr->c_cflag |= CS5;break;
        default:
            termptr->c_cflag |= CS8;
            printStr("Invalid Data Bit, System Default is 8bit");
            break;
    }
}

/*设置停止位*/
void SetStopBit(struct termios *termptr, int stop_bit)
{
    if(stop_bit == 1) termptr->c_cflag &= ~CSTOPB;
    else if(stop_bit == 2) termptr->c_cflag |= CSTOPB;
    else{
        termptr->c_cflag &= ~CSTOPB;
        printStr("Invalid stop bit, system default is 1bit");
    }
}

/*设置校验位*/
void SetParity(struct termios *termptr, char check_bit)
{
    switch(check_bit){
        case 'N': /*NO check*/
        case 'n':
            termptr->c_cflag &= ~PARENB;break;
        case 'E': /*Even parity check*/
        case 'e':
            termptr->c_cflag |= PARENB;
            termptr->c_cflag &= ~PARODD;
            break;
        case 'O': /*Odd check*/
        case 'o':
            termptr->c_cflag |= PARENB;
            termptr->c_cflag |= ~PARODD;
            break;
        default:
            termptr->c_cflag &= ~PARENB;
            printStr("Invalid parity, system default is No parity");
            break;
    }
}

/*设置串口属性*/
bool SetSerialPortAttributes(int32_t fd, int data_bit, int stop_bit, char check_bit)
{
    struct termios term;
    if(tcgetattr(fd, &term) != 0) return FALSE;
    term.c_iflag &= ~(BRKINT | ICRNL | ISTRIP | IXON); /*BRKINT:将产生SIGINT中断；ICRNL:将输入的CR转换为NL；ISTRIP:剥除字符第8位；IXON:启用/停止输出控制流起作用*/
    term.c_oflag &= ~OPOST;                            /*OPOST:执行输出处理*/
    term.c_cflag |= (CLOCAL | CREAD);                  /*CLOCAL:忽略调制解调器状态行；CREAD:启用接收装置*/
    term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);   /*ICANON:启用规范输入；ECHO:进行回送；ECHOE:可见擦除字符；ISIG:启用终端产生的信号*/
    term.c_cflag &= ~CSIZE;                            /*CSIZE:数据位屏蔽*/

    SetDataBit(&term, data_bit);
    SetStopBit(&term, stop_bit);
    SetParity(&term, check_bit);

    term.c_cc[VTIME] = 20; /*read()允许等待的最大时间=5s*/
    term.c_cc[VMIN] = 0;   /*在串口接到1字节数据或者计时器超时后即返回*/
    FlushFdIn(fd);         /*Update the termptr and do it NOW*/
    if(tcsetattr(fd, TCSANOW, &term) != 0){
        Perror("tcsetattr");
        return FALSE;
    }
    return TRUE;
}

/*设置串口波特率*/
bool SetSpeed(int fd, int speed)
{
    struct termios term;
    tcgetattr(fd, &term);
    FlushFdInOut(fd);
    cfsetispeed(&term, int2baud(speed));
    cfsetospeed(&term, int2baud(speed));
    /*TCSANOW:不等数据传输完毕就立即改变属性；TCSADRAIN:等待所有数据传输结束才改变属性；TCSAFLUSH:等待所有数据传输结束,清空输入输出缓冲区才改变属性*/
    if(tcsetattr(fd, TCSANOW, &term) != 0){
        Perror("tcsetattr");
        return FALSE;
    }
    else{
        FlushFdInOut(fd);
        return TRUE;
    }
}

/*获取打开串口的fd文件描述符*/
int32_t GetSerialPortFd(unit_16 port_ith)
{
    int32_t fd_port = INVALID_FD;
    serial_port **port = SerialPort();
    int index = port_ith;
    if(port[index]->state == UNINITIALIZED){
        printf("%-16s Not initialized\n!!!", port[index]->portName);
        return INVALID_FD;
    }
    else if(port[index]->state == UNUSABLE){
        printf("%-16s is invalid\n!!!", port[index]->portName);
        return INVALID_FD;
    }
    fd_port = port[index]->fd_port;
    if(port[index]->state == MONOPOLY){
        printf("Fd(%d) is exclusive to %s(ispc)!!!\n", fd_port, port[index]->obj_state);
        return INVALID_FD;
    }
    else if(port[index]->state == NORMAL){
        printf("Get fd(%d) serial port is %s\n", port[index]->fd_port,port[index]->portName);
        return fd_port;
    }
}

/*单字符转十六进制*/
unit_8 CharToHex(char c)
{
    switch(c){
        case '0':return 0x00;
        case '1':return 0x01;
        case '2':return 0x02;
        case '3':return 0x03;
        case '4':return 0x04;
        case '5':return 0x05;
        case '6':return 0x06;
        case '7':return 0x07;
        case '8':return 0x08;
        case '9':return 0x09;
        case 'A':
        case 'a':
            return 0x0A;
        case 'B':
        case 'b':
            return 0x0B;
        case 'C':
        case 'c':
            return 0x0C;
        case 'D':
        case 'd':
            return 0x0D;
        case 'E':
        case 'e':
            return 0x0E;
        case 'F':
        case 'f':
            return 0x0F;
        default:
            return 0x00;
    }
}

/*文本字符转到单字节十六进制无符号指令*/
unit_32 StringToCmd(unit_8 *src, char *str, int len)
{
    int i = 0;
    unit_32 cmd_i = 0;
    memset(src, 0, 32);
    for(i = 2; i < len; i += 2)
        src[cmd_i++] = ((CharToHex(str[i]) << 4) | CharToHex(str[i + 1])) & 0xFF;
    return cmd_i;
}

/*文本字符转到十六进制数*/
unit_32 StringToHex(char *str, int len)
{
    unit_8 cmd_buff[32];
    int cmd_len = StringToCmd(cmd_buff, str, len);
    int i = 0;
    unsigned int cmd_hex = 0;
    for(i = 0; i < cmd_len; i++)
        cmd_hex |= cmd_buff[i] << (8 * (cmd_len - 1 - i));
    return cmd_hex;
}

/*字符串转数字*/
unit_32 StringTounit32(char *str, int len)
{
    int result = 0;
    int n = len;
    int devide = 1;
    int i = 0;
    for(i = 0; i < len; i++){
        devide = pow(10, n - 1);
        result += (str[i] - '0') * devide;
        n--;
    }
    return result;
}

char IsOrNot(char *str)
{
    if(!strncmp(str, "OFF", 3) || !strncmp(str, "Off", 3) || !strncmp(str, "off", 3))
        return OFF;
    if(!strncmp(str, "ON", 2) || !strncmp(str, "On", 2) || !strncmp(str, "on", 2))
        return ON;
    else{
        printf("Invalid switch!!!\n");
        return OFF;
    }
}

/*获取传感器类型结点信息*/
bool GetTypeNode(sensor_type **src, unit_16 s_link)
{
    *src = NULL;
    bool result = FALSE;
    sensor_type *ptr_type = TypeLinkHead();
    while(ptr_type != NULL){
        if(ptr_type->sensor_link == s_link){
            *src = ptr_type;
            break;
        }
        ptr_type = ptr_type->next;
    }
    if((*src) == NULL)
        return FALSE;
    else
        return TRUE;
}

/*标识码排序(从小到大)*/
void SortCodeMinFirst(unit_16 **buff, int start, int len)
{
    int newtag;
    unit_16 temp = 0;
    do{
        newtag = 0;
        int i = 0;
        for(i=start+1; i<len; i++)
            if(*buff[i] < *buff[i-1]){
                temp = *buff[i-1];
                *buff[i-1] = *buff[i];
                *buff[i] = temp;
                newtag = i;
            }
        len = newtag;
    } while(newtag != 0);
    return;
}

/*===============================================#初始化信息结点储存函数#===============================================*/
/*储存传感器类型信息*/
bool SaveTypeNode(sensor_type *ptr_type)
{
    if(ptr_type == NULL){
        printf("Save type Node is Empty!!!\n");
        return FALSE;
    }
    sensor_type *ptr_temp = TypeLinkHead();
    if(ptr_temp == NULL){
        if(SetTypeLinkHead(&ptr_type) == FALSE)
            return FALSE;
    }
    else{
        while(ptr_temp->next != NULL)
            ptr_temp = ptr_temp->next;
        ptr_temp->next = ptr_type;
        if(ptr_temp->next == NULL)
            return FALSE;
    }
    return TRUE;
}

/*储存应急指令信息*/
bool SaveEmerNode(emergency_cmd *src, unit_32 s_link)
{
    if(src == NULL){
        printf("Save Emer Node is Empty!!!\n");
        return FALSE;
    }
    sensor_type *ptr_type = NULL;
    bool result = GetTypeNode(&ptr_type, s_link);
    if(result == FALSE){
        printf("Not found Sensor type!!!\n");
        return FALSE;
    }
    if(ptr_type->emer_head == NULL){
        ptr_type->emer_head = src;
    }
    else{
        emergency_cmd *ptr_emer = ptr_type->emer_head;
        while(ptr_emer->next != NULL)
            ptr_emer = ptr_emer->next;
        ptr_emer->next = src;
    }
    return TRUE;
}

/*储存传感器参数信息*/
bool SaveItemNode(sensor_item *src, unit_32 s_link)
{
    if(src == NULL){
        printf("Save Item Node is Empty!!!\n");
        return FALSE;
    }
    sensor_type *ptr_type = NULL;
    if(GetTypeNode(&ptr_type, s_link) == FALSE){
        printf("Can not Get Sensor Type Node,s_link invalid!!!\n");
        return FALSE;
    }
    if(ptr_type->item_head == NULL)
        ptr_type->item_head = src;
    else{
        sensor_item *ptr_item = ptr_type->item_head;
        while(ptr_item->next != NULL)
            ptr_item = ptr_item->next;
        ptr_item->next = src;
    }
    return TRUE;
}

/*储存设备信息结点*/
bool SaveDevVersionNode(version *src)
{
    device_para *ptr_dev = DeviceParaPtr();
    if(src == NULL || ptr_dev == NULL){
        printf("Version infor node can not init finished!!!\n");
        return FALSE;
    }
    ptr_dev->version = src;
    return TRUE;
}

/*储存设备功能开关量信息结点*/
bool SaveDevFuncNode(func_switch *src)
{
    device_para *ptr_dev = DeviceParaPtr();
    if(src == NULL || ptr_dev == NULL){
        printf("Device function switch can not init finished!!!\n");
        return FALSE;
    }
    ptr_dev->func = src;
    return TRUE;
}

/*储存控制器结点信息*/
bool SaveCtrlNode(ctrl* src)
{
    if(src == NULL){
        printf("Controller is null,error!!!");
        return FALSE;
    }
    device_para *ptr_dev = DeviceParaPtr();
    if(ptr_dev == NULL)
        return FALSE;
    if(ptr_dev->ctrl_head == NULL)
        ptr_dev->ctrl_head = src;
    else{
        ctrl *ptr_c = ptr_dev->ctrl_head;
        while(ptr_c->next != NULL)
            ptr_c = ptr_c->next;
        ptr_c->next = src;
    }
    return TRUE;
}

/*存储串口交互信息结点*/
bool SaveISPCNode(ispc* src)
{
    if(src == NULL){
        printf("Inter-SerialPort Communication node is Empty!!!");
        return FALSE;
    }
    device_para *ptr_dev = DeviceParaPtr();
    if(ptr_dev == NULL)
        return FALSE;
    if(ptr_dev->ispc_head == NULL)
        ptr_dev->ispc_head = src;
    else{
        ispc *ptr_temp = ptr_dev->ispc_head;
        while(ptr_temp->next != NULL)
            ptr_temp = ptr_temp->next;
        ptr_temp->next = src;
    }
    return TRUE;
}

/*储存串口信息结点*/
bool SaveSerialPortNode(serial_port *src)
{
    if(src == NULL){
        printf("serial port is null,error!!!");
        return FALSE;
    }
    serial_port **port = SerialPort();
    int index = src->port_ith;
    if(index < 0) index = 0;
    if(index > SERIAL_POER_NUM-1) index = SERIAL_POER_NUM - 1;
    port[index] = src;
    return TRUE;
}


/*=============================================#参数初始化函数#=============================================*/
/*初始化传感器类型信息*/
bool InitSensorTypeInfor(char buff[])
{
    sensor_type *ptr_type = (sensor_type *)malloc(sizeof(sensor_type));
    if(ptr_type == NULL){
        printf("Init Para Sensor type malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_type->type_name, 0, 32);
    memset(ptr_type->read_cmd,0,32);
    memset(ptr_type->start_flag, 0, 8);
    ptr_type->fd_port = -1;
    ptr_type->state = 'N';
    ptr_type->item_head = NULL;
    ptr_type->emer_head = NULL;
    ptr_type->next = NULL;

    int pos = 1;
    unit_32 len = 0;
    char *tmp_char = NULL;
    unit_8 src[32];
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //type_name
                memcpy(ptr_type->type_name, str, strlen(str)); break;
            case 2:
                if(!strncmp(str, "0x01", 4))
                    ptr_type->read_mode = READ_PASSIVE;
                else if(!strncmp(str, "0x02", 4))
                    ptr_type->read_mode = READ_ACTIVE;
                break;
            case 3: //Read_cmd
                memset(src, 0, 32);
                len = StringToCmd(src, str, strlen(str));
                memcpy(ptr_type->read_cmd, src, len);
                break;
            case 4: //Read_cmd_len
                ptr_type->len_cmd = atoi(str); break;
            case 5: //Len_data
                ptr_type->len_data = atoi(str); break;
            case 6: //start_flag
                memset(src, 0, 32);
                len = StringToCmd(src, str, strlen(str));
                memcpy(ptr_type->start_flag, src, len);
                break;
            case 7: //len_startflag
                ptr_type->len_startflag = (unit_16)atoi(str); break;
            case 8: //port_ith
                ptr_type->port_ith = atoi(str); break;
            case 9: //crc_module
                ptr_type->crc_module = (unit_16)StringToHex(str, strlen(str)); break;
            case 10: //crc_turn
                ptr_type->crc_turn = (unit_16)StringToHex(str, strlen(str)); break;
            case 11: //Sensor response time
                ptr_type->time_response = (unit_16)atoi(str); break;
            case 12: //sensor_link
                ptr_type->sensor_link = (unit_16)atoi(str); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveTypeNode(ptr_type);
    return result;
}

/*初始化传感器应急指令系统的信息*/
bool InitSensorEmerInfor(char buff[])
{
    emergency_cmd *ptr_emer = (emergency_cmd *)malloc(sizeof(emergency_cmd));
    if(ptr_emer == NULL){
        printf("Init Sensor Emer Para malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_emer->emer_name, 0, 32);
    memset(ptr_emer->cmd, 0, 32);
    ptr_emer->next = NULL;

    int pos = 1;
    unit_32 s_link = 0;
    unit_32 len = 0;
    char *tmp_char = NULL;
    unit_8 src[32];
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch (pos){
            case 1: //sensor link
                s_link = atoi(str); break;
            case 2: //应急指令名称
                memcpy(ptr_emer->emer_name, str, strlen(str)); break;
            case 3: //应急指令
                memset(src, 0, 32);
                len = StringToCmd(src, str, strlen(str));
                memcpy(ptr_emer->cmd, src, len);
                break;
            case 4: //指令长度
                ptr_emer->len_cmd = (unit_16)atoi(str); break;
            case 5: //指令比较起始位
                ptr_emer->start_byte = (unit_16)atoi(str); break;
            case 6: //指令比较长度
                ptr_emer->len_byte = (unit_16)atoi(str); break;
            case 7: //出错识别码
                ptr_emer->emer_code = (unit_16)StringToHex(str, strlen(str)); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveEmerNode(ptr_emer, s_link);
    return result;
}

/*初始化传感器参数的各项信息*/
bool InitSensorItemInfor(char buff[], unit_32 buff_i)
{
    sensor_item *ptr_item = (sensor_item *)malloc(sizeof(sensor_item));
    if(ptr_item == NULL){
        printf("Init Sensor Item Para malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_item->item_name, 0, 32);
    memset(ptr_item->unit_origin, 0, 16);
    memset(ptr_item->unit_trans, 0, 16);
    memset(ptr_item->algo_code, 0, MAX_ALGO_CODE_SIZE);
    ptr_item->buff_ith = buff_i;
    ptr_item->next = NULL;

    int pos = 1;
    unit_32 s_link = 0;
    unit_16 temp_code = 0;
    int temp_num = 0;
    char *tmp_char = NULL;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //sensor link
                s_link = atoi(str); break;
            case 2: //item name
                memcpy(ptr_item->item_name, str, strlen(str)); break;
            case 3: //解析起始字节位置
                ptr_item->start_byte = atoi(str); break;
            case 4: //解析字节长度
                ptr_item->len_byte = atoi(str); break;
            case 5: //解析数据类型
                if(!strncmp(str, "int", 3) || !strncmp(str, "INT", 3))
                    ptr_item->value_type = SIGNED_INT;
                else if(!strncmp(str, "unint", 5) || !strncmp(str, "UNINT", 5))
                    ptr_item->value_type = UNSIGNED_INT;
                if(!strncmp(str, "short", 5) || !strncmp(str, "SHORT", 5))
                    ptr_item->value_type = SIGNED_SHORT;
                else if(!strncmp(str, "unshort", 7) || !strncmp(str, "UNSHORT", 7))
                    ptr_item->value_type = UNSIGNED_SHORT;
                else if(!strncmp(str, "float", 5) || !strncmp(str, "FLOAT", 5))
                    ptr_item->value_type = FLOATING;
                break;
            case 6: //解析字节顺序
                ptr_item->byte_order = (unit_16)StringToHex(str, strlen(str)); break;
            case 7: //缩放因子
                ptr_item->a = atof(str); break;
            case 8: //数据偏移
                ptr_item->b = atof(str); break;
            case 9: //初始数据单位
                memcpy(ptr_item->unit_origin, str, strlen(str)); break;
            case 10://单位转换系数
                ptr_item->unit_ratio = (float)atof(str); break;
            case 11://转换数据单位
                memcpy(ptr_item->unit_trans, str, strlen(str)); break;
            case 12://最小值
                ptr_item->minimum = (float)atof(str); break;
            case 13://最大值
                ptr_item->maximum = (float)atof(str); break;
            case 14: //参数标识码
                temp_code = (unit_16)StringToHex(str, strlen(str));
                if(temp_code < SENSOR_ONE_START || temp_code > SENSIOR_TWO_END){
                    printf("\t!!!Sensor item code(0x%04X)is invalid(%d~%d)\n",
                            temp_code,SENSOR_ONE_START,SENSIOR_TWO_END);
                    return FALSE;
                }else
                    ptr_item->sensor_code =  temp_code;
                break;

            case 15: //算法标识码(注意这里的case标签号与代码的关系)
                temp_num = atoi(str);  //指定的算法个数
                while(str = strtok(NULL, DELIM)){
                    if(*str == '\n' || *str == '\r' || *str == '\0')
                        break;
                    if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
                        *tmp_char = '\0';
                    if((pos-15+1) > MAX_ALGO_CODE_SIZE || (pos-15+1) > temp_num){
                        printf("\t!!!Algo code number(%d) overflow(>%d) or Greater than the specified number(>%d)\n",
                               (pos-15+1),MAX_ALGO_CODE_SIZE, temp_num);
                        break;
                    }
                    temp_code = (unit_16)StringToHex(str, strlen(str));
                    if(temp_code < OPTIMIZE_START || temp_code > OPTIMIZE_END){
                        if(temp_num > 0)
                            temp_num--;
                        printf("\t!!!Algo code(0x%04X) is invalid\n",temp_code);
                    }
                    else{
                        ptr_item->algo_code[pos-15] = temp_code;
                        pos++;
                    }
                }
                if((pos-15) < temp_num)
                    ptr_item->num_algo_code = pos-15;
                else
                    ptr_item->num_algo_code = temp_num;
                memset(&ptr_item->algo_code[ptr_item->num_algo_code], 0, MAX_ALGO_CODE_SIZE-ptr_item->num_algo_code);
                unit_16 *code = ptr_item->algo_code;
                SortCodeMinFirst(&code, 0, ptr_item->num_algo_code-1); //算法优先级排序
                break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveItemNode(ptr_item, s_link);
    return result;
}


/*设备基本参数初始化*/
bool InitDevBaseParaInfor(char buff[])
{
    device_para *ptr_dev = (device_para *)malloc(sizeof(device_para));
    if(ptr_dev == NULL){
        printf("Init device base malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_dev->IP, 0, 16);
    memset(ptr_dev->mask, 0, 16);
    memset(ptr_dev->gateway, 0, 16);
    ptr_dev->func = NULL;
    ptr_dev->version = NULL;
    ptr_dev->ctrl_head = NULL;
    ptr_dev->ispc_head = NULL;

    int pos = 1;
    char *tmp_char = NULL;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //Device IP
                memcpy(ptr_dev->IP, str, strlen(str)); break;
            case 2: //Device Port
                ptr_dev->port = atoi(str); break;
            case 3: //Device IP Mask
                memcpy(ptr_dev->mask, str, strlen(str)); break;
            case 4: //Device IP Gateway
                memcpy(ptr_dev->gateway, str, strlen(str)); break;
            case 5: //heart_beat_time
                ptr_dev->heart_beat_time = (unit_16)atoi(str); break;
            case 6: //cycle_buff_num
                ptr_dev->cycle_buff_num = (unit_16)atoi(str); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SetDeviceParaPtr(&ptr_dev);
    if(result == FALSE)
        printf("Save Device Node failed!!!\n");
    return result;
}

/*设备功能开关量初始化*/
bool InitDevFuncInfor(char buff[])
{
    func_switch *ptr_switch = (func_switch *)malloc(sizeof(func_switch));
    if(ptr_switch == NULL){
        printf("Init Para Func switch malloc failed!!!\n");
        return FALSE;
    }
    int pos = 1;
    char *tmp_char = NULL;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //联动
                ptr_switch->action = IsOrNot(str); break;
            case 2: //视频叠加
                ptr_switch->video_overlay = IsOrNot(str); break;
            case 3: //视频抓拍
                ptr_switch->video_capture = IsOrNot(str); break;
            case 4: //DTU数据通信
                ptr_switch->DTU_trans = IsOrNot(str); break;
            case 5: //网口数据通信
                ptr_switch->net_port_trans = IsOrNot(str); break;
            case 6: //蜂鸣器报警
                ptr_switch->beeper_alarm = IsOrNot(str); break;
            case 7: //文本屏
                ptr_switch->text_screen = IsOrNot(str); break;
            case 8: //校准
                ptr_switch->calibration = IsOrNot(str); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveDevFuncNode(ptr_switch);
    return result;
}

/*基本产品信息初始化*/
bool InitManuInfor(char buff[])
{
    version *ptr_ver = (version *)malloc(sizeof(version));
    sprintf(ptr_ver->software_version, "JoMiDevice %0.1f", (float)DEVICE_VERSION/10);
    if(ptr_ver == NULL){
        printf("Init para device Version malloc is failed!!!\n");
        return FALSE;
    }
    memset(ptr_ver->dev_module, 0, 32);
    memset(ptr_ver->dev_no, 0, 32);
    memset(ptr_ver->production_date, 0, 32);
    memset(ptr_ver->hardware_version, 0, 16);
    memset(ptr_ver->software_version, 0, 16);
    memset(ptr_ver->kernel_version, 0, 16);
    memset(ptr_ver->manufacture_name, 0, 16);

    int pos = 1;
    char *tmp_char = NULL;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //设备型号
                memcpy(ptr_ver->dev_module, str, strlen(str)); break;
            case 2: //设备编号
                memcpy(ptr_ver->dev_no, str, strlen(str)); break;
            case 3: //出厂日期
                memcpy(ptr_ver->production_date, str, strlen(str)); break;
            case 4: //硬件版本
                memcpy(ptr_ver->hardware_version, str, strlen(str)); break;
            case 5: //软件版本
                // memcpy(ptr_ver->software_version, str, strlen(str)); break;
                break;
            case 6: //内核版本
                memcpy(ptr_ver->kernel_version, str, strlen(str)); break;
            case 7: //厂商信息
                memcpy(ptr_ver->manufacture_name, str, strlen(str)); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveDevVersionNode(ptr_ver);
    return result;
}

/*控制器信息初始化*/
bool InitControllerInfor(char buff[], int ith)
{
    ctrl *ptr_ctrl = (ctrl *)malloc(sizeof(ctrl));
    if(ptr_ctrl == NULL){
        printf("Init Para Device Control malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_ctrl->ctrl_name, 0, 32);
    memset(ptr_ctrl->open_order, 0, 64);
    memset(ptr_ctrl->close_order, 0, 64);
    ptr_ctrl->ctrl_ON = ith;
    ptr_ctrl->fd_port = INVALID_FD;
    ptr_ctrl->next = NULL;

    int pos = 1;
    char *tmp_char = NULL;
    unit_32 len = 0;
    unit_8 src[32];
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //联动名称
                memcpy(ptr_ctrl->ctrl_name, str, strlen(str)); break;
            case 2: //使用串口编号
                ptr_ctrl->port_ith = atoi(str); break;
            case 3: //开启指令
                memset(src, 0, 32);
                len = StringToCmd(src, str, strlen(str));
                memcpy(ptr_ctrl->open_order, src, len);
                break;
            case 4: //开启指令长度
                ptr_ctrl->len_open_order = atoi(str); break;
            case 5: //关闭指令
                memset(src, 0, 32);
                len = StringToCmd(src, str, strlen(str));
                memcpy(ptr_ctrl->close_order, src, len);
                break;
            case 6: //关闭指令长度
                ptr_ctrl->len_close_order = atoi(str); break;
            case 7:
                ptr_ctrl->initial_state = IsOrNot(str); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveCtrlNode(ptr_ctrl);
    return result;
}

/*初始化串口交互信息*/
bool InitISPCInfor(char buff[])
{
    ispc *ptr_ispc = (ispc*)malloc(sizeof(ispc));
    if(ptr_ispc == NULL){
        printf("Init ISPC malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_ispc->name, 0, 32);
    ptr_ispc->fd_port = INVALID_FD;
    ptr_ispc->next = NULL;

    int pos = 1;
    char *tmp_char = NULL;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //串口交互名称
                memcpy(ptr_ispc->name, str, strlen(str)); break;
            case 2: //使能位
                ptr_ispc->enable = IsOrNot(str); break;
            case 3: //是否独占
                ptr_ispc->monopoly = IsOrNot(str); break;
            case 4: //串口编号
                ptr_ispc->port_ith = atoi(str); break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveISPCNode(ptr_ispc);
    return result;
}

/*设备串口初始化*/
bool InitSerialPortInfor(char buff[])
{
    serial_port *ptr_port = (serial_port *)malloc(sizeof(serial_port));
    if(ptr_port == NULL){
        printf("Init Para Serial port malloc failed!!!\n");
        return FALSE;
    }
    memset(ptr_port->portName, 0, 32);
    memset(ptr_port->devName, 0, 32);
    ptr_port->fd_port = INVALID_FD;
    ptr_port->state = 'U';
    memset(ptr_port->obj_state, 0, 16);
    ptr_port->next = NULL;

    int pos = 1;
    char *tmp_char = NULL;
    int len_dev_name=0;
    char *str = strtok(buff, DELIM);
    while(str = strtok(NULL, DELIM)){
        if(*str == '\n' || *str == '\r' || *str == '\0')
             break;
        if((tmp_char = strchr(str, '\n')) || (tmp_char = strchr(str, '\r')))
            *tmp_char = '\0';
        switch(pos){
            case 1: //串口编号
                ptr_port->port_ith = atoi(str); break;
            case 2: //端口名称
                memcpy(ptr_port->portName, str, strlen(str)); break;
            case 3: //端口名称长度
                len_dev_name = StringTounit32(str,strlen(str)); break;
            case 4: //串口文件名
                memcpy(ptr_port->devName, str, len_dev_name); break;
            case 5: //波特率
                ptr_port->speed = atoi(str); break;
            case 6: //数据位
                ptr_port->dataBit = atoi(str); break;
            case 7: //停止位
                ptr_port->stopBit = (unit_16)atoi(str); break;
            case 8: //校验位
                ptr_port->parity = *str; break;
            default:
                break;
        }
        pos++;
    }
    bool result = SaveSerialPortNode(ptr_port);
    if(result == FALSE)
        printf("Save Serial port Failed!!!\n");
    return result;
}

/*=================================================#配置初始化#=================================================*/
/*基本信息初始化*/
bool InitDevicePara(FILE *fp)
{
    unit_32 item_i = 0;
    unit_32 type_i = 0;
    int ctrl_ith = 0;
    bool result = FALSE;
    char *buff = (char *)malloc(INIT_BUFF_SIZE);
    if(buff == NULL){
        printf("Init para device malloc failed!!!\n");
        return FALSE;
    }
    memset(buff, 0, INIT_BUFF_SIZE);
    while(fgets(buff, INIT_BUFF_SIZE, fp) != NULL){
        if(!strncmp(buff, "#start", 6) || !strncmp(buff, "start", 5))
            printf("Device layer para Init Start...\n");
        else if(!strncmp(buff, "#end", 4) || !strncmp(buff, "end", 3)){
            printf("Device layer para Init End!\n");
            break;
        }
        else{
            if(!strncmp(buff, "title", 5)){
                // printf("Get title!\n");
            }
            else if(!strncmp(buff, "devi", 4) || !strncmp(buff, "DEVI", 4) ||!strncmp(buff, "device", 6)){
                printf("Get Device base info...\n");
                result = InitDevBaseParaInfor(buff);
                if(result == FALSE){
                    printf("Get Device Infor Error!!!n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "info", 4) || !strncmp(buff, "INFO", 4) || !strncmp(buff, "infor", 5)){
                printf("Get Device information...\n");
                result = InitManuInfor(buff);
                if(result == FALSE){
                    printf("Get Manufacture info Error!!!\n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "func", 4) || !strncmp(buff, "FUNC", 4)){
                printf("Get Device function module...\n");
                result = InitDevFuncInfor(buff);
                if(result == FALSE){
                    printf("Get Device Function switch info Error!!!\n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "port", 4) || !strncmp(buff, "PORT", 4)){
                printf("Get device serial port...\n");
                result = InitSerialPortInfor(buff);
                if(result == FALSE){
                    printf("Get Serial Port info Error!!!\n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "ctrl", 4) || !strncmp(buff, "CTRL", 4)){
                printf("Get Action infor...\n");
                result = InitControllerInfor(buff,ctrl_ith);
                if(result == FALSE){
                    printf("Get action info Error!!!\n");
                    return FALSE;
                }
                ctrl_ith++;
            }
            else if(!strncmp(buff, "ispc", 4) || !strncmp(buff, "ISPC", 4)){
                printf("Get serial port interact infor...\n");
                result = InitISPCInfor(buff);
                if(result == FALSE){
                    printf("Get serial port interact info Error!!!\n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "type", 4) || !strncmp(buff, "TYPE", 4)){
                printf("Get Sensor Type info...\n");
                result = InitSensorTypeInfor(buff);
                if(result == FALSE){
                    printf("Get Sensor Type info Error!!!\n");
                    return FALSE;
                }
                type_i++;
            }
            else if(!strncmp(buff, "emer", 4) || !strncmp(buff, "EMER", 4)){
                printf("Get Sensor Emergency info...\n");
                result = InitSensorEmerInfor(buff);
                if(result == FALSE){
                    printf("Get Sensor emergency info Error!!!\n");
                    return FALSE;
                }
            }
            else if(!strncmp(buff, "item", 4) || !strncmp(buff, "ITEM", 4)){
                printf("Get Sensor item info...\n");
                result = InitSensorItemInfor(buff,item_i);
                if(result == FALSE){
                    printf("Get Sensor Item Info Error!!!\n");
                    return FALSE;
                }
                item_i++;
            }
        }
        memset(buff, 0, INIT_BUFF_SIZE);
    }
    SetTypeTotalNum(type_i);
    SetItemTotalNum(item_i);
    free(buff);
    return TRUE;
}

/*双缓存初始化*/
bool InitDataFBuffer()
{
    dual_buff *buff_one = (dual_buff *)malloc(sizeof(dual_buff));
    if(buff_one == NULL){
        printf("Cannot allocate more space for buff_one");
        return FALSE;
    }
    buff_one->flag = 1;
    memset(buff_one->buff, 0.0, MAX_BUFFER_SIZE);
    memset(buff_one->m_time, 0, MAX_ITEM_SIZE);

    dual_buff *buff_two = (dual_buff *)malloc(sizeof(dual_buff));
    if(buff_two == NULL){
        printf("Cannot allocate more space for buff_two");
        return FALSE;
    }
    buff_two->flag = 2;
    memset(buff_two->buff, 0.0, MAX_BUFFER_SIZE);
    memset(buff_two->m_time, 0, MAX_ITEM_SIZE);

    buff_one->next = buff_two;
    buff_two->next = buff_one;

    SetBuffOne(buff_one);
    SetBuffTwo(buff_two);
    ChangeBuffPush(buff_one);
    ChangeBuffPull(buff_two);

    /*双缓冲超时时间 = 所有传感器轮询一遍的时间 * 缓存存储遍数 + 等待时间*/
    int ex_overtime = 0;
    ex_overtime = ItemTotalNum() * TIME_INTERVAL;
    ex_overtime *= CycleBuffNum();
    ex_overtime += 2;
    SetExchangeOverTime(ex_overtime);
    return TRUE;
}

/*串口配置:只要有一个串口可用就返回TRUE，否则全部串口都不可用就返回FALSE*/
bool SensorPortConfigure()
{
    printf("\n========================#Serial port configure#========================\n");
    bool result = FALSE;
    serial_port **port = SerialPort();
    int i = 0;
    for(i=0; i<SERIAL_POER_NUM; i++){
        if(port[i] != NULL){
            int fd = open(port[i]->devName, O_RDWR | O_NOCTTY);
            if(fd == INVALID_FD){
                printf("The %s serial port open fail!!!\n", port[i]->portName);
                port[i]->state = UNINITIALIZED;
            }
            else{
                port[i]->fd_port = fd;
                printf("%s: Port Fd=%d; ", port[i]->portName, fd);
                if(SetSpeed(fd, port[i]->speed) == TRUE &&
                SetSerialPortAttributes(fd, port[i]->dataBit, port[i]->stopBit, port[i]->parity) == TRUE){
                    printf("Speed=%d; DataBit=%d; StopBit=%d; Parity=%c\n",
                    port[i]->speed, port[i]->dataBit, port[i]->stopBit, port[i]->parity);
                    result = TRUE;
                    port[i]->state = NORMAL;
                }else
                    printf("Serial port attribute configuration failed!!!\n");
            }
        }
    }
    return result;
}

/*分配串口文件描述符*/
bool DistributePortFd()
{
    bool result = FALSE;
    int32_t fd_port = INVALID_FD;
    printf("\n--------------------------#File descriptor allocation#--------------------------\n");
    /*初始化串口交互文件描述符*/
    device_para *ptr_dev = DeviceParaPtr();
    ispc* ptr_ispc = ptr_dev->ispc_head;
    while(ptr_ispc != NULL && ptr_ispc->enable == ON){
        serial_port **port = SerialPort();
        fd_port = port[ptr_ispc->port_ith]->fd_port;
        if(ptr_ispc->monopoly == ON){
            port[ptr_ispc->port_ith]->state = MONOPOLY;
            memcpy(port[ptr_ispc->port_ith]->obj_state,ptr_ispc->name,strlen(ptr_ispc->name));
        }
        if(fd_port == INVALID_FD)
            printf("Cannot assign Fd for %s!!!\n", ptr_ispc->name);
        else{
            ptr_ispc->fd_port = fd_port;
            printf("%-12s: Assign to (Fd= %d )\n", ptr_ispc->name,ptr_ispc->fd_port);
            result = TRUE;
        }
        ptr_ispc = ptr_ispc->next;
    }

    /*初始化传感器串口文件描述符*/
    sensor_type *ptr_type = TypeLinkHead();
    while(ptr_type != NULL){
        fd_port = GetSerialPortFd(ptr_type->port_ith);
        if(fd_port == INVALID_FD)
            printf("Cannot assign Fd for %s!!!\n", ptr_type->type_name);
        else{
            ptr_type->fd_port = fd_port;
            printf("%-12s: Assign to (Fd= %d )\n", ptr_type->type_name,ptr_type->fd_port);
            result = TRUE;
        }
        ptr_type = ptr_type->next;
    }

    /*初始化控制器串口文件描述符*/
    fd_port = INVALID_FD;
    ctrl *ptr_ctrl = ptr_dev->ctrl_head;
    while(ptr_ctrl != NULL){
            fd_port = GetSerialPortFd(ptr_ctrl->port_ith);
            if(fd_port == INVALID_FD)
                printf("Cannot assign Fd for %s!!!\n", ptr_ctrl->ctrl_name);
            else{
                ptr_ctrl->fd_port = fd_port;
                printf("%-12s: Assign to (Fd= %d )\n", ptr_ctrl->ctrl_name,ptr_ctrl->fd_port);
                result = TRUE;
            }
        ptr_ctrl = ptr_ctrl->next;
    }
    return result;
}

/*设置网络*/
bool SetNet()
{
    FILE *file;
    bool result;
    char cmd[64];
    device_para *ptr_dev = DeviceParaPtr();
    printf("\n--------------------------#Set_Local_NetWork#--------------------------\n");
    memset(cmd, 'a', 64);
    file = fopen("/etc/resolv.conf", "a+");
    if(file != NULL){
        sprintf(&cmd[0], "nameserver %s\n", ptr_dev->gateway);
        fprintf(file, "%s\n", cmd);
        fprintf(file, "%s", "nameserver 114.114.114.114");
        fclose(file);
        printf("set into \"/etc/resolv.conf\":\n\tnameserver %s\n\tnameserver 114.114.114.114\n", ptr_dev->gateway);
    }
    else{
        printf("Set nameserver fail!!!\n");
        return FALSE;
    }

    memset(cmd, 'a', 64);
    sprintf(&cmd[0], "ifconfig eth0 %s", ptr_dev->IP);
    result = commandExecute(cmd, "w");
    if(result == FALSE){
        printf("set IP fail!!!\n");
        return FALSE;
    }else
        printf("set ip success: %s\n",cmd);

    memset(cmd, 'a', 64);
    sprintf(&cmd[0], "ifconfig eth0 netmask %s", ptr_dev->mask);
    result = commandExecute(cmd, "w");
    if(result == FALSE){
        printf("set mask fail!!!\n");
        return FALSE;
    }else
        printf("set mask success: %s\n",cmd);

    memset(cmd, 'a', 64);
    sprintf(&cmd[0], "route add default gw %s", ptr_dev->gateway);
    result = commandExecute(cmd, "w");
    if(result == FALSE){
        printf("set gateway fail!!!\n");
        return FALSE;
    }else
        printf("set gateway success: %s\n",cmd);

    commandExecute("ifconfig lo up", "w");
    return TRUE;
}


/*设备系统参数及配置初始化*/
bool InitParament()
{
    FILE *fp = NULL;
    fp = fopen(DEVICE_INFO_PATH, "r");
    if(fp == NULL){
        printf("Init Device Parament fail,please check \"%s\"\n", DEVICE_INFO_PATH);
        return FALSE;
    }
    bool result = InitDevicePara(fp);
    fclose(fp);
    if(result == FALSE){
        printf("There is a problem during device initialization, Please check the configuration file:\"%s\" \n", DEVICE_INFO_PATH);
        return FALSE;
    }

    printDeviceInfor();
    printSerailPortInfo();
    printSensorInfomation();

    if(InitDataFBuffer() == FALSE){
        printf("Failed to allocate space for dual buffer!!!\n");
        return FALSE;
    }
    if(SensorPortConfigure() == FALSE){
        printf("Serial port configuration failed. Please check the Hardware and configuration file information\n");
        return FALSE;
    }
    if(DistributePortFd() == FALSE){
        printf("The sensor or controller cannot allocate the serial port correctly, Please check the configuration file and device!!!\n");
        return FALSE;
    }
    if(SetNet() == FALSE){
        printf("An error occurred while configuring the local network information!!!\n");
        return FALSE;
    }

    return TRUE;
}



/*======================================================================================================================
|   |   |   |   |   |   |   |   |   |   |   |设备系统初始化程序部分|   |   |   |   |   |   |   |   |   |   |   |   |   |   |
-----------------------------------------------------------------------------------------------------------------------*/
/*串口间通信状态*/
bool ISPCFlag()
{
    ispc *ptr_ispc = DeviceParaPtr()->ispc_head;
    bool ispc_flag = FALSE;
    while(ptr_ispc != NULL){
        if(ptr_ispc->enable == ON){
            ispc_flag = TRUE;
            break;
        }
        ptr_ispc = ptr_ispc->next;
    }
    return ispc_flag;
}

void SignalPthread_PL()
{
    pthread_mutex_lock(&lock_PL);
    if (IsWait(wait_PL_status))
    {
        wait_PL_status = FALSE;
        printStr("weak up Sensor Perceive Layer Thread!");
        pthread_cond_signal(&pthread_cond_PL);
    }
    pthread_mutex_unlock(&lock_PL);
}

void SignalPthread_IPCOne()
{
    pthread_mutex_lock(&lock_IPC1);
    if (IsWait(wait_IPC1_status))
    {
        wait_IPC1_status = FALSE;
        pthread_cond_signal(&pthread_cond_IPC1);
        printStr("weak up System Function Layer Thread!");
    }
    pthread_mutex_unlock(&lock_IPC1);
}

void SignalPthread_AL()
{
    pthread_mutex_lock(&lock_AL);
    if (IsWait(wait_AL_status))
    {
        wait_AL_status = FALSE;
        pthread_cond_signal(&pthread_cond_AL);
        printStr("weak up Algorithm Handling Thread!");
    }
    pthread_mutex_unlock(&lock_AL);
}

void SignalPthread_LM()
{
    pthread_mutex_lock(&lock_LM);
    if (IsWait(wait_LM_status))
    {
        wait_LM_status = FALSE;
        pthread_cond_signal(&pthread_cond_LM);
        printStr("weak up Local Module Thread!");
    }
    pthread_mutex_unlock(&lock_LM);
}

void SignalPthread_ISPC()
{
    pthread_mutex_lock(&lock_ISPC);
    if(IsWait(wait_ISPC_status))
    {
        wait_ISPC_status = FALSE;
        pthread_cond_signal(&pthread_cond_ISPC);
        printStr("weak up Inter-SPC Thread!");
    }
    pthread_mutex_unlock(&lock_ISPC);
}

/*唤醒所有阻塞线程*/
void Pthread_Cond_Broadcast()
{
    SignalPthread_IPCOne();
    SignalPthread_AL();
    SignalPthread_PL();
    SignalPthread_LM();
    if(ISPCFlag() == TRUE)
        SignalPthread_ISPC();
}

/*线程条件变量及互斥量初始化*/
void CondAndMutexInit()
{
    pthread_cond_init(&pthread_cond_PL, NULL);
    pthread_mutex_init(&lock_PL, NULL);
    pthread_cond_init(&pthread_cond_IPC1, NULL);
    pthread_mutex_init(&lock_IPC1, NULL);
    pthread_cond_init(&pthread_cond_AL, NULL);
    pthread_mutex_init(&lock_AL, NULL);
    pthread_cond_init(&pthread_cond_LM, NULL);
    pthread_mutex_init(&lock_LM, NULL);
    pthread_cond_init(&pthread_cond_ISPC, NULL);
    pthread_mutex_init(&lock_ISPC, NULL);

    wait_PL_status = FALSE;
    wait_IPC1_status = FALSE;
    wait_AL_status = FALSE;
    wait_LM_status = FALSE;
    wait_ISPC_status = FALSE;
}

/*主管理程序初始化*/
bool DeviceInit()
{
    printStr("\r-----------------------------------------------------------------------");
    printStr("\r Enter The Device Main Manage System; Device System Initialize Start...");
    printStr("\r-----------------------------------------------------------------------");

    int err;
    pthread_t tid_perception; /*传感器感知层线程*/
    pthread_t tid_sys_func;   /*系统层管理现存*/
    pthread_t tid_algorithm;  /*算法解析层线程*/
    pthread_t tid_local;      /*本地模块*/
    pthread_t tid_ispc;

    CondAndMutexInit(); /*线程条件变量及互斥量初始化*/

    err = pthread_create(&tid_perception, NULL, PerceiveLayerManage, NULL);
    if(err != 0){
        perror("Can Not Create Sensor Perceive pthread!!!");
        return FALSE;
    }
    sleep(1);
    pthread_detach(tid_perception);

    err = pthread_create(&tid_sys_func, NULL, IPCOne, NULL);
    if(err != 0){
        perror("Can Not Create Systematic Func pthread!!!");
        return FALSE;
    }
    sleep(1);
    pthread_detach(tid_sys_func);

    if(ISPCFlag() == TRUE){
        err = pthread_create(&tid_ispc, NULL, ISPCManage, NULL);
        if (err != 0){
            perror("Can Not Create Local module pthread!!!");
            return FALSE;
        }
        sleep(1);
        pthread_detach(tid_ispc);
    }

    err = pthread_create(&tid_algorithm, NULL, AlgorithmManage, NULL);
    if(err != 0){
        perror("Can Not Create Data Optimization Algorithm pthread!!!");
        return FALSE;
    }
    sleep(1);
    pthread_detach(tid_algorithm);

    err = pthread_create(&tid_local, NULL, LocalManage, NULL);
    if(err != 0){
        perror("Can Not Create Local module pthread!!!");
        return FALSE;
    }
    sleep(1);
    pthread_detach(tid_local);

    return TRUE;
}

