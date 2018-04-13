#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *function1()
{
	printf("\nFunction1\n\n");
	printf("Libera para outra thread.\n");
	cyield();
	printf("\nThread da function1 ganha processador.\n");
	printf("Thread da function1 finalizada.\n");

	return NULL;
}

void *function2 ()
{
	printf("\nFunction2\n\n");
	printf("Thread da function2 finalizada.\n");

	return NULL;
}

void *function3 ()
{
	printf("\nFunction3\n\n");
	printf("Libera para outra thread.\n");
	cyield();
	
	printf("\nThread da function3 ganha processador.\n");
	int tid2 = ccreate(function2, NULL, 5);
	if (tid2 != -1)
	{
		printf("Thread da function2 criada.\n");
		printf("Usando join na function2.\n");
		cjoin(tid2);
	}
	
	printf("\nThread da function3 ganha processador.\n");
	printf("Thread da function3 finalizada.\n");

	return NULL;
}

int main()
{
	printf("Thread main\n\n");
	int td1 = ccreate(function1, NULL, 5);
	if (td1 != -1)
		printf("Thread da function1 criada.\n");
	
	int td3 = ccreate(function3, NULL, 5);
	if (td3 != -3)
	{
		printf("Thread da function3 criada.\n");
		printf("Usando join na function3.\n");
		cjoin(td3);
	}
	
	printf("\nThread main ganha processador.\n");
	printf("Thread main finalizada.");

	return 0;
}
