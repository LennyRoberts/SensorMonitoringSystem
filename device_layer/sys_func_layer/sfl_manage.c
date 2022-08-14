#include "sfl_manage.h"

/*交换双缓存*/
bool ExchangeBuffer(dual_buff *buff_pull_now)
{
	int push_flag = 1;
	int pull_flag = buff_pull_now->flag;
	if(pull_flag == 1)
		push_flag = 2;
	else if(pull_flag == 2)
		push_flag = 1;
	printf("\n====================#!From SFL_pull_buffer_%d to SPL_push_buffer_%d!#=======================\n",pull_flag,push_flag);
	bool ex_status = TRUE;
	dual_buff* buff_one = BuffPtrOne();
	dual_buff* buff_two = BuffPtrTwo();
	if(pull_flag == 1){
		ChangeBuffPull(buff_two);
		ChangeBuffPush(buff_one);
	}
	else if(pull_flag == 2){
		ChangeBuffPull(buff_one);
		ChangeBuffPush(buff_two);
	}

	/*交换是否成功*/
	if(pull_flag == 1){
		if(BuffPull() != buff_two){
			printStr("Buffer exchange failure!!!");
			ex_status = FALSE;
		}
	}
	else if(pull_flag == 2){
		if(BuffPull() != buff_one){
			printStr("Buffer exchange failure!!	!");
			ex_status = FALSE;
		}
	}
	return ex_status;
}

/*数据发送到指定串口*/
int UplaodModbusDataToPort()
{
	char buff[MAX_MODBUS_BUFFER_SIZE];
	memset(buff,0,MAX_MODBUS_BUFFER_SIZE);
	int len = GetModbusData(buff);
	ispc *ptr_ispc = DeviceParaPtr()->ispc_head;
	while(ptr_ispc != NULL){
		if(ptr_ispc->enable == ON){
			FlushFdOut(ptr_ispc->fd_port);
			len = write(ptr_ispc->fd_port, buff, len);
			if(len <= 0){
				serial_port **port = SerialPort();
				printf("IPSC %s send to %s failed!!!\n", ptr_ispc->name, port[ptr_ispc->port_ith]->portName);
			}else
				printHexStr("Write data:", buff, len);
		}
		ptr_ispc = ptr_ispc->next;
	}
	return len;
}


#if 0
/*数据包或数据信息上传至服务层*/
int UploadToSevice(int fd, char buf[])
{
	int send_len = send(fd, buf, strlen(buf), 0);
	return send_len;
}

/*接收服务层下发的指令或数据*/
char *DownloadFromService(int fd, char *buf, int recv_len)
{
	char str[1024];
	printf("\n\nrecv data from service...\n");
	int recv_num = recv(fd, str, 1024, 0);
	printStr(str);
	perror("err=");
	return buf;
}

/*Unix本地套接字IPC初始化(作为客户端)*/
int UnixSockeIPCInit( )
{
	int fd_sock_unix = -1;
	int len = 0;
	int status = FALSE;
	struct sockaddr_un serviceAddr, deviceAddr;
	fd_sock_unix = socket(AF_UNIX, SOCK_STREAM, 0); //创建一个套接字，返回一个描述符
	if(fd_sock_unix == INVALID_FD){
		perror("Unix socket:socket error");
		return FALSE;
	}
	bzero(&deviceAddr, sizeof(deviceAddr));		 //清空套接字信息
	deviceAddr.sun_family = AF_UNIX;			 //设置客户端套接字属性信息
	strcpy(deviceAddr.sun_path, DEVICE_ADDR);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(deviceAddr.sun_path);
	unlink(DEVICE_ADDR);							  //清除之前的.socket文件
	status = bind(fd_sock_unix, (struct sockaddr *)&deviceAddr, len); //将描述符与端口(.socket文件)进行绑定
	if(status == FALSE){
		perror("Unix socket:bind error");
		return status;
	}

	bzero(&serviceAddr, sizeof(serviceAddr)); 	//清除之前的.socket文件
	serviceAddr.sun_family = AF_UNIX;		  	//设置服务端套接字属性信息
	strcpy(serviceAddr.sun_path, SERVICE_ADDR);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(serviceAddr.sun_path);
	status = connect(fd_sock_unix, (struct sockaddr *)&serviceAddr, len); //客户端(描述符)与服务端建立连接
	if(status == FALSE){
		perror("Unix socket: connect");
		return status;
	}
	return fd_sock_unix;
}
#endif

/*Unix本地套接字IPC初始化(设备层作为服务端)*/
int UnixSockServerInit()
{
	int lfd = -1;
	int len = 0;
	struct sockaddr_un serveraddr;
	lfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(lfd == INVALID_FD){
		perror("device unix socket");
	}
	bzero(&serveraddr, sizeof(serveraddr));	//套接字信息初始化
	serveraddr.sun_family = AF_UNIX;		//设置套接字属性信息
	strcpy(serveraddr.sun_path, IPC_DEVICE);
	len = offsetof(struct sockaddr_un, sun_path) + strlen(serveraddr.sun_path);
	unlink(IPC_DEVICE);                                                         //清除之前的.socket文件
	if((bind(lfd, (struct sockaddr *)&serveraddr, (socklen_t)len) == FALSE)){	//将描述符与端口(.socket文件)进行绑定
		perror("device ipc bind");
	}
	if(listen(lfd, 5) == FALSE){	//监听
		perror("device ipc listen");
	}
	printStr("IPC device listening...");
	return lfd;
}

int UnixSockAccept(int lfd)
{
	int cfd = -1;
	int len = 0;
	struct sockaddr_un cliaddr;
	len = sizeof(cliaddr);
	strcpy(cliaddr.sun_path, IPC_BUSINESS);
	cfd = accept(lfd, (struct sockaddr *)&cliaddr, (socklen_t *)&len);	//接受客户端连接请求
	if(cfd == INVALID_FD){
		perror("accept error");
		return FALSE;
	}
	else{
		len -= offsetof(struct sockaddr_un, sun_path);
		cliaddr.sun_path[len] = '\0';
		printf("Accept client filename: %s\n", cliaddr.sun_path);
	}
	return cfd;
}

/*==========================================================#!IPC one!#==================================================================*/
/*进程间通信IPC_One*/
void* IPCOne(void *arg)
{
	pthread_t tid;
	tid = pthread_self( );
	printf("pthread for IPC One: tid= %lld (0x%llX)\n", (unit_64)tid, (unit_64)tid);

	ExecuteInit();
	CMDManageInit();
	int fd_dev = UnixSockServerInit();
	signal(SIGPIPE,SIG_IGN); /*忽略SIGPIPE信号，避免当客户端程序意外崩溃后导致服务端程序崩溃*/

	pthread_mutex_lock(&lock_IPC1);
	wait_IPC1_status = TRUE;
	while(IsWait(wait_IPC1_status))
		pthread_cond_wait(&pthread_cond_IPC1, &lock_IPC1);
	wait_IPC1_status = FALSE;
	pthread_mutex_unlock(&lock_IPC1);
	sleep(1);

	unsigned char cmd_buff[CMD_SIZE * MAX_CMD_NUMBER];
	unit_16 cmd_status = FALSE;
	time_t start_time = 0;
	time_t end_time = 0;
	while(1){
		int fd = INVALID_FD;
		fd = UnixSockAccept(fd_dev);
		/*accept仅当有一个客户端连接时才返回，否则一直阻塞*/

		memset(cmd_buff, 0, CMD_SIZE * MAX_CMD_NUMBER);
		while(1){
			start_time = clock();
			if(RecvCmd(fd, cmd_buff, sizeof(cmd_buff)) > 0){
				cmd_status = CmdManage(cmd_buff, fd, BUSINESS); /*指令处理*/
				if(cmd_status == CMD_RETRANS)					/*未完成本次全部的指令传输则继续，否则退出*/
					continue;
				else
					break;
			}	
			else{
				end_time = clock();
				if(((end_time-start_time)/CLOCKS_PER_SEC) > 2){
					break;
				}
			}
		}
		if(DeleteCmdCache() == FALSE){	/*若指令缓存释放失败，则再试一次*/
			printf("Cmd cache release failed. Try again!!!\n");
			DeleteCmdCache();
		}
		close(fd);
	}
	close(fd_dev);
}


/*串口间通信*/
void* ISPCManage(void *arg)
{
	pthread_t tid;
	tid = pthread_self( );
	printf("pthread for inter-serial port Communication: tid= %lld (0x%llX)\n", (unit_64)tid, (unit_64)tid);
	printStr("Enter ISPC Manage Programme");

	pthread_mutex_lock(&lock_ISPC);
	wait_ISPC_status = TRUE;
	while(IsWait(wait_ISPC_status))
		pthread_cond_wait(&pthread_cond_ISPC, &lock_ISPC);
	wait_ISPC_status = FALSE;
	pthread_mutex_unlock(&lock_ISPC);
	sleep(1);

	while(1){
		ispc *ptr_ispc = DeviceParaPtr()->ispc_head;
		while(ptr_ispc != NULL){
			int len = 0;
			unit_8 buff[CMD_SIZE];
			memset(buff, 0, CMD_SIZE);
			if(ptr_ispc->enable == ON){
				len = read(ptr_ispc->fd_port, buff, CMD_SIZE);
				if(len > 0){
					printHexStr("revice data:", buff, len);
					CmdManage(buff, ptr_ispc->fd_port, ISPC);
				}
			}
			ptr_ispc = ptr_ispc->next;
		}
	}
}


/*数据解析优化算法管理程序*/
void *AlgorithmManage(void *arg)
{
	pthread_t tid;
	tid = pthread_self( );
	printf("pthread for Algorithm: tid= %lld (0x%llX)\n", (unit_64)tid, (unit_64)tid);

	printStr("Enter Optimization Algorithm Manage Programme");
	InitOptimize();
	wait_AL_status = TRUE;
	while(IsWait(wait_AL_status))
		pthread_cond_wait(&pthread_cond_AL, &lock_AL);
	wait_AL_status = FALSE;
	pthread_mutex_unlock(&lock_AL);
	sleep(1);
	
	struct timeval now;
	struct timespec out_time;
	sensor_type *ptr_type = NULL;
	TypeHead(&ptr_type);
	dual_buff *buffer_pull = BuffPull();
	while(1){
		if(IsEmptyType(ptr_type)){
			JsonDataPackage(buffer_pull->m_time);
			ModbusPacket();
			// UplaodModbusDataToPort();
			pthread_mutex_trylock(&lock_AL);
			gettimeofday(&now, NULL);                                       /*获取当前时间*/
			out_time.tv_sec = now.tv_sec + ExchangeOverTime();
			out_time.tv_nsec = now.tv_usec;
			TypeHead(&ptr_type);
			wait_AL_status = TRUE;
			pthread_cond_timedwait(&pthread_cond_AL, &lock_AL, &out_time);
			wait_AL_status = FALSE;
			ExchangeBuffer(BuffPull());
			buffer_pull = BuffPull();
			pthread_mutex_unlock(&lock_AL);
		}
		DataItemHandling(ItemHead(ptr_type), buffer_pull->buff);
		TypeNext(&ptr_type);
	}
}
