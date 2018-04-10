#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <string.h>
#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/cdata.h"

/*Variables to define system status*/
#define INITIALIZED 1
#define NOT_INITIALIZED 0


/*System control variables*/
int systemStatus = NOT_INITIALIZED;
int counterTid = 0;
TCB_t *runningThread = NULL;
ucontext_t *end_Context = NULL;

/*System Queues*/
PFILA2 readyQueue = NULL;
PFILA2 blockedQueue = NULL;
PFILA2 blockedSuspendedQueue = NULL;
PFILA2 readySuspendedQueue = NULL;



/*HEADERS*/
TCB_t *getNextThread();
int ccreate(void* (*start)(void*), void *arg, int prio);

int cidentify();
void schedulerDispatcherManager();
void finishedThread();
int initializeSystem();
TCB_t *getThread(int threadID, PFILA2 queue);


/*FUNCTIONS*/

int ccreate(void* (*start)(void*), void *arg, int prio){
    if(initializeSystem() != 1){
        return -1;
    }

    TCB_t *newThread = (TCB_t *)malloc(sizeof(TCB_t));
    if(newThread == NULL){
        return -1;
    }

    if(getcontext(&(newThread->context)) == -1){
        return -1;
    }

    newThread->context.uc_link = end_Context;
    newThread->context.uc_stack.ss_sp = (char *)malloc(SIGSTKSZ);
    newThread->context.uc_stack.ss_size = SIGSTKSZ;
    makecontext(&(newThread->context), (void (*)(void)) start, 1, arg);

    newThread->tid = counterTid;
    counterTid++;
    newThread->state = PROCST_CRIACAO;
    newThread->prio = prio;

    if(AppendFila2(readyQueue,newThread) == 0){
        newThread->state = PROCST_APTO;
    }else{
        return -1;
    }

    return newThread->tid;
}

/**/

int cyield(void){
    if(initializeSystem() == 1){
        if(runningThread != NULL){
            if(AppendFila2(readyQueue, runningThread) == 0){
                runningThread->state = PROCST_APTO;
                schedulerDispatcherManager();
                return 0;
            }
        }
    }
    return -1;
}


int cjoin(int tid){
    return 0;
}

/*retorna 0 quando funciona corretamente
  retorna negativo caso de errado*/
int csuspend(int tid){
    TCB_t *wantedThread;
    //verifica se esta no apto ou no bloqueado
    wantedThread = getThread(tid, readyQueue);
    if(wantedThread != NULL){
        DeleteAtIteratorFila2(readyQueue);
        wantedThread->state = PROCST_APTO_SUS;
        if(AppendFila2(readySuspendedQueue, wantedThread) != 0){
            return -1;
        }
        return 0;
    }

    wantedThread = getThread(tid, blockedQueue);
    if(wantedThread != NULL){
        DeleteAtIteratorFila2(blockedQueue);
        wantedThread->state = PROCST_BLOQ_SUS;
        if(AppendFila2(blockedSuspendedQueue,wantedThread) != 0){
            return -1;
        }
        return -1;
    }
    return -1;
}



int cresume(int tid){
    TCB_t *wantedThread = NULL;
    //verifica se esta no apto ou no bloqueado

    wantedThread = getThread(tid, readySuspendedQueue);
    if(wantedThread != NULL){
        DeleteAtIteratorFila2(readySuspendedQueue);
        wantedThread->state = PROCST_APTO;
        if(AppendFila2(readyQueue, wantedThread) != 0){
            return -1;
        }
        return 0;
    }

    wantedThread = getThread(tid, blockedSuspendedQueue);
    if(wantedThread != NULL){
        DeleteAtIteratorFila2(blockedSuspendedQueue);
        wantedThread->state = PROCST_BLOQ;
        if(AppendFila2(blockedQueue, wantedThread) != 0){
            return -1;
        }
        return 0;
    }

    //thread not found
    return -1;
}

int csem_init(csem_t *sem, int count){
    if(initializeSystem() == 1){
        if(runningThread != NULL){
            sem->fila = (PFILA2)malloc(sizeof(FILA2));
            if(sem->fila != NULL){
                if(CreateFila2(sem->fila) == 0){
                    sem->count = count;
                    return 0;
                }
            }
        }
    }
    return -1;
}


int cwait(csem_t *sem){

    return 0;
}


int csignal(csem_t *sem){

    return 0;
}


int cidentify(char *names, int size){
    char groupIdentification[] = "Gunter Hertz - 220491\nCristiano Lunardi - 240508";
    if(initializeSystem() == 1){
        memcpy(names, groupIdentification, size);
        return 0;
    }else{
        return -1;
    }
}


/*AUXILIAR FUNCTIONS*/
//Return thread if is on queue else return NULL
TCB_t *getThread(int threadID, PFILA2 queue){
    TCB_t *wantedThread;

    if(FirstFila2(queue) != 0){
        return NULL;
    }
    while(NextFila2(queue) != NXTFILA_ENDQUEUE){
        wantedThread = GetAtIteratorFila2(queue);
        if(wantedThread->tid == threadID){
            return wantedThread;
        }
    }
    return NULL;
}


TCB_t *getNextThread(){
    TCB_t *nextThread;
    if(FirstFila2(readyQueue) != 0){
        nextThread = GetAtIteratorFila2(readyQueue);
        DeleteAtIteratorFila2(readyQueue);
    }
    return nextThread;
}

void schedulerDispatcherManager(){

    TCB_t *previousThread = runningThread;
    TCB_t *nextThread = NULL;

    if(initializeSystem() == 1){
        nextThread = getNextThread();
    }

    if(nextThread != NULL){
        nextThread->state = PROCST_EXEC;
        runningThread = nextThread;
        runningThread->state = PROCST_EXEC;
        if(previousThread != NULL){
            swapcontext(&(previousThread->context), &(nextThread->context));
        }else{
            setcontext(&(nextThread->context));
        }
    }else{
        printf("ERRO: ESCALONADOR SEM THREADS\n");
        exit(1);
    }
}


int initializeSystem(){
	if (systemStatus == NOT_INITIALIZED){
	    //initialize queues
	    if(blockedQueue == NULL){
	        blockedQueue = (PFILA2)malloc(sizeof(FILA2));
	        if(blockedQueue == NULL){
	            return -1;
	        }
	        if(CreateFila2(blockedQueue) != 0){
	        return -1;
	        }
	    }

   	    if(blockedSuspendedQueue == NULL){
	        blockedSuspendedQueue = (PFILA2)malloc(sizeof(FILA2));
	        if(blockedSuspendedQueue == NULL){
	            return -1;
	        }
	        if(CreateFila2(blockedSuspendedQueue) != 0){
	        return -1;
	        }
	    }

        if(readyQueue == NULL){
	        readyQueue = (PFILA2)malloc(sizeof(FILA2));
	        if(readyQueue == NULL){
	            return -1;
	        }
	        if(CreateFila2(readyQueue) != 0){
	        return -1;
	        }
	    }

        if(readySuspendedQueue == NULL){
	        readySuspendedQueue = (PFILA2)malloc(sizeof(FILA2));
	        if(readySuspendedQueue == NULL){
	            return -1;
	        }
	        if(CreateFila2(readySuspendedQueue) != 0){
	        return -1;
	        }
	    }


	    //create a final context for threads
        if(end_Context == NULL){
            end_Context = (ucontext_t *)malloc(sizeof(ucontext_t));

            if(end_Context == NULL){
                return -1;
            }
            if(getcontext(end_Context) == -1){
                return -1;
            }
            end_Context->uc_link = NULL;
            end_Context->uc_stack.ss_sp = (char *)malloc(SIGSTKSZ);
            end_Context->uc_stack.ss_size = SIGSTKSZ;
            makecontext(end_Context, (void (*)(void)) finishedThread, 0);
        }

        systemStatus = INITIALIZED;
	}
	return 1;
}


void finishedThread(){
    if(initializeSystem() == 1){
        if(runningThread->context.uc_link != end_Context){
            setcontext(runningThread->context.uc_link);
        }
        //release resources
        runningThread->state = PROCST_TERMINO;
        free(runningThread->context.uc_stack.ss_sp);
        free(runningThread);
        runningThread = NULL;
        //call next thread to execute
        schedulerDispatcherManager();
    }
}
