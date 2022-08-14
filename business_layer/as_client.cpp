#include "as_client.hpp"

using namespace std;

TCPClient tcp_client; //本机客户端对象

/*============================================================================================*/
void *InteractCentre(void *arg)
{
	pthread_t tid;
	tid = pthread_self( );
	printf("pthread for Interact Centre: tid= %lld (0x%llX)\n", (long long)tid, (long long)tid);

    tcp_client.TCPSockLink();
	pthread_mutex_lock(&lock_Centre);
	wait_Centre_status = true;
	while(wait_Centre_status == true)
		pthread_cond_wait(&pthread_cond_Centre, &lock_Centre);
	wait_Centre_status = false;
	pthread_mutex_unlock(&lock_Centre);
	sleep(1);

    char buff[1024];
    while(true){
        vector<JServerInfo>::iterator it;
	    for(it=jInfo.server_para.begin(); it!=jInfo.server_para.end(); it++){
            memset(buff, 0, 1024);
            int len = tcp_client.TcpRevice((*it).fd_sock, buff);
            if(len > 0){
                cout << "Recive " << (*it).name << " server data: " << hex << uppercase << showbase << buff << endl;
            }
        }
    }
}

/*--------------------------------------------------------------------------------------------*/
TCPClient::TCPClient()
{
}

TCPClient::~TCPClient(){ }

/*建立TCP通信*/
void TCPClient::TCPSockLink()
{
    cout << "~~~~~~~~~~~~~~~~~TCP connection status~~~~~~~~~~~~~~~~~~" << endl;
    vector<JServerInfo>::iterator it;
    for(it=jInfo.server_para.begin(); it!=jInfo.server_para.end(); it++){
        cout << (*it).name << ": ";
        int fd = TCPSockConnectSingle( (*it).net_IP, (*it).net_port);
        if(fd == INVALID_FD)
            cout << (*it).name << ": Server link fali!!!" << endl;
        else
            cout << (*it).name << ": tcp client sucess; fd=" << fd << endl;
        cout << endl;
        (*it).fd_sock = fd;
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

/*单个TCP建立执行函数*/
int TCPClient::TCPSockConnectSingle(const char *ip, const int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == INVALID_FD){
        perror("Tcp socket");
        printf("errno=%d\n", errno);
        errno = 0;
        return INVALID_FD;
    }
    struct sockaddr_in sock_addr;
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    cout << "IP:"<< ip << "; "; 
    inet_pton(AF_INET, ip, &sock_addr.sin_addr);
    cout << "port:" << *dec << port << endl;
    sock_addr.sin_port = htons(port);
    // int flags = fcntl(fd, F_GETFL, 0);      //获取文件描述符
	// fcntl(fd, F_SETFL, flags | O_NONBLOCK); //设置文件为非阻塞。
    int state = connect(fd, (struct sockaddr *)&sock_addr, sizeof(sockaddr));
    if(state == -1){
        perror("Tcp connect");
        printf("errno=%d; ", errno);
        errno = 0;
        return INVALID_FD;
    }
    return fd;
}

/*数据上传至服务器*/
int TCPClient::TcpSend(JServerInfo &server, const char *str, int len)
{
    int len_send = 0;
    if(server.fd_sock != INVALID_FD && CheckFdStatus(server.fd_sock, 2, 200) != false){
        len_send = send(server.fd_sock, str, len, 0);
        if(len_send == -1){
            cout << server.name << ": TCP send fail,retry link and send data!!!" << endl;
            tcp_client.CloseTcpLink(server);
            tcp_client.TCPSockConnectSingle(server.net_IP,server.net_port);
            usleep(50000);
            return -1;
        }
        else if(len_send > 0){
            printf("Send success %d byte data to %s\n", len_send, server.name);
            return len_send;
        }
    }
    else{
        printf("The connection to the %s is not established or unavailable\n", server.name);
        return -1;
    }
    return len_send;
}

/*接收指令信息*/
int TCPClient::TcpRevice(int fd_sock, char *buff)
{
    int len_send = 0;
    if(fd_sock != INVALID_FD && CheckFdStatus(fd_sock, 2, 200) != false){
        len_send = recv(fd_sock, buff, sizeof(buff), 0);
    }
    return len_send;
}

/*关闭TCP连接*/
void TCPClient::CloseTcpLink(JServerInfo &ser)
{
    if(ser.fd_sock != INVALID_FD){
        close(ser.fd_sock);
        ser.fd_sock = INVALID_FD;
    }
}