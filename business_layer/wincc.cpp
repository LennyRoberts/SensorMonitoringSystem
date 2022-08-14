#include "wincc.hpp"


void *InteractWinCC(void *arg)
{
    pthread_t tid;
	tid = pthread_self( );
	printf("pthread for Interact WinCC: tid= %lld (0x%llX)\n", (long long)tid, (long long)tid);


	pthread_mutex_lock(&lock_Wincc);
	wait_Wincc_status = true;
	while(wait_Wincc_status == true)
		pthread_cond_wait(&pthread_cond_Wincc, &lock_Wincc);
	wait_Wincc_status = false;
	pthread_mutex_unlock(&lock_Wincc);
	sleep(1);

    while(true){
        sleep(1);
    }
}
