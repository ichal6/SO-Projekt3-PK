#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <time.h>

#define SEM_PRODUCER 0
#define SEM_CONSUMER 1


int pamiec;
int odlaczenie1;
int odlaczenie2;
char *adres;
char* ASCIIcode;
int const sizeOfSemaphore = 2;
key_t key;
int semaphoreId;
FILE *outputFile;

void createSharedMemory()
  {
	pamiec=shmget(key, 256, 0777|IPC_CREAT);
    if (pamiec==-1) 
      {
        printf("Problemy z utworzeniem pamieci dzielonej.\n");
        exit(EXIT_FAILURE);
      }
    else printf("Pamiec dzielona zostala utworzona : %d\n",pamiec);
  }

void attachSharedMemory()
  {
    adres=shmat(pamiec,0,0);
    if (*adres==-1)
      {
        printf("Problem z przydzieleniem adresu.\n");
        exit(EXIT_FAILURE);
      }
  }

void detachSharedMemory()
  {
    odlaczenie1=shmctl(pamiec,IPC_RMID,0);
    // sleep(5);
    odlaczenie2=shmdt(adres);
    if (odlaczenie1==-1 || odlaczenie2==-1)
      {
        printf("Problemy z odlaczeniem pamieci dzielonej.\n");
        exit(EXIT_FAILURE);
      }
    else printf("Pamiec dzielona zostala odlaczona.\n");
  }

void openFile(){
    outputFile = fopen("output.txt", "w+");
    if(outputFile == NULL)
    {
        perror("Blad otwierania pliku wyjsciowego!");
        exit(-1);
    }
}

key_t getKey(){
    if((key = ftok(".",'Z')) == -1)
    {
        perror("Problem with generate a key!");
        exit(2);
    }
}

static void getSemaphore(){
    semaphoreId=semget(key, sizeOfSemaphore, 0600|IPC_CREAT);
    if(semaphoreId==-1){
        perror("Problem with create a semaphore.");
        exit(EXIT_FAILURE);
    } else{
        printf("Semaphore has created! Id - %d\n", semaphoreId);
    }  
}

static void semaphoreDown(int nr){
    int returnValue;
    struct sembuf semaphoreBufor;
    semaphoreBufor.sem_num = nr;
    semaphoreBufor.sem_op = -1;
    semaphoreBufor.sem_flg = SEM_UNDO;
    returnValue = semop(semaphoreId, &semaphoreBufor, 1);
    if(returnValue == -1){
        if(errno == EINTR){
            semaphoreDown(nr);
        } else{
            perror("Problem with close a semaphore.");
            exit(EXIT_FAILURE);
        }
    }
}

static void semaphoreUp(int nr){
    int returnValue;
    struct sembuf semaphoreBufor;
    semaphoreBufor.sem_num = nr;
    semaphoreBufor.sem_op = 1;
    semaphoreBufor.sem_flg = SEM_UNDO;
    returnValue = semop(semaphoreId, &semaphoreBufor, 1);
    if(returnValue == -1){
        if(errno == EINTR){
            semaphoreUp(nr);
        } else{
            perror("Problem with close a semaphore.");
            exit(EXIT_FAILURE);
        }
    }
}


void consumption(){
    srand(time(NULL));
    while(1)
    {
        if(*adres == EOF){
          printf("Producent was finished work.\n");
          break;
        }
        printf("I'm waiting for product\n");
        semaphoreDown(SEM_CONSUMER);
        sleep((rand()%3)+1);

        if(*adres != EOF){
          fprintf(outputFile, "%s", adres);
          printf("Odczytalem: %s\n", adres);
        }

        semaphoreUp(SEM_PRODUCER);
        sleep((rand()%3)+1);
    }
}


int main(int argc, char* argv[])
  {
    getKey();
    createSharedMemory();
    getSemaphore();
    attachSharedMemory();
    openFile();

    consumption();
    
    detachSharedMemory();
    fclose(outputFile);
    exit(EXIT_SUCCESS);
  }
