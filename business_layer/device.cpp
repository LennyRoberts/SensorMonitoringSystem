#include "device.hpp"
using namespace std;

UnixSock unix_sock;
JPackage package;

/*============================================================================================================*/
void *InteractDevice(void *arg)
{
	pthread_t tid;
	tid = pthread_self();
	printf("pthread for IPC Interact device: tid= %lld (0x%llX)\n", (long long)tid, (long long)tid);

	pthread_mutex_lock(&lock_Dev);
	wait_Dev_status = true;
	while(wait_Dev_status == true)
		pthread_cond_wait(&pthread_cond_Dev, &lock_Dev);
	wait_Dev_status = false;
	pthread_mutex_unlock(&lock_Dev);
	sleep(1);

	int size = 0;
	char buff[BUFF_SIZE] = {0};
	time_t start_time = 0;
	time_t end_time = 0;
	while(true){
		start_time = time(NULL);
		struct tm *timeval = localtime(&start_time);
		unix_sock.UnixSockClientLink();
		size = unix_sock.SendCmdToDevice();
		while(true){
			memset(buff, 0, BUFF_SIZE);
			size = unix_sock.RecvData(buff, BUFF_SIZE);
			if(size > 0){
				printf("revice data is:\n%s\n", buff);
				unix_sock.DeviceJSONDataParser(buff);
				unix_sock.InterCtlManage(timeval);
				unix_sock.UploadDataToServer();
			}
			else{
				end_time = time(NULL);
				if((end_time-start_time) > 2){
					std::cout << "Business layer get cJSON Data failed!!!" << std::endl;
					break;
				}
			}
		}
		sleep(2);
		unix_sock.CloseUnixSock();
	}
}

/*=========================================#本地Unix_socket_IPC && 联动控制管理 && TCP发送处理类函数#===========================================*/
UnixSock::UnixSock(){this->sock_fd = INVALID_FD;}
UnixSock::~UnixSock(){ }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~设备层IPC部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*Unix域套接字IPC客户端连接程序*/
void UnixSock::UnixSockClientLink()
{
	int len = 0, fd = INVALID_FD;
	struct sockaddr_un servaddr, cliaddr;
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd == -1){
		perror("business ipc socket");
		printf("errno=%d\n",errno);
		errno = 0;
	}
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sun_family = AF_UNIX;
	strcpy(cliaddr.sun_path, IPC_BUSINESS);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(cliaddr.sun_path);	/*计算客户端地址结构有效长度*/
	unlink(IPC_BUSINESS);
	if(bind(fd, (struct sockaddr *)&cliaddr, (socklen_t)len) == -1){	/*客户端也需要bind,不能依赖自动绑定*/
		perror("business ipc bind");
		printf("errno=%d\n",errno);
		errno = 0;
	}

	bzero(&servaddr, sizeof(servaddr));	/*构造server地址*/
	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path, IPC_DEVICE);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(servaddr.sun_path);	/*计算服务器端地址结构有效长度*/
	if(connect(fd, (struct sockaddr *)&servaddr, (socklen_t)len) == -1){
		perror("business ipc connect");
		printf("errno=%d\n",errno);
		errno = 0;
	}
	this->sock_fd = fd;
}

/*断开与设备层的本地套接字连接*/
void UnixSock::CloseUnixSock()
{
	close(this->sock_fd);
}

/*封装指令并向设备层发送指令*/
int UnixSock::SendCmdToDevice()
{
	char cmd_buff[CMD_SIZE];
	memset(cmd_buff, 0, CMD_SIZE);
	package.CmdGenerator(cmd_buff, VERSION, MODULE_DEVICE, DEV_FUNC_DATA, GET_ALL_DATA, GET_ALL_DATA, 1.123);
	int size = SendCmd(cmd_buff, 16);
	return size;
}

/*发送指令到设备层*/
int UnixSock::SendCmd(char *cmd_data, int len)
{
	int size = 0;
	if(cmd_data != NULL && strlen((const char*)cmd_data) > 0){
		size = (int)send(this->sock_fd, cmd_data, len, 0);
	}else
		std::cout << "Send data is empty,can not send to!!!" << std::endl;
	return size;
}

/*接收来自设备层的数据和信息*/
int UnixSock::RecvData(char *buff_data, int len)
{
	int size = 0;
	size = (int)recv(this->sock_fd, buff_data, len, 0);
	return size;
}


/*解析数据并缓存*/
bool UnixSock::DeviceJSONDataParser(const char buff[])
{
	string str_buff = buff;
	neb::CJsonObject *cJsonObj = new neb::CJsonObject(str_buff);
	cJsonObj->Get(DEVICE_NO, package.deviceNo);
	
	package.data_buff.clear();
	int i = 1;
	string node = "s01";
	while(true){
		neb::CJsonObject cJsonNode;
		if((i/10) == 0)
			node.replace(2,1,to_string(i));  
		else if((i/100) == 0)
			node.replace(1,2,to_string(i));
		if(cJsonObj->Get(node, cJsonNode) == false)
			break;
		else{
			DataBuff data_node;
			int code = 0;
			cJsonNode.Get(SENSOR_CODE,code);
			data_node.sensor_code = (unsigned short)code;
			cJsonNode.Get(SENSOR_DATA,data_node.data);
			string str;
			cJsonNode.Get(SENSOR_FLAG,str);
			data_node.status = *str.c_str();
			cJsonNode.Get(SENSOR_UNIT, data_node.unit);
			package.data_buff.push_back(data_node);
			cout << "code:" << setw(6) << hex << uppercase << showbase << data_node.sensor_code 
			<< ";data:" << data_node.data << data_node.unit << ";status:" << data_node.status << endl;
		}
		i++;
	}
	//统计测量值...
	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~设备联动控制管理程序部分~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*智能设备终端联动控制*/
void UnixSock::InterCtlManage(struct tm *timeval)
{
	cout << *dec;
	static vector<JCtrPara> ctr;	//临时的有效控制参数缓存
	static int time_min = 0;
	if(time_min != timeval->tm_min){	//每分钟更新一次
		GetCurrentCtrPara(timeval, ctr);
		time_min = timeval->tm_min;
	}

	static std::vector<JItemInfor> item = jInfo.server_para[jInfo.enable_ser_ith].item;    //找到作为控制参数的服务器中传感器子项参数
	static unsigned short counter = 0;	                                                   //本次检测次数
	static bool frist_overflow = true;                                                     //首次超标位(在测量值首次超标后立即开始检测)
	static time_t start_time = 0;                                                          //报警时间记录
	
	/*每个传感器子项都拥有独立报警控制机制*/
	for(int i=0; i<item.size(); ++i){
		float value = GetSensorData(item[i].item_code);
		time_t current_time = time(NULL);                                                  //当前检测时间
		for(int j=i; j<ctr.size(); ++j){
			if(item[i].item_code == ctr[j].item_code){
				cout << item[i].item << " currentValue=" << value 
					<< " (warningValue=" << ctr[j].warning_value << "; alarmValue=" << ctr[j].alarm_value << ")" << endl;
				if(item[i].flag_alarm == OFF && item[i].flag_sleep == OFF){
					cout << *dec << "Before: total_counter=" << counter << "; counter_warning=" 
						<< item[i].counter_warning << "; counter_alarm=" << item[i].counter_overflow << endl;
					if(counter <= ctr[j].cycle_check){
						// 检测中...
						if(ctr[j].mode == 'H'){                                            //高于阈值时报警
							if(value > ctr[j].alarm_value){
								if(frist_overflow == true){
									frist_overflow = false;
									counter = 0;
									cout << "Frist overflow alarm" << endl;
								}
								item[i].counter_overflow++;
								cout << "Value Overflow Alarm" << endl;
							}
							else if(value> ctr[j].warning_value){
								item[i].counter_warning++;
								cout << "Value over Warning" << endl;
							}else
								cout << "Value Normal" << endl;
						}
						else if(ctr[j].mode == 'L'){                                      //低于阈值时报警
							if(value < ctr[j].alarm_value){
								if(frist_overflow == true){
									frist_overflow = false;
									counter = 0;
									cout << "Frist overflow alarm" << endl;
								}
								item[i].counter_overflow++;
								cout << "Value Overflow Alarm" << endl;
							}
							else if(value < ctr[j].warning_value){
								item[i].counter_warning++;
								cout << "Value over Warning" << endl;
							}else
								cout << "Value Normal" << endl;
						}
						counter++;
						cout << *dec << "After : total_counter=" << counter << "; counter_warning=" << item[i].counter_warning 
							<< "; counter_alarm=" << item[i].counter_overflow << endl;
					}
					if(counter > (ctr[j].cycle_check)){
						// 报警判断...
						float hit_rate_alarm = (float)item[i].counter_overflow / counter;
						cout << "counter_alarm=" << *dec << item[i].counter_overflow << "; counter=" << counter << "; hit_rate_alarm=" << hit_rate_alarm*100 << "%" << endl;
						if(hit_rate_alarm > ctr[j].hit_rate){
							item[i].flag_alarm = ON;                                      //打开报警状态，随即进入报警
							// alarm action
							start_time = time(NULL);                                      //记录报警时刻
							cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!=alarm=!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
						}
						else{
							// 预警判断...
							float hit_rate_warning = (float)item[i].counter_warning / counter;
							cout << "counter_warning=" << item[i].counter_warning << "; counter=" << counter << "; hit_rate_warning=" << hit_rate_warning*100 << "%" << endl;
							if(hit_rate_warning > ctr[j].hit_rate){
								// warning action
								cout << "!.!.!.!.!.!.!.!.!.!.!.!.!.!-warning-!.!.!.!.!.!.!.!.!.!.!.!.!.!" << endl;
							}
						}
						counter = 0;
						frist_overflow = true;
						item[i].counter_warning = 0;
						item[i].counter_overflow = 0;
					}
				}
				else{
					// 报警中...
					int s_left = 0;
					s_left = ctr[j].hold_period - (current_time - start_time);
					if(item[i].flag_alarm == ON){
						cout << "Device in alarm(" << s_left << " seconds left)..." << endl;
						if(s_left <= 0){
							start_time = time(NULL);                                      //记录报警时刻
							item[i].flag_alarm = OFF;
							frist_overflow = true;
							item[i].flag_sleep = ON;                                      //打开休眠状态，随即进入休眠
							cout << "Device Alarm over!" << endl;
						}
						else{
							// alarm state upload
						}
					}
					// 休眠中...
					s_left = ctr[j].dormant_period - (current_time - start_time);
					if(item[i].flag_sleep == ON && item[i].flag_alarm == OFF){
						cout << "Device in dormancy(" << s_left << " seconds left)..." << endl;
						if(s_left <= 0){
							item[i].flag_sleep = OFF;
							cout << "Device Dormancy over!" << endl;
						}
						else{
							// dormancy state upload
						}
					}
				}
			}
		}
	}
}

/*获取传感器测量值*/
float UnixSock::GetSensorData(unsigned short code)
{
	std::vector<DataBuff>::iterator it;
	for(it=package.data_buff.begin(); it!=package.data_buff.end(); it++){
		if((*it).sensor_code == code)
			return (*it).data;
	}
	return 0;
}

/*将当前符合要求的控制参数项取出来并储存以供后面的判断使用*/
void UnixSock::GetCurrentCtrPara(struct tm *timeval, std::vector<JCtrPara> &ctr)
{
	printf("Current time: %d/%d %02d:%02d\n", timeval->tm_mon, timeval->tm_mday, timeval->tm_hour, timeval->tm_min);
	ctr.clear();
	std::vector<JCtrPara>::iterator cit;
	for(cit = jInfo.ctr_para.begin(); cit != jInfo.ctr_para.end(); cit++){
		bool flag_dst = false;
		bool flag_st  = false;
		if((*cit).enable == ON){
			if(timeval->tm_mon >= 5 && timeval->tm_mon < 10){	// DTS:夏令时[5.1~9.30]
				if((timeval->tm_hour > (*cit).DST_start.hour) || (timeval->tm_hour==(*cit).DST_start.hour && timeval->tm_min >= (*cit).DST_start.min)){ //先判断起始时间
					if((*cit).DST_start.hour > (*cit).DST_end.hour){
						if((timeval->tm_hour < ((*cit).DST_end.hour + 24)) || (timeval->tm_hour==((*cit).DST_end.hour + 24) && timeval->tm_min < (*cit).DST_end.min)) //再判断结束时间
							flag_dst = true;
					}
					else{
						if((timeval->tm_hour < (*cit).DST_end.hour) || (timeval->tm_hour==(*cit).DST_end.hour && timeval->tm_min < (*cit).DST_end.min))
							flag_dst = true;
					}
				}
			}
			else if(timeval->tm_mon < 5 || timeval->tm_mon >= 10){	// StandardTime:标准时(冬令时[10.1~nextyear4.30])
				if((timeval->tm_hour > (*cit).ST_start.hour) || (timeval->tm_hour==(*cit).ST_start.hour && timeval->tm_min >= (*cit).ST_start.min)){ //先判断起始时间
					if((*cit).ST_start.hour > (*cit).ST_end.hour){
						if((timeval->tm_hour < ((*cit).ST_end.hour + 24)) || (timeval->tm_hour==((*cit).ST_end.hour + 24) && timeval->tm_min < (*cit).ST_end.min)) //再判断结束时间
							flag_st = true;
					}
					else{
						if((timeval->tm_hour < (*cit).ST_end.hour) || (timeval->tm_hour==(*cit).ST_end.hour && timeval->tm_min < (*cit).ST_end.min))
							flag_st = true;
					}
				}
			}
			if(flag_dst == true){
				(*cit).ST_start.hour = 0;
				(*cit).ST_start.min = 0;
				(*cit).ST_end.hour = 0;
				(*cit).ST_end.min = 0;
				ctr.push_back(*cit);
			}
			else if(flag_st == true){
				(*cit).DST_start.hour = 0;
				(*cit).DST_start.min = 0;
				(*cit).DST_end.hour = 0;
				(*cit).DST_end.min = 0;
				ctr.push_back(*cit);
			}
		}
	}
	jInfo.printCtrParaInfo(ctr);    //打印当前控制信息
} 


/*数据封装并上传至服务器函数*/
void UnixSock::UploadDataToServer()
{
	time_t current_time = time(NULL);
	struct tm *timeval = localtime(&current_time);
	int size = 0;
	
	vector<JServerInfo>::iterator sit;
	for(sit=jInfo.server_para.begin(); sit!=jInfo.server_para.end(); sit++){
		package.TimeCtrStatistics(timeval, &(*sit));
		

		cout << endl;
	}
}

/*=================================================#数据处理与封装处理类函数#=================================================*/
JPackage::JPackage(){}
JPackage::~JPackage(){}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~数据统计控制机制~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*数据统计时间控制机制*/
void JPackage::TimeCtrStatistics(struct tm *timeval, JServerInfo *server)
{
	static int minute_one_old = -1;
	static int minute_two_old = -1;
	static int hour_one_old = -1;
	static int hour_two_old = -1;
	static int day_one_old = -1;
	static int day_two_old = -1;

	static bool clear_flag_minute_one = false;
	static bool clear_flag_minute_two = false;
	static bool clear_flag_hour_one   = false;
	static bool clear_flag_hour_two   = false;
	static bool clear_flag_day_one    = false;
	static bool clear_flag_day_two    = false;

	int size = 0;
	char buff[4096];
	memset(buff, 0, 4096);

	// 第1个分钟值
	if(server->time_minute_one != 0){
		StatisticsMeasureValue(server, 1, MINUTE_ONE, clear_flag_minute_one);
		if(timeval->tm_min != minute_one_old && timeval->tm_min % (int)server->time_minute_one == 0){
			StatisticsMeasureValue(server, 2, MINUTE_ONE, clear_flag_minute_one);
			size = package.GetDataPackets(buff, server->packet_code, 2011, &(server->item), &(server->package_info));
			printf("%s time minute one data packet(len=%d):\n%s", server->name.c_str(), size, buff);
			size = tcp_client.TcpSend(*server, buff, size);
			if(size == -1){
				cout << (*server).name << " send fail,Try again..." << endl;
				size = tcp_client.TcpSend(*server, buff, size);
				if(size == -1)
					cout << (*server).name << " send over time, Abandon this connection sending!!!" << endl;
			}
			minute_one_old = timeval->tm_min;
		}
	}
	// 第2个分钟值
	if(server->time_minute_two != 0){
		StatisticsMeasureValue(server, 1, MINUTE_TWO, clear_flag_minute_two);
		if(timeval->tm_min != minute_two_old && timeval->tm_min % (int)server->time_minute_two == 0){
			StatisticsMeasureValue(server, 2, MINUTE_TWO, clear_flag_minute_two);
			minute_two_old = timeval->tm_min;
		}
	}
	// 第1个小时值
	if(server->time_hour_one != 0){
		StatisticsMeasureValue(server, 1, HOUR_ONE, clear_flag_hour_one);
		if(timeval->tm_hour != hour_one_old && timeval->tm_hour % (int)server->time_hour_one == 0){
			StatisticsMeasureValue(server, 2, HOUR_ONE, clear_flag_hour_one);
			hour_one_old = timeval->tm_hour;
		}
	}
	// 第2个小时值
	if(server->time_hour_two != 0){
		StatisticsMeasureValue(server, 1, HOUR_TWO, clear_flag_hour_two);
		if(timeval->tm_hour != hour_two_old && timeval->tm_hour % (int)server->time_hour_two == 0){
			StatisticsMeasureValue(server, 2, HOUR_ONE, clear_flag_hour_two);
			hour_two_old = timeval->tm_hour;
		}
	}
	// 第1个日数据值
	if(server->time_day_one != 0){
		StatisticsMeasureValue(server, 1, DAY_ONE, clear_flag_day_one);
		if(timeval->tm_mday != day_one_old && timeval->tm_mday % (int)server->time_day_one == 0){
			StatisticsMeasureValue(server, 2, DAY_ONE, clear_flag_day_one);
			day_one_old = timeval->tm_mday;
		}
	}
	// 第2个日数据值
	if(server->time_day_two != 0){
		StatisticsMeasureValue(server, 1, DAY_TWO, clear_flag_day_two);
		if(timeval->tm_mday != day_two_old && timeval->tm_mday % (int)server->time_day_two == 0){
			StatisticsMeasureValue(server, 2, HOUR_ONE, clear_flag_day_two);
			day_two_old = timeval->tm_mday;
		}
	}
}

/*统计测量值*/
void JPackage::StatisticsMeasureValue(JServerInfo *server, int step, int opt, bool &clear_flag)
{
	std::vector<JItemInfor>::iterator it;
	for(it=server->item.begin(); it!=server->item.end(); it++){
		if((*it).item_en == ON){
			if(step == 1){
				float value = unix_sock.GetSensorData((*it).item_code);
				CalculateStatisticsData(&(*it), step, value, opt, clear_flag);
			}
			else if(step == 2)
				CalculateStatisticsData(&(*it), step, 0, opt, clear_flag);
		}
	}
}

/*选择数据统计计算方式*/
void JPackage::CalculateStatisticsData(JItemInfor *item, int step, float value, int opt, bool &clear_flag)
{
	switch(item->mode_statistics){
		case ARITHMETIC_MEAN:
			ArithmeticMean(item, step, value, opt, clear_flag);
			break;
		case MOVING_AVERAGE:
			break;
		case CUMULATIVE_VALUE:
			break;
		default:
			break;
	}
}

/*计算算术平均数*/
void JPackage::ArithmeticMean(JItemInfor *item, int step, float value, int opt, bool &clear_flag)
{
	if(step == 1){
		switch(opt){
			case MINUTE_ONE:
				if(clear_flag == true){
					item->minute_one.sum = 0;
					item->minute_one.counter = 0;
					item->minute_one.minimum = 99999;
					item->minute_one.maximum = 0-99999;
					clear_flag = false;
				}
				item->minute_one.sum += (double)value;
				item->minute_one.counter ++;
				if(item->minute_one.minimum > value)
					item->minute_one.minimum = value;
				if(item->minute_one.maximum < value)
					item->minute_one.maximum = value;
				break;
			case MINUTE_TWO:
				if(clear_flag == true){
					item->minute_two.sum = 0;
					item->minute_two.counter = 0;
					item->minute_two.minimum = 99999;
					item->minute_two.maximum = 0-99999;
					clear_flag = false;
				}
				item->minute_two.sum += (double)value;
				item->minute_two.counter ++;
				if(item->minute_two.minimum > value)
					item->minute_two.minimum = value;
				if(item->minute_two.maximum < value)
					item->minute_two.maximum = value;
				break;
			case HOUR_ONE:
				if(clear_flag == true){
					item->hour_one.sum = 0;
					item->hour_one.counter = 0;
					item->hour_one.minimum = 99999;
					item->hour_one.maximum = 0-99999;
					clear_flag = false;
				}
				item->hour_one.sum += (double)value;
				item->hour_one.counter ++;
				if(item->hour_one.minimum > value)
					item->hour_one.minimum = value;
				if(item->hour_one.maximum < value)
					item->hour_one.maximum = value;
				break;
			case HOUR_TWO:
				if(clear_flag == true){
					item->hour_two.sum = 0;
					item->hour_two.counter = 0;
					item->hour_two.minimum = 99999;
					item->hour_two.maximum = 0-99999;
					clear_flag = false;
				}
				item->hour_two.sum += (double)value;
				item->hour_two.counter ++;
				if(item->hour_two.minimum > value)
					item->hour_two.minimum = value;
				if(item->hour_two.maximum < value)
					item->hour_two.maximum = value;
				break;
			case DAY_ONE:
				if(clear_flag == true){
					item->day_one.sum = 0;
					item->day_one.counter = 0;
					item->day_one.minimum = 99999;
					item->day_one.maximum = 0-99999;
					clear_flag = false;
				}
				item->day_one.sum += (double)value;
				item->day_one.counter ++;
				if(item->day_one.minimum > value)
					item->day_one.minimum = value;
				if(item->day_one.maximum < value)
					item->day_one.maximum = value;
				break;
			case DAY_TWO:
				if(clear_flag == true){
					item->day_two.sum = 0;
					item->day_two.counter = 0;
					item->day_two.minimum = 99999;
					item->day_two.maximum = 0-99999;
					clear_flag = false;
				}
				item->day_two.sum += (double)value;
				item->day_two.counter ++;
				if(item->day_two.minimum > value)
					item->day_two.minimum = value;
				if(item->day_two.maximum < value)
					item->day_two.maximum = value;
				break;
			default:
				break;
		}
	}
	else if(step == 2){
		switch(opt){
			case MINUTE_ONE:
				item->minute_one.sum = (double)item->minute_one.sum /item->minute_one.counter;
				clear_flag = true; break;
			case MINUTE_TWO:
				item->minute_two.sum = (double)item->minute_two.sum / item->minute_two.counter;
				clear_flag = true; break;
			case HOUR_ONE:
				item->hour_one.sum = (double)item->hour_one.sum /item->hour_one.counter;
				clear_flag = true; break;
			case HOUR_TWO:
				item->hour_two.sum = (double)item->hour_two.sum / item->hour_two.counter;
				clear_flag = true; break;
			case DAY_ONE:
				item->day_one.sum = (double)item->day_one.sum / item->day_one.counter;
				clear_flag = true; break;
			case DAY_TWO:
				item->day_two.sum = (double)item->day_two.sum / item->day_two.counter;
				clear_flag = true; break;
			default:
				break;
		}
	}
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~数据封装处理~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*数据封装函数*/
int JPackage::GetDataPackets(char *buff, unsigned short format, int CN, vector<JItemInfor> *item, vector<string> *package_info)
{
	int size = 0;
	switch (format){
		case SURFACE_WATER_QUALITY:

			break;
		case AIR_QUALITY:
			size = package.hj212.GetDataPacketHJ212(buff, item, CN, package_info);

			break;
		
		default:
			break;
	}
	return size;
}

/*获取设备号*/
string JPackage::GetDeviceNo()
{
	return this->deviceNo;
}

/*获取时间*/
string JPackage::GetMeasureTime()
{
	return " ";
}

/*==========================================#国标212协议数据包处理类函数#==========================================*/
UnifyHJ212Protocol::UnifyHJ212Protocol(){}
UnifyHJ212Protocol::~UnifyHJ212Protocol(){}

/*212数据包封装*/
int UnifyHJ212Protocol::GetDataPacketHJ212(char *buff, vector<JItemInfor> *item, int CN, vector<string> *package_info)
{
	int i = 0;
	int len = 0;
	len += sprintf(&buff[len],"##0000");
	struct tm *timeval;
	time_t packet_time;
	time(&packet_time);
	timeval = localtime(&packet_time);
	len += sprintf(&buff[len], "QN=%04d%02d%02d%02d%02d%02d000;",
				   1900+timeval->tm_year, 1+timeval->tm_mon, timeval->tm_mday, timeval->tm_hour, timeval->tm_min, timeval->tm_sec);
	if(!package_info[i].empty()){
		len += sprintf(&buff[len],"%s;", (*package_info)[i].c_str());
		i++;
	}else
		len += sprintf(&buff[len], "ST=22;");
	len += sprintf(&buff[len], "CN=%-4d;", CN);
	if(!package_info[i].empty()){
		len += sprintf(&buff[len], "%s;", (*package_info)[i].c_str());
		i++;
	}else
		len += sprintf(&buff[len], "PW=123456;");
	char tmp[32];
	memset(tmp,0,32);
	string strtmp = package.GetDeviceNo();
	strncpy(tmp, strtmp.c_str(),strtmp.length());
	len += sprintf(&buff[len], "MN=%s;", tmp);
	if(!package_info[i].empty()){
		len += sprintf(&buff[len], "%s;", (*package_info)[i].c_str());
		i++;
	}else
		len += sprintf(&buff[len], "Flag=5;");
	len += sprintf(&buff[len], "CP=&&DataTime=%04d%02d%02d%02d%02d%02d;",
				   1900+timeval->tm_year, 1+timeval->tm_mon, timeval->tm_mday, timeval->tm_hour, timeval->tm_min, timeval->tm_sec);

	if(CN == 2011){  //Real data
		std::vector<JItemInfor>::iterator it;
		for(it=item->begin(); it!=item->end(); it++){
			char tabel[16];
			char flag = 'B';
			int precision = 0;
			float data = 0;
			if((*it).item_en == ON){
				for(int i = 0; i < package.data_buff.size(); ++i){
					if((*it).item_code ==package.data_buff[i].sensor_code){
						data = package.data_buff[i].data;
						flag = package.data_buff[i].status;
					}
				}
				memset(tabel,0,16);
				strncpy(tabel, (*it).table.c_str(), (*it).table.length());
				precision = (int)(*it).precision;
				len += sprintf(&buff[len],"%s-Rtd=%0.*f,%s-Flag=%c;",tabel,precision,data,tabel,flag);
			}
		}
	}
	else{
		switch(CN){
		default:
			break;
		}
	}

	char t6 = buff[6]; 
	len = len -1;
	memcpy(&buff[len], "&&", 2); len += 2;
	sprintf(&buff[2], "%04d", len - 6); /*length exclude head "##0000"*/
	buff[6] = t6;
	unsigned short crc = HJ212CRCGenerator((unsigned char*)&buff[6], len-6); /*crc exclude head "##0000"*/
	len += sprintf(&buff[len], "%04X\r\n", crc);
	return len;
}

/*HJ212协议CRC校验发生器*/
unsigned short UnifyHJ212Protocol::HJ212CRCGenerator(unsigned char* puchMsg, unsigned int usDataLen)
{
	unsigned int i,j,check;
	unsigned short crc_reg = 0xFFFF;
	for(i=0;i<usDataLen;i++){
		crc_reg = (crc_reg>>8) ^ puchMsg[i];
		for(j=0;j<8;j++){
			check = crc_reg & 0x0001;
			crc_reg >>= 1;
			if(check==0x0001)
				crc_reg ^= 0xA001;
		}
	}
	return crc_reg;
}

/*获取命令编码*/
string UnifyHJ212Protocol::GetCN(const unsigned char value_model)
{

}

/*==========================================#JSON格式协议数据包处理类函数#==========================================*/
JsonProtocol::JsonProtocol(){ }
JsonProtocol::~JsonProtocol(){ }


/*==========================================#MQTT格式协议数据包处理类函数#==========================================*/
MQTTProtocol::MQTTProtocol(){}
MQTTProtocol::~MQTTProtocol(){}
