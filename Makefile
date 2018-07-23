CC=gcc

all: example.o json/json.o json/helper.o
	$(CC) -o example json/helper.o json/json.o  example.o


test: test.o json/json.o json/helper.o
	$(CC) -o test json/helper.o json/json.o test.o


main: main.o json.o

