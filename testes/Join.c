#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *function1()
{
	printf("Function1 - Ganha o processador.\n");
	printf("Function1 - Libera para outra thread.\n");
	cyield();
	printf("Function1 - Thread da function1 ganha processador.\n");
	printf("Function1 - Thread da function1 finalizada.\n");

	return NULL;
}

void *function2 ()
{
	printf("Function2 - Ganha o processador.\n");
	printf("Function2 - Thread da function2 finalizada.\n");

	return NULL;
}

void *function3 ()
{
	printf("Function3 - Ganha o processador.\n");
	printf("Function3  -Libera para outra thread.\n");
	cyield();
	
	printf("Function3 - Thread da function3 ganha processador.\n");
	int tid2 = ccreate(function2, NULL, 5);
	if (tid2 != -1)
	{
		printf("Thread da function2 criada.\n");
		printf("Function3 - function 3 se bloqueia usando join na function2.\n");
		cjoin(tid2);
	}
	
	printf("Function3 - Thread da function3 ganha processador.\n");
	printf("Function3 - Thread da function3 finalizada.\n");

	return NULL;
}

int main()
{
	printf("Main - Thread main executando\n");

	int td1 = ccreate(function1, NULL, 5);
	if (td1 != 0)
		printf("Thread da function1 criada.\n");
	
	int td3 = ccreate(function3, NULL, 5);
	if (td3 != 0)
	{
		printf("Thread da function3 criada.\n");
		printf("Main se bloqueia ate o termino da function3.\n");
		cjoin(td3);
	}
	
	printf("Main - Thread main ganha processador.\n");
	printf("Main -Thread main finalizada.\n");

	return 0;
}
