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
	char Buffer[SHARED_MEMORY_SIZE];
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
	//pFile = fopen("test1g.txt", "rb");
	// pFile = fopen("1G.dummy", "rb");
	pFile = fopen("test.txt", "rb");
	if (pFile == NULL) {
        perror("File Open Error");
		exit(0);
	}

	int iReadSize = 0;
	long lSendFileSize = 0;
	fseek(pFile, 0, SEEK_END);
	long lTotFileSize = ftell(pFile);
	rewind(pFile);

	// while(lSendFileSize != lTotFileSize)
	while(feof(pFile) == 0)
	{
		printf(" 111  str : (%d, %d) %s\n", strlen(pShareMsg), sizeof(pShareMsg), pShareMsg);
        sem_wait(SemId);
		if(strlen(pShareMsg) == 0) {
			if ( (iReadSize = fread(pShareMsg, 1, SHARED_MEMORY_SIZE, pFile)) < 0) {
				printf("Err fread\n");
				break;
			}

			printf(" 222 str : (%d) %s\n", strlen(pShareMsg), pShareMsg);
			lSendFileSize =  lSendFileSize + iReadSize;
			printf("파일 읽어 메모리에 쓰는중 : %ld / %ld \n", lSendFileSize, lTotFileSize);
		}
		printf("sleep 2 \n");
		sleep(2);
        sem_post(SemId);
	}


	printf("파일 읽어 메모리에 업로드 완료 : %ld \n", lSendFileSize);

	while(strlen(pShareMsg) > 0) { sleep (0); }

	printf("마무리~\n");
    sem_wait(SemId);
	*pShareMsg  = '#';
    sem_post(SemId);

	fclose(pFile);
	return 1;
}