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
int initializeSystem();
void finishedThread();



int cidentify(char *names, int size){
    char groupIdentification[] = "Gunter Hertz - 220491\nCristiano Lunardi - 240508";
    if(initializeSystem() == 1){
        memcpy(names, groupIdentification, size);
        return 0;
    }else{
        return -1;
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
        //CALL SCHEDULER
    }
}
