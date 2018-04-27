CC=gcc
LIB_DIR=../lib
INC_DIR=../include
BIN_DIR=../bin
SRC_DIR=../src

all:  test join show semaforo

join: $(LIB_DIR)/libcthread.a ./Join.c
	$(CC) -o Join Join.c -L$(LIB_DIR) -lcthread -Wall


show: $(LIB_DIR)/libcthread.a ./Show.c
	$(CC) -o Show Show.c -L$(LIB_DIR) -lcthread -Wall

test: $(LIB_DIR)/libcthread.a ./ThreadTest.c
	$(CC) -o ThreadTest ThreadTest.c -L$(LIB_DIR) -lcthread -Wall

semaforo: $(LIB_DIR)/libcthread.a ./Semaforo.c
	$(CC) -o Semaforo Semaforo.c -L$(LIB_DIR) -lcthread -Wall

clean:
	rm -rf ./TheadTest ./Join ./Show ./Semaforo
