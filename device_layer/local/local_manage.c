#include "local_manage.h"

// int UnixSockClientLink()
// {
// 	int len = 0;
// 	int sock_fd = -1;
// 	struct sockaddr_un servaddr, cliaddr;
// 	sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
// 	bzero(&cliaddr, sizeof(cliaddr));
// 	cliaddr.sun_family = AF_UNIX;
// 	strcpy(cliaddr.sun_path, DEVICE_ADDR);

// 	len = offsetof(struct sockaddr_un, sun_path) + strlen(cliaddr.sun_path);  /* 计算客户端地址结构有效长度 */
// 	unlink(DEVICE_ADDR);
// 	bind(sock_fd, (struct sockaddr *)&cliaddr, len);                          /* 客户端也需要bind, 不能依赖自动绑定*/

// 	bzero(&servaddr, sizeof(servaddr));                                       /* 构造server 地址 */
// 	servaddr.sun_family = AF_UNIX;
// 	strcpy(servaddr.sun_path, SERVICE_ADDR);
// 	len = offsetof(struct sockaddr_un, sun_path) + strlen(servaddr.sun_path); /* 计算服务器端地址结构有效长度 */
// 	connect(sock_fd, (struct sockaddr *)&servaddr, len);
// 	return sock_fd;
// }

// int RecvFrom(int sock_fd, unsigned char *buf, int len)
// {
// 	int size = 0;
// 	size = (int)recv(sock_fd, buf, len, 0);
// 	return size;
// }

// int SendTO(int sock_fd, unsigned char *buf, int len)
// {
// 	int size = 0;
// 	if(buf != NULL && strlen(buf) > 0)
// 		size = (int)send(sock_fd, buf, len, 0);
// 	else
// 		printf("Local service data is empty,can not send to!!!");
// 	return size;
// }

// void LocalServiceManage()
// {
// 	unsigned char buf_cmd[256];
// 	unsigned char buf_data[2048];
// 	while(1){
// 		int fd_sock = UnixSockClientLink();
// 		sleep(1);
// 		unsigned char cmd_str[] = {0x07, 0x12, 0x34, 0x01, 0x01, 0x06, 0x00, 0x00, 0x01, 0x06, 0x01, 0x01, 0x01, 0xAA, 0x03, 0x7B};
// 		SendTO(fd_sock, cmd_str, 16);
// 		memset(buf_data, 0, 2048);
// 		if(RecvFrom(fd_sock, buf_data, sizeof(buf_data)) > 0){
// 			printf("Local service recive data is:\n%s\n\n", buf_data);
// 		}
// 		else{
// 			printf("Local service recive data is null!!!");
// 		}
// 		sleep(2);
// 		close(fd_sock);
// 	}
// }



/*处理来自服务层的本地服务模块指令*/
bool LocalModuleModify(unit_8 *cmd)
{
    return TRUE;
}



/*获取传感器测量数据*/
void GetSensorData()
{
    sensor_type *ptr_type = TypeLinkHead();
    while(ptr_type != NULL){
        sensor_item *ptr_item = ptr_type->item_head;
        while(ptr_item != NULL){
            float result = GetResult(ptr_item->buff_ith);
            printf("Local Module get data is:%s=%f\n", ptr_item->item_name, result);
            ItemNext(&ptr_item);
        }
        TypeNext(&ptr_type);
    }
}

/*本地服务模块处理线程*/
void *LocalManage(void *arg)
{
    pthread_t tid;
	tid = pthread_self( );
	printf("pthread for Local Moduel: tid= %lld (0x%llX)\n", (unit_64)tid, (unit_64)tid);
    
	printStr("Enter Local Module Manage Programme");
	wait_LM_status = TRUE;
	while(IsWait(wait_LM_status))
		pthread_cond_wait(&pthread_cond_LM, &lock_LM);
	wait_LM_status = FALSE;
	pthread_mutex_unlock(&lock_LM);
	sleep(1);

    while(1){
        GetSensorData();
        sleep(6);
    }
}