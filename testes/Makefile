all:  temp restaurante semaforo

join: ../lib/libcthread.a ./Join.c
	gcc -o Join Join.c -L../lib -lcthread -Wall

semaforo: ../lib/libcthread.a ./Semaforo.c
	gcc -o Semaforo Semaforo.c -L../lib -lcthread -Wall
	
restaurante: ../lib/libcthread.a ./Restaurante.c
	gcc -o Restaurante Restaurante.c -L../lib -lcthread -Wall

temp: ../lib/libcthread.a ./Temp.c
	gcc -o Temp Temp.c -L../lib -lcthread -Wall

clean:
	rm -rf ./Temp ./Restaurante ./Semaforo
