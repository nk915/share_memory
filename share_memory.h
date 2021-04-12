#ifndef __SHARE_MEMORY_H__
#define __SHARE_MEMORY_H__


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h>
#include <semaphore.h>

#define SEM_PATH				/mvol/sem
#define SEMKEY_PATH				SEM_PATH/key
#define SEMSTATUS_PATH			SEM_PATH/status
#define SHARED_MEMORY_SIZE 		1024 * 1024



typedef struct _shm_info{
	char         str_ip[40];
	unsigned int int_ip;
	unsigned int int_id;
}SHM_INFOS;



#endif//__SHARE_MEMORY_H__
