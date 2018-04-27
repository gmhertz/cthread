#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *functionAlpha(){
	printf("Thread Alpha ganha processador.\n");
	printf("Alpha suspende a Bravo.\n");
	csuspend(2);
	printf("Alpha encerra.\n");
	return NULL;
}

void *functionBravo(){
	printf("Thread Bravo ganha processador.\n");
	printf("Bravo suspende a Charlie.\n");
	csuspend(3);
	printf("Bravo encerra.\n");
	return NULL;
}


void *functionCharlie(){
	printf("Thread Charlie ganha processador.\n");
	printf("Charlie suspende a Delta.\n");
	csuspend(3);
	printf("Bravo encerra.\n");
	return NULL;
}

void *functionEcho(){
	printf("Thread Echo ganha processador.\n");
	printf("Echo libera o processador.\n");
	cyield();
	printf("Echo encerra.\n");
	return NULL;

}

void *functionDelta(){
	int tid5;
	printf("Thread Delta ganha processador.\n");
	tid5 = ccreate(functionEcho, NULL, 5);
	if(tid5 != 0){
		printf("Thread Echo criada corretamente.\n");
	}
	cjoin(tid5);
	printf("Delta encerra.\n");
	return NULL;
}



int main(){
	int tid1, tid2, tid3, tid4;

	tid1 = ccreate(functionAlpha, NULL, 5);
	if(tid1 != 0){
		printf("Thread Alpha criada corretamente.\n");
	}

	tid2 = ccreate(functionBravo, NULL, 5);
	if(tid2 != 0){
		printf("Thread Bravo criada corretamente.\n");
	}

	tid3 = ccreate(functionCharlie, NULL, 5);
	if(tid3 != 0){
		printf("Thread Charlie criada corretamente.\n");
	}

	tid4 = ccreate(functionDelta, NULL, 5);
	if(tid4 != 0){
		printf("Thread Delta criada corretamente.\n");
	}





	
	cjoin(tid1);
	printf("Main desbloqueia a Bravo\n");
	cresume(tid2);
	cjoin(tid2);
	printf("Main desbloqueia a Charlie\n");
	cresume(tid3);
	cjoin(tid3);
	printf("Main desbloqueia a Delta\n");
	cresume(tid4);
	cjoin(tid4);
	printf("Main encerra.\n");

	return 0;
}