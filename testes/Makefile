all:  test join show semaforo

join: ../lib/libcthread.a ./Join.c
	gcc -o Join Join.c -L../lib -lcthread -Wall


show: ../lib/libcthread.a ./Show.c
	gcc -o Show Show.c -L../lib -lcthread -Wall

test: ../lib/libcthread.a ./ThreadTest.c
	gcc -o ThreadTest ThreadTest.c -L../lib -lcthread -Wall

semaforo: ../lib/libcthread.a ./Semaforo.c
	gcc -o Semaforo Semaforo.c -L../lib -lcthread -Wall

clean:
	rm -rf ./TheadTest ./Join ./Show ./Semaforo
