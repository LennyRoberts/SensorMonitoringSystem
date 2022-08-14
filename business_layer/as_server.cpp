#include "as_server.hpp"


void *AaServer(void *arg)
{
    pthread_t tid;
	tid = pthread_self( );
	printf("pthread for Interact local as server: tid= %lld (0x%llX)\n", (long long)tid, (long long)tid);


	pthread_mutex_lock(&lock_Server);
	wait_Server_status = true;
	while(wait_Server_status == true)
		pthread_cond_wait(&pthread_cond_Server, &lock_Server);
	wait_Server_status = false;
	pthread_mutex_unlock(&lock_Server);
	sleep(1);

    while(true){
        sleep(1);
    }
}