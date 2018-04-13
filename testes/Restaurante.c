#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *cliente3()
{
	printf("Cliente3 esta comendo.\n");
	cyield();
	printf("Cliente3 vai embora.\n");

	return NULL;
}

void *cliente1(void *rest)
{
	csem_t *restaurante = rest;

	printf("Cliente1 vai ao balcao.\n");
	
	cwait(restaurante);
	printf("Cliente1 senta na mesa.\n");
	
	int tid3 = ccreate(cliente3, NULL);
	if (tid3 != -1)
	{
		printf("Cliente3 entra no restaurante.\n");
		printf("Cliente3 senta na mesa do Cliente1.\n");
		cjoin(tid3);
	}
	
	printf("Cliente1 libera a mesa.\n");
	csignal(restaurante);
	
	printf("Cliente1 vai embora.\n");
	return NULL;	
}

void *cliente2(void *rest)
{
	csem_t *restaurante = rest;

	printf("Cliente2 vai ao balcao.\n");
	
	printf("Cliente2 pergunta se ha mesas livres.\n");
	if (restaurante->count > 0)
	{
		cwait(restaurante);
		printf("Cliente2 senta na mesa.\n");
		printf("Cliente2 esta comendo.\n");
		cyield();
		printf("Cliente2 libera a mesa.\n");
		csignal(restaurante);
	}
	else
		printf("Nao ha mesas livres.\n");
	
	printf("Cliente2 vai embora.\n");
	return NULL;	
}

void *cliente4(void *rest)
{
	csem_t *restaurante = rest;

	printf("Cliente4 vai ao balcao.\n");
	
	printf("Cliente4 pergunta se ha mesas livres.\n");
	if (restaurante->count <= 0)
		printf("Cliente4 espera liberar uma mesa.\n");
	
	if (cwait(restaurante) == 0)
	{
		printf("Cliente4 senta na mesa.\n");
		printf("Cliente4 nao gosta da comida.\n");
		printf("Cliente4 libera a mesa.\n");
		csignal(restaurante);
	}
	
	printf("Cliente4 vai embora.\n");
	return NULL;
}

int main()
{
	csem_t restaurante;
	
	int tid1 = ccreate(cliente1, &restaurante);
	if (tid1 != -1)
		printf("Cliente1 entra no restaurante.\n");
	
	int tid2 = ccreate(cliente2, &restaurante);
	if (tid2 != -1)
		printf("Cliente2 entra no restaurante.\n");

	int tid4 = ccreate(cliente4, &restaurante);
	if (tid4 != -1)
		printf("Cliente4 entra no restaurante.\n");
	
	csem_init(&restaurante,2);
	
	cjoin(tid1);
	cjoin(tid2);
	cjoin(tid4);

	return 0;	
}
