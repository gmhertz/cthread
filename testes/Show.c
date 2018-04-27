/*Este programa tem como objetivo simular um evento, com bandas fazendo show. Fez-se o uso de primitivas de cedencia, bloqueio o uso de semaforo*/


#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *show1(void *sem){

	printf("Banda 1 se prepara para entrar no palco.\n");

	if(cwait(sem) == 0){
		printf("Banda 1 entra no palco e faz o show.\n");
		printf("Intervalo do show da banda 1.\n");
		cyield();
		printf("Banda 1 encerra o show e libera o palco.\n");
		csignal(sem);
	}
	return NULL;
}

void *show2(void *sem){
	printf("Banda 2 se prepara para entrar no palco.\n");
	printf("Ocorreu um contratempo e a banda tem que esperar o baterista.\n");
	cyield();

		if(cwait(sem) == 0){
		printf("Banda 2 entra no palco e faz o show.\n");
		printf("Intervalo do show da banda 2.\n");
		cyield();
		printf("Banda 2 encerra o show e libera o palco.\n");
		csignal(sem);
	}
	return NULL;
}

void *show3(void *sem){
	printf("Banda 3 se prepara para entrar no palco.\n");
		if(cwait(sem) == 0){
		printf("Banda 3 entra no palco e faz o show.\n");
		printf("Intervalo do show da banda 3.\n");
		cyield();
		printf("Banda 3 encerra o show e libera o palco.\n");
		csignal(sem);
	}
	return NULL;	
}



int main(){
	printf("Show comeca em 30 minutos.\n");
	int banda1, banda2, banda3;
	/*iniciando a estrutura do semaforo*/
	csem_t palco;

	csem_init(&palco,1);

	banda1 = ccreate(show1, &palco, 5);
	if(banda1 != 0){
		printf("Banda 1 chega ao local de evento.\n");
	}

	banda2 = ccreate(show2, &palco, 5);
	if(banda2 != 0){
		printf("Banda 2 chega ao local do evento.\n");
	}

	banda3 = ccreate(show3, &palco, 5);
	if(banda3 != 0){
		printf("Banda 3 chega ao local do evento.\n");
	}





	cjoin(banda1);
	cjoin(banda2);
	cjoin(banda3);
	printf("Termino do show\n");

	return 0;
}