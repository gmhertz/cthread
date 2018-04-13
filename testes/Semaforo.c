#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *function1(void *sem)
{
	printf("\nFunction1\n\n");
	printf("Libera para outra thread.\n");
	cyield();
	printf("\nThread da function1 ganha processador.\n");
	printf("Verifica se tem recurso disponivel (semaforo).\n");
	
	if(cwait(sem)==0)
	{
		printf("Thread da function1 utilizando recurso do semaforo.\n");
		printf("Liberando semaforo.\n");
		csignal(sem);
	}
	
	printf("Thread da function1 finalizada.\n");
	return NULL;
}

void *function2 (void *sem)
{
	printf("\nFunction2\n\n");
	
	if(cwait(sem)==0)
	{
		printf("Thread da function2 utlizando recurso do semaforo.\n");
		printf("Libera para outra thread.\n");
		cyield();

		printf("\nThread da function2 ganha processador.\n");
		printf("Liberando semaforo.\n");
		csignal(sem);
	}
	
	printf("Thread da function2 finalizada.\n");
	return NULL;
}


int main()
{
	/*iniciando a estrutura do semaforo*/
	csem_t semaforo;
	
	printf("Thread main\n\n");
	
	int tid1 = ccreate(function1, &semaforo,5);
	if (tid1 != -1)
		printf("Thread da function1 criada.\n");
	
	int tid2 = ccreate(function2, &semaforo, 5);
	if (tid2 != -1)
		printf("Thread da function2 criada.\n");
	
	csem_init(&semaforo,1);
	printf("Esperando function1 e function2 finalizarem.\n");
	cjoin(tid2);
	cjoin(tid1);

	printf("\nThread main ganha processador.\n");
	printf("Thread main finalizada.");

	return 0;
}
