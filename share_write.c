#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <unistd.h> 
#include "share_memory.h"

int main()
{
	int shmid;
	int i;
	SHM_INFOS *shm_info= NULL;
	void *shared_memory = (void *)0;


	shmid = shmget((key_t)3836, sizeof(SHM_INFOS)*SHM_INFO_COUNT, 0666|IPC_CREAT);

	if (shmid == -1)
	{
		perror("shmget failed : ");
		exit(0);
	}

	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1)
	{
		perror("shmat failed : ");
		exit(0);
	}

	shm_info = (SHM_INFOS *)shared_memory;

	while(1)
	{
		for(i=0 ;i < SHM_INFO_COUNT; i++){
			snprintf(shm_info[i].str_ip,sizeof(shm_info[i].str_ip),"1.1.1.%d",i);
			shm_info[i].int_ip =  12891010 +i;
			shm_info[i].int_id =  128 + i;
		}
	}
}
