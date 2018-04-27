#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *cliente1(void *sem){
	printf("Cliente1 deseja reservar a poltrona X\n");
	printf("Cliente1 perde o processador\n");
	cyield();
	if(cwait(sem) == 0){
		printf("Cliente1 entra na zona controlada (semaforo)\n");
		printf("Cliente1 perde o processador.\n");
		cyield();
		printf("Cliente1 efetua a compra a libera recurso.\n");
		csignal(sem);
	}
	printf("Cliente1 termina operacao\n");
	return NULL;
}

void *cliente2(void *sem){
	printf("Cliente2 deseja reservar a poltrona X\n");
	
	if(cwait(sem) == 0){
		printf("Cliente2 entra na zona controlada (semaforo)\n");
		printf("Cliente2 perde o processador.\n");
		cyield();
		printf("Cliente2 efetua a compra a libera recurso.\n");
		csignal(sem);
	}
	printf("Cliente2 termina operacao\n");
	return NULL;
}

void *cliente3(void *sem){
	printf("Cliente3 deseja reservar a poltrona X\n");
	printf("Cliente3 perde o processador pois nao tem recurso\n");
	
	if(cwait(sem) == 0){
		printf("Cliente3 entra na zona controlada (semaforo)\n");
		printf("Cliente3 perde o processador.\n");
		cyield();
		printf("Cliente3 efetua a compra a libera recurso.\n");
		csignal(sem);
	}
	printf("Cliente3 termina operacao\n");
	return NULL;
}

int main(){
	int tid1,tid2,tid3;
	/*iniciando a estrutura do semaforo*/
	csem_t semaforo;

	printf("Sistema de venda de passagens aereas.\n");


	tid1 = ccreate(cliente1, &semaforo, 1);
	if(tid1 != 0)
		printf("Cliente1 logado com sucesso.\n");

	tid2 = ccreate(cliente2, &semaforo, 1);
	if(tid2 != 0)
		printf("Cliente2 logado com sucesso.\n");


	tid3 = ccreate(cliente3, &semaforo, 1);
	if(tid3 != 0)
		printf("Cliente3 logado com sucesso.\n");

	//inicializa semaforo
	csem_init(&semaforo,1);

	cjoin(tid1);
	cjoin(tid2);
	cjoin(tid3);

	printf("Main termina a execucao.\n");
	return 0;
}