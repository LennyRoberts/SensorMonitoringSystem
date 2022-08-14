#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "info.hpp"
#include "device.hpp"
#include "as_server.hpp"
#include "as_client.hpp"
#include "wincc.hpp"
#include "self_test.hpp"

using namespace std;

void LoggingRecord();
bool PthreadCreate();
void Pthread_Cond_Broadcast( ); /*唤醒所有阻塞线程*/
void SignalPthread_Dev();
void SignalPthread_Centre();
void SignalPthread_Wincc();
void SignalPthread_Server();

/*=====================================================================================================*/
int main(int argc, char *argv[])
{
    cout << "====================================================================" << endl;
    cout << "Enter the Business logic layer program; Business layer init start..." << endl;
    cout << "--------------------------------------------------------------------" << endl;
    InitGlobalValue();
    jInfo.InitParameters();
    jInfo.printSeverInfor();
    jInfo.printCtrParaInfo(jInfo.ctr_para);
    bool result = PthreadCreate();
    if(result == false)
        exit(1);
    else
        Pthread_Cond_Broadcast();
    sleep(1);
    LoggingRecord();
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/
bool PthreadCreate()
{
    cout << "------------------------------" << endl;
    cout << "Business layer pthread init..." << endl;
    cout << "------------------------------" << endl;
    int err;    
    err = pthread_create(&tid_device, NULL, InteractDevice, NULL);
    if(err != 0){
        perror("Can Not Create pthread for Interact device!!!");
        return false;
    }
    sleep(1);
    pthread_detach(tid_device);

    err = pthread_create(&tid_centre, NULL, InteractCentre, NULL);
    if(err != 0){
        perror("Can Not Create pthread for Interact centre!!!");
        return false;
    }
    sleep(1);
    pthread_detach(tid_centre);

    err = pthread_create(&tid_wincc, NULL, InteractWinCC, NULL);
    if(err != 0){
        perror("Can Not Create pthread for Interact WinCC!!!");
        return false;
    }
    sleep(1);
    pthread_detach(tid_wincc);

    err = pthread_create(&tid_server, NULL, AaServer, NULL);
    if(err != 0){
        perror("Can Not Create pthread for Local server!!!");
        return false;
    }
    sleep(1);
    pthread_detach(tid_server);
    return true;
}

/*唤醒所有阻塞线程*/
void Pthread_Cond_Broadcast()
{
    SignalPthread_Dev();
    SignalPthread_Centre();
    SignalPthread_Wincc();
    SignalPthread_Server();
}  


void SignalPthread_Dev()
{
    pthread_mutex_lock(&lock_Dev);
    if(wait_Dev_status == true){
        wait_Dev_status = false;
        pthread_cond_signal(&pthread_cond_Dev);
        cout<<"weak up Device interact thread!"<<endl;
    }
    pthread_mutex_unlock(&lock_Dev);
}

void SignalPthread_Centre()
{
    pthread_mutex_lock(&lock_Centre);
    if(wait_Centre_status == true){
        wait_Centre_status = false;
        pthread_cond_signal(&pthread_cond_Centre);
        cout<<"weak up Centre thread!"<<endl;
    }
    pthread_mutex_unlock(&lock_Centre);
}

void SignalPthread_Wincc()
{
    pthread_mutex_lock(&lock_Wincc);
    if(wait_Wincc_status == true){
        wait_Wincc_status = false;
        pthread_cond_signal(&pthread_cond_Wincc);
        cout<<"weak up Wincc thread!"<<endl;
    }
    pthread_mutex_unlock(&lock_Wincc);
}

void SignalPthread_Server()
{
    pthread_mutex_lock(&lock_Server);
    if(wait_Server_status == true){
        wait_Server_status = false;
        pthread_cond_signal(&pthread_cond_Server);
        cout<<"weak up Server thread!"<<endl;
    }
    pthread_mutex_unlock(&lock_Server);
}

void LoggingRecord()
{
    while(true){
        sleep(1);
    }
}