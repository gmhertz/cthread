#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/cthread.h"

void *testeCiyeld()
{
	printf("Entrou funcao auxiliar\n");

	cyield();
	printf("Terminou testeCiyeld\n");

	return NULL;
}

void *testeCiyeld2(){
	printf("testeCiyeld2\n");

	cyield();
	printf("Terminou testeCiyeld2\n");
	return NULL;
}

void *testeCiyeld3(){
	printf("testeCiyeld3\n");

	cyield();

	printf("Terminou testeCiyeld3\n");
	return NULL;
}


int main()
{
	
	int tid1 = ccreate(testeCiyeld, NULL, 5);
	if (tid1 != -1)
		printf("Criou a thread.\n");

	int tid2 = ccreate(testeCiyeld2, NULL, 3);
	if(tid2 != -1)
		printf("Criou testeCiyeld2\n");


	int tid3 = ccreate(testeCiyeld3, NULL, 3);
	if(tid3 != -1){
		printf("Criou testeCiyeld3\n");
	}
	printf("libera cpu\n");
	cyield();
	printf("Retoma cpu\n");
	cjoin(tid1);
	// cjoin(tid2);
	// cjoin(tid3);
	printf("Acaba main\n");
	return 0;	
}