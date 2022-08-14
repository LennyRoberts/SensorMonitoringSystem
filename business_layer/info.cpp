#include "info.hpp"

using namespace std;

JInformation jInfo;    //参数信息类全局对象
pthread_t tid_device;  //与设备层交互线程
pthread_t tid_centre;  //与服务器交互线程
pthread_t tid_wincc;   //与上位机交互线程
pthread_t tid_server;  //本机作为服务器交互线程

/*用于控制各个线程的变量*/
/*--------------------------------------------------*/
pthread_cond_t pthread_cond_Dev;     //与设备层交互线程
pthread_mutex_t lock_Dev;
bool wait_Dev_status;
pthread_cond_t pthread_cond_Centre;  //与服务器交互线程
pthread_mutex_t lock_Centre;
bool wait_Centre_status;
pthread_cond_t pthread_cond_Wincc;   //与上位机交互线程
pthread_mutex_t lock_Wincc;
bool wait_Wincc_status;
pthread_cond_t pthread_cond_Server;  //本机作为服务器交互线程
pthread_mutex_t lock_Server;
bool wait_Server_status;

/*=======================================================================*/
JObject::JObject(){}
JObject::~JObject(){}

/******************************************************************************
 * Name:    CRC-16/MODBUS       x16+x15+x2+1
 * Poly:    0x8005
 * Init:    0xFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0x0000
 * Note:
 *****************************************************************************/
unsigned short JObject::crc16_modbus(unsigned char *data, unsigned short length)
{
	unsigned char i;
	unsigned short crc = 0xffff;  // Initial value
	while(length--){
		crc ^= *data++;           // crc ^= *data; data++;
		for(i = 0; i < 8; ++i){
			if(crc & 1)
				crc = (crc >> 1) ^ 0xA001; // 0xA001 = reverse 0x8005
			else
				crc = (crc >> 1);
		}
	}
	return crc;
}

/*指令CRC生成*/
void JObject::CrcGenerator(char *data, int len_cmd)
{
    // unsigned char cmd_buff[len_cmd];
    // memset(cmd_buff, 0, len_cmd);
    // for(int i=0; i<len_cmd; i++){
    //     cmd_buff[i] = (unsigned char)data[i];
    // }
    // unsigned short crc = crc16_modbus(cmd_buff, len_cmd-2);
    // data[len_cmd-1-1] = (char)crc >> 8 & 0xFF;
    // data[len_cmd-1] = (char)crc & 0xFF;
    data[len_cmd-1-1] = 0xFF;
    data[len_cmd-1] = 0xFE;
}

/*指令CRC校验*/
bool JObject::CrcCheck(char *data, int len_cmd)
{
    unsigned char cmd_buff[len_cmd];
    memset(cmd_buff, 0, len_cmd);
    for(int i=0; i<len_cmd; i++){
        cmd_buff[i] = (unsigned char)data[i];
    }
    unsigned short origin_crc = 0;
    origin_crc |= cmd_buff[len_cmd-1-1] << 8;
    origin_crc |= cmd_buff[len_cmd-1];
    unsigned short crc = crc16_modbus(cmd_buff, len_cmd-2);
    if(crc == origin_crc)
        return true;
    else
        return false;
}

/*I/O多路复用poll，超时检测当前文件描述符是否准备好*/
bool JObject::CheckFdStatus(int fd, int rw_status, int32_t overtime_ms)
{
	int ret = 0;
	bool result = false;
	struct pollfd fds[1];
	if(fd == INVALID_FD)
		return false;
	fds[0].fd = fd;                    /*仅监听当前串口*/
	if(rw_status == 2)
		fds[0].events = POLLOUT;
	else
		fds[0].events = POLLIN;
	ret = poll(fds, 1, overtime_ms);   /*I/O多路复用*/
	if(ret == -1)
		perror("Poll");
	else if(ret == 0){
		result = false;
	}
	else if(ret > 0){
		if(rw_status == 2){
			if((fds[0].revents & POLLOUT) == POLLOUT)
				result = true;
		}
		else{
			if((fds[0].revents & POLLIN) == POLLIN)
				result = true;
		}
	}
	return result;
}

/*---------------------------------------------------------#cmdRegulation#---------------------------------------------------------*/
cmdRegulation::cmdRegulation() {}
cmdRegulation::~cmdRegulation() {}

/*指令修改*/
void cmdRegulation::CmdModify(char *cmd, char func_code)
{
    CmdFuncCode(cmd, func_code);
}

void cmdRegulation::CmdModify(char *cmd, short addr1, short addr2)
{
    CmdAddress1(cmd, addr1);
    CmdAddress2(cmd, addr2);
}

void cmdRegulation::CmdModify(char *cmd, float data)
{
    CmdData(cmd, data);
}

/*指令组成函数*/
void cmdRegulation::CmdGenerator(char *cmd, char version, char module_addr, char func_code, short addr1, short addr2, float data)
{
    memset(cmd, 0, CMD_SIZE);
    CmdHeadTail(cmd);
    CmdVersion(cmd, version);
    CmdModuleAddr(cmd,module_addr);
    CmdFuncCode(cmd, func_code);
    CmdDataLenght(cmd);
    CmdAddress1(cmd, addr1);
    CmdAddress2(cmd, addr2);
    CmdData(cmd, data);
    CrcGenerator(cmd, CMD_SIZE-1);
}

/*封装指令头尾*/
void cmdRegulation::CmdHeadTail(char *cmd)
{
    cmd[HEAD_POS] = (char)CMD_HEAD & 0xFF;
    cmd[TAIL_POS] = (char)CMD_TAIL & 0xFF;
}
/*封装版本号*/
void cmdRegulation::CmdVersion(char *cmd, char version)
{
    cmd[VERSION_POS] = version & 0xFF;
}

/*封装模块地址*/
void cmdRegulation::CmdModuleAddr(char *cmd, char module)
{
    cmd[MODULE_POS] = module & 0xFF;
}

/*封装指令功能位*/
void cmdRegulation::CmdFuncCode(char *cmd, char func_code)
{
    cmd[FUNC_POS] = (char)func_code & 0xFF;
}

/*封装数据长度*/
void cmdRegulation::CmdDataLenght(char *cmd)
{
    cmd[LENGHT_POS] = (char)LENGHT_DATA;
}

/*封装指令地址１*/
void cmdRegulation::CmdAddress1(char *cmd, short addr1)
{
    for(int i=0; i<ADDR_1_SIZE; i++)
        cmd[ADDR_POS_1 + i] |= (char)(addr1 >> 8*(ADDR_1_SIZE-i-1)) & 0xFF;
}

/*封装指令地址2*/
void cmdRegulation::CmdAddress2(char *cmd, short addr2)
{
    for(int i=0; i<ADDR_2_SIZE; i++)
        cmd[ADDR_POS_2 + i] |= (char)(addr2 >> 8*(ADDR_2_SIZE-i-1)) & 0xFF;
}

/*封装指令数据字段*/
void cmdRegulation::CmdData(char *cmd, float data)
{
    int result = (int)(data*DATA_DEVIDE);
    for(int i=0; i<DATA_SIZE; i++)
        cmd[DATA_POS + i] |= (char)(result >> 8*(DATA_SIZE-i-1)) & 0xFF;
}


/*------------------------------------------------------#JInformation#------------------------------------------------------*/
JInformation::JInformation(){this->buff_size = 512;}
JInformation::~JInformation(){ }

/*业务层参数初始化*/
bool JInformation::InitParameters()
{
    unsigned short server_No = 0;
    char buff[this->buff_size + 10];
    std::vector<JCtrPara> ctr_buff;
    ifstream inFile("./ser_info.csv",ios::in);
    if(!inFile){
        cout<<"Can not open file!!!"<< endl;
        return false;
    }
    else{
        while(inFile.getline(buff, this->buff_size)){
            std::vector<string> res = splitChar(buff, this->buff_size, ",");
            if(res[0] == "#start"){
                cout << "Get Business Parameters start..." << endl;
            }
            else if(res[0] == "#end"){
                cout << "Get Business Parameters finish!" << endl;
            }
            else if(res[0] == "title"){
                // cout << "Get title" << endl;
            }
            else if(res[0] == "inet"){
                cout << "Get Server Infomation..." << endl;
                InitServerInfo(res, server_No, inFile);
                server_No++;
            }
            else if(res[0] == "para"){
                cout << "Get Control parameters..." << endl;
                BuffCtrParaInfo(ctr_buff, res);
            }
        }
    }
    InitCtrParaInfo(ctr_buff);
    vector<JCtrPara>().swap(ctr_buff);
    return true;
}

/*初始化服务端连接相关信息*/
void JInformation::InitServerInfo(vector<string> res, unsigned short ser_No, ifstream &inFile)
{
    /*服务器信息*/
    JServerInfo *ser_node = new JServerInfo();
    ser_node->fd_sock = INVALID_FD;
    ser_node->sever_No = ser_No;
    ser_node->name = res[1];
    ser_node->type = res[2];
    if(res[3] == "ON" || res[3] == "on" || res[3] == "On")
        ser_node->ser_en = ON;
    else if(res[3] == "OFF" || res[3] == "off" || res[3] == "Off")
        ser_node->ser_en = OFF;
    if(res[4] == "ON" || res[4] == "on" || res[4] == "On")
        ser_node->alarm_en = ON;
    else if(res[4] == "OFF" || res[4] == "off" || res[4] == "Off")
        ser_node->alarm_en = OFF;
    strncpy(ser_node->net_IP,res[5].c_str(),res[5].length());
    ser_node->net_port = (unsigned int)std::stoi(res[6]);
    ser_node->num_item = (unsigned short)std::stoi(res[7]);
    if(res[8] == "ON" || res[8] == "on" || res[8] == "On")
        ser_node->real_time = ON;
    else if(res[8] == "OFF" || res[8] == "off" || res[8] == "Off")
        ser_node->real_time = OFF;
    ser_node->time_minute_one = (unsigned short)std::stoi(res[9]);
    ser_node->time_minute_two = (unsigned short)std::stoi(res[10]);
    ser_node->time_hour_one = (unsigned short)std::stoi(res[11]);
    ser_node->time_hour_two = (unsigned short)std::stoi(res[12]);
    ser_node->time_day_one = (unsigned short)std::stoi(res[13]);
    ser_node->time_day_two = (unsigned short)std::stoi(res[14]);
    ser_node->packet_code = (unsigned short)std::stoi(res[15]);
    for(int i=16; i<res.size(); i++){
        ser_node->package_info.push_back(res[i]);
    }
    ser_node->item.reserve(ser_node->num_item);
    /*上传参数信息*/
    for(unsigned short i=0; i<ser_node->num_item; ++i){
        char buff[buff_size + 10];
        if(inFile.getline(buff,buff_size)){
            std::vector<string> res_item = splitChar(buff, buff_size, ",");
            if(res_item[0] != "item")
                continue;
            else{
                cout << "Get Item information..." << endl;
                JItemInfor *item_node = new JItemInfor();
                item_node->item = res_item[1];
                item_node->item_code = StrToHex(res_item[2].replace(0,2,"00"));
                if(res[3] == "ON" || res[3] == "on" || res[3] == "On")
                    item_node->item_en = ON;
                else if(res[3] == "OFF" || res[3] == "off" || res[3] == "Off")
                    item_node->item_en = OFF;
                item_node->table = res_item[4];
                item_node->precision = (unsigned short)stoi(res_item[5]);
                item_node->mode_statistics = StrToHex(res_item[6].replace(0,2,"00"));

                item_node->minute_one.minimum = 99999;
                item_node->minute_one.maximum = 0-99999;
                item_node->minute_one.sum = 0;
                item_node->minute_one.counter = 0;

                item_node->minute_two.minimum = 99999;
                item_node->minute_two.maximum = 0-99999;
                item_node->minute_two.sum = 0;
                item_node->minute_two.counter = 0;

                item_node->hour_one.minimum = 99999;
                item_node->hour_one.maximum = 0-99999;
                item_node->hour_one.sum = 0;
                item_node->hour_one.counter = 0;

                item_node->hour_two.minimum = 99999;
                item_node->hour_two.maximum = 0-99999;
                item_node->hour_two.sum = 0;
                item_node->hour_two.counter = 0;

                item_node->day_one.minimum = 99999;
                item_node->day_one.maximum = 0-99999;
                item_node->day_one.sum = 0;
                item_node->day_one.counter = 0;

                item_node->day_two.minimum = 99999;
                item_node->day_two.maximum = 0-99999;
                item_node->day_two.sum = 0;
                item_node->day_two.counter = 0;

                item_node->counter_overflow = 0;
                item_node->counter_warning = 0;
                item_node->flag_alarm = OFF;
                item_node->flag_sleep = OFF;
                ser_node->item.push_back(*item_node);
            }
        }
    }
    this->server_para.push_back(*ser_node);
}

/*获取检测控制时段信息*/
void JInformation::ParaCtlTime(JCtrPara &para_node, string str)
{
    std::vector<string> res = split(str,"&&");
    std::vector<string> ti;
    std::vector<string> ti_hm;
    ti = split(res[0],"~");
    ti_hm = split(ti[0],":");
    para_node.DST_start.hour = (unsigned short)stoi(ti_hm[0].replace(0,5,"00000"));
    para_node.DST_start.min = (unsigned short)stoi(ti_hm[1]);
    ti_hm = split(ti[1],":");
    para_node.DST_end.hour = (unsigned short)stoi(ti_hm[0]);
    para_node.DST_end.min = (unsigned short)stoi(ti_hm[1].replace(2,1,"0"))/10;

    ti = split(res[1],"~");
    ti_hm = split(ti[0],":");
    para_node.ST_start.hour = (unsigned short)stoi(ti_hm[0].replace(0,4,"0000"));
    para_node.ST_start.min = (unsigned short)stoi(ti_hm[1]);
    ti_hm = split(ti[1],":");
    para_node.ST_end.hour = (unsigned short)stoi(ti_hm[0]);
    para_node.ST_end.min = (unsigned short)stoi(ti_hm[1].replace(2,1,"0"))/10;
}

/*初始化控制参数信息*/
void JInformation::BuffCtrParaInfo(vector<JCtrPara> &ctr_buff, vector<string> res)
{
    JCtrPara *para_node = new JCtrPara();
    para_node->item_code = StrToHex(res[1].replace(0,2,"00"));
    if(res[2] == "ON" || res[2] == "on" || res[2] == "On")
        para_node->enable = ON;
    else if (res[2] == "OFF" || res[2] == "off" || res[2] == "Off")
        para_node->enable = OFF;
    else 
        para_node->enable = OFF;
    para_node->slot = res[3];
    ParaCtlTime(*para_node, res[4]);
    strncpy(&para_node->mode,res[5].c_str(),res[5].length());
    para_node->warning_value = stof(res[6]);
    para_node->alarm_value = stof(res[7]);
    para_node->cycle_check = (unsigned short)stoi(res[8]);
    para_node->hit_rate = stof(res[9]);
    para_node->delay_switching = (unsigned short)stoi(res[10]);
    para_node->hold_period = (unsigned short)stoi(res[11]);
    para_node->dormant_period =  (unsigned short)stoi(res[12]);
    para_node->cancel_value = stof(res[13]);
    para_node->warning_action = StrToHex(res[14].replace(0,2,"00"));
    para_node->alarm_action = StrToHex(res[15].replace(0,2,"00"));
    para_node->cancel_action = StrToHex(res[16].replace(0,2,"00"));
    ctr_buff.push_back(*para_node);
}

/*按从头开始首次出现服务器中控制参数标识为ON的传感器子项参数来初始化控制参数列表*/
void JInformation::InitCtrParaInfo(std::vector<JCtrPara> &ctr)
{
    /*找到控制参数为ON的服务器列表*/
    for(int i=0; i< this->server_para.size(); ++i){
        if(this->server_para[i].alarm_en == ON){
            this->enable_ser_ith = i;
            break;
        }
    }

    /*只选取控制参数为ON的服务器列表中的传感器子项参数*/
    std::vector<JItemInfor>::iterator it;
    for(it=this->server_para[enable_ser_ith].item.begin(); it!=this->server_para[enable_ser_ith].item.end(); it++){
        std::vector<JCtrPara>::iterator cit;
        for(cit = ctr.begin(); cit != ctr.end(); cit++){
            if((*it).item_code==(*cit).item_code){
                this->ctr_para.push_back(*cit);
            }
        }
    }
}


/*打印输出服务器配置信息*/
void JInformation::printSeverInfor(){
    vector<JServerInfo>::iterator it;
    for(it = this->server_para.begin(); it != this->server_para.end(); it++){
        cout << setfill(' ');
        cout.flags(ios::left);
        cout << endl;
        cout << "                                         " << (*it).name << " Server Paramaters informations" << endl;
        cout << "===========================================================================================================================================" << endl;
        cout << "|NO|   Name  |  Type   |S_Y|A_Y|       IP       | Port |i_num|     Packet_Format_Code    |real|MinOne|MinTwo|HourOne|HourTwo|DayOne|DayTwo|" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << " ";
        cout.width(3); cout << (*it).sever_No;
        cout.width(10); cout << (*it).name;
        cout.width(10); cout << (*it).type;
        cout.width(4); 
        if((*it).ser_en == ON) cout << "ON";
        else if((*it).ser_en == OFF) cout << "OFF";
        cout.width(4); 
        if((*it).alarm_en == ON) cout << "ON";
        else if((*it).alarm_en == OFF) cout << "OFF";
        cout.width(17); cout << (*it).net_IP;
        cout.width(7); cout << (*it).net_port;
        cout.width(6); cout << (*it).num_item;
        switch((*it).packet_code){
            case SURFACE_WATER_QUALITY:
                cout.width(28); cout << "surface_water_quality"; break;
            case AIR_QUALITY:
                cout.width(28); cout << "air_quality"; break;
            default:
                cout.width(28); cout << "NotFound!!!"; break;
        }
        if((*it).real_time == ON){cout.width(5); cout << "ON";}
        else if((*it).real_time == OFF){cout.width(5); cout << "OFF";}
        else{cout.width(5); cout << "NULL";}
        cout.width(7); cout << (*it).time_minute_one;
        cout.width(7); cout << (*it).time_minute_two;
        cout.width(8); cout << (*it).time_hour_one;
        cout.width(8); cout << (*it).time_hour_two;
        cout.width(7); cout << (*it).time_day_one;  
        cout.width(7); cout << (*it).time_day_two;
        cout << endl;
        for(int i=0;i<(*it).package_info.size();++i)
            cout << (*it).package_info[i] << "; ";
        cout << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "|  Item  |  Label  |Precision|Enable| Code |ModeStatistics|" << endl;
        cout << "-----------------------------------------------------------" << endl;
        vector<JItemInfor>::iterator it_item;
        for(it_item = (*it).item.begin(); it_item != (*it).item.end(); ++it_item){
            cout << setfill(' ');
            cout.flags (ios::left);
            cout.width(1); cout << " ";
            cout.width(9); cout << (*it_item).item;
            cout.width(10); cout << (*it_item).table;
            cout.width(10); cout << (*it_item).precision;
            cout.width(7); 
            if((*it_item).item_en == ON) cout << "ON";
            else if((*it_item).item_en == OFF) cout << "OFF";
            else cout << "NULL";
            cout << setfill('0');
            cout.flags (ios::internal);
            cout << setw(6) << hex << uppercase << showbase << (*it_item).item_code << setw(1) << " ";
            cout << setfill(' ');
            cout.flags (ios::left);
            string cal_mode = FindAvgCalculateMode((*it_item).mode_statistics); cout.width(15); cout << cal_mode;
            cout << endl;
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "===========================================================================================================================================" << endl;
    }
}

/*查找均值计算方式*/
string JInformation::FindAvgCalculateMode(unsigned short mode_opt)
{
    switch(mode_opt){
        case ARITHMETIC_MEAN:
            return "Average";
        case MOVING_AVERAGE:
            return "MovingAvg";
        case CUMULATIVE_VALUE:
            return "Cumulative";
        default:
            return "Disnable";
    }
}

/*打印输出控制参数信息*/
void JInformation::printCtrParaInfo(std::vector<JCtrPara> &ctr)
{
    vector<JCtrPara>::iterator it;
    cout << setfill(' ');
    cout.flags(ios::left);
    cout << "\n                                                      Control Paramaters informations" << endl;
    cout << "=============================================================================================================================================" << endl;
    cout << "|  Slot   |Enable|Mode|Warning|Alarm  |Cycle|HitRate|Delay|Hold |Dormant|Cancle| code  | W_Act | A_Act | C_Act |     DST     | StandardTime |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    for(it = ctr.begin(); it != ctr.end(); ++it){
        cout << setfill(' ');
        cout.flags(ios::left);
        cout << " ";
        cout.width(10); cout << (*it).slot;
        cout.width(7); 
        if((*it).enable == ON) cout << "ON";
        else if((*it).enable == OFF) cout << "OFF";
        cout.width(5); cout << (*it).mode;
        cout.width(8); cout << (*it).warning_value;
        cout.width(8); cout << (*it).alarm_value;
        cout.width(6); cout << (*it).cycle_check;
        cout.width(8); cout << (*it).hit_rate;
        cout.width(6); cout << (*it).delay_switching;
        cout.width(6); cout << (*it).hold_period;
        cout.width(8); cout << (*it).dormant_period;
        cout.width(7); cout << (*it).cancel_value;
        cout << setfill('0');
        cout.flags (ios::internal);
        cout.width(6); cout << hex << uppercase << showbase << (*it).item_code << "  ";
        cout << setw(6) << hex << uppercase << showbase << (*it).warning_action << setw(1) << "  ";
        cout << setw(6) << hex << uppercase << showbase << (*it).alarm_action << setw(1) << "  ";
        cout << setw(6) << hex << uppercase << showbase << (*it).cancel_action << setw(1) << "  ";
        cout << setw(2) << *dec << (*it).DST_start.hour << ":" << setw(2) << (*it).DST_start.min << "~" << setw(2) << (*it).DST_end.hour << ":" << setw(2) << (*it).DST_end.min << "   ";
        cout << setw(2) << *dec << (*it).ST_start.hour << ":" << setw(2) << (*it).ST_start.min << "~" << setw(2) << (*it).ST_end.hour << ":" << setw(2) << (*it).ST_end.min;
        cout << endl;
    }
    cout << "=============================================================================================================================================" << endl;
}

/*字符串分割函数；将字符串str按delim分割并存放在vector中返回*/
vector<string> JInformation::split(const string &str, const string &delim)
{
    vector<string> res;
    if("" == str)
        return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());                //将string的char形式拷贝到strs
    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p){
        string s = p;                         //分割得到的字符串转换为string类型
        res.push_back(s);                     //存入结果数组
        p = strtok(NULL, d);
    }
    return res;
}

/*字符串分割函数；将字符数组str按delim分割并存放在vector中返回*/
vector<string> JInformation::splitChar(const char *str, int len, const string& delim) 
{
	vector<string> res;
	if("" == str) return res;
	char * strs = new char[len + 1];
	strncpy(strs, str, len);
	char *d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
 
	char *p = strtok(strs, d);
	while(p) {
		string s = p;                       //分割得到的字符串转换为string类型
		res.push_back(s);                   //存入结果数组
		p = strtok(NULL, d);
	}
	return res;
}

unsigned short JInformation::StrToHex(string str)
{
	const string hex = "0123456789ABCDEF";
	for(string::size_type i = 0; i < str.size(); ++i)
		str[i] << hex[(unsigned char)str[i] & 0xf];
    //第三个参数base为合法字符范围，base=2,为0、1，base=16，合法字符则为0-F，开头的0x自动忽略
    unsigned short result = (unsigned short)strtol(str.c_str(), NULL, 16);
    return result;
}

/*=============================================#Global_Value#=============================================*/
/*全局变量初始化*/
void InitGlobalValue()
{
    pthread_cond_init(&pthread_cond_Dev, NULL);
    pthread_mutex_init(&lock_Dev, NULL);
    pthread_cond_init(&pthread_cond_Centre, NULL);
    pthread_mutex_init(&lock_Centre, NULL);
    pthread_cond_init(&pthread_cond_Wincc, NULL);
    pthread_mutex_init(&lock_Wincc, NULL);
    pthread_cond_init(&pthread_cond_Server, NULL);
    pthread_mutex_init(&lock_Server, NULL);

    wait_Dev_status = false;
    wait_Centre_status = false;
    wait_Wincc_status = false;
    wait_Server_status = false;
}