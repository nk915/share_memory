#include "share_memory.h"

/*
	1. 세마포어 만든다.  세마포어는 공유메모리 전송 시 lock신호를 위해 사용
	   - 세마포어 경로는 공유 폴더에 저장.
*/


int main()
{
	int iLoopValue;
	int iShmId;
	char* pShareMsg = NULL;
	
	void *shared_memory = (void *)0;


	// 공유메모리 공간 Create
	iShmId = shmget((key_t)3836, SHARED_MEMORY_SIZE, 0666|IPC_CREAT);
	if (iShmId == -1) {
		perror("shmget failed \n");
		exit(0);
	}

	// 공유메모리 attach	
	shared_memory = shmat(iShmId, (void *)0, 0666|IPC_CREAT);
	if (shared_memory == (void *)-1) {
		perror("shmat attach is failed : ");
		exit(0);
	}

	pShareMsg = (char *)shared_memory;

	// semaphore 오픈
    sem_t *SemId;
    sem_unlink("mysem");
    if((SemId = sem_open("mysem", O_CREAT, 0777, 1)) == NULL)  {
        perror("Sem Open Error");
		exit(0);
	}

	// File Open
	FILE* pFile;
	pFile = fopen("test2.txt", "wb+");
	if (pFile == NULL) {
        perror("File Open Error");
		exit(0);
	}

	int iReadSize = 0;
	long lRecvFileSize = 0;
	long lTotFileSize = 0;

	while(*pShareMsg != '#')
	{
		printf("111\n");
        sem_wait(SemId);
		if(strlen(pShareMsg) > 0) {
			printf("메모리를 읽어 파일에 쓰는중 \n");
			// fwrite(pShareMsg, 1, SHARED_MEMORY_SIZE, pFile);
			fprintf(pFile, "%s", pShareMsg);
			fflush(pFile);
			memset(pShareMsg, 0, SHARED_MEMORY_SIZE);
		}
		printf("sleep 2\n");
		sleep(2);
        sem_post(SemId);
	//	if (iReadSize = fwrite(pShareMsg, 1, SHARED_MEMORY_SIZE, pFile) < 0) {
	//		printf("Err fread\n");
	//		break;
	//	}
		// lRecvFileSize += iReadSize;
	}
	printf("파일 읽어 메모리에 업로드 완료 \n");

	shmdt(shared_memory);
	
	if( -1 == shmctl(iShmId, IPC_RMID, 0)) {
		printf("공유 메모리 반환 실패 \n");
	}
	else {
		printf("공유 메모리 반환 성공 \n");
	}
	fclose(pFile);

	return 1;
}
