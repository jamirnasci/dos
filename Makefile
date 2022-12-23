CC=gcc
LIB=-l pthread -l ws2_32
OBJ=obj/main.o
CODE=main.c
TARGET=bin/dos

all: main.o
	$(CC) $(OBJ) $(LIB) -o $(TARGET)
main.o: $(CODE)
	$(CC) -c $(CODE) $(LIB) -o $(OBJ)