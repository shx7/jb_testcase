CC=g++
CFLAGS=-std=c++11 -Wall -Wextra
LIBS=-lboost_filesystem -lboost_system -lssl -lcrypto -lboost_program_options

all: main file_classifier util
	$(CC) $(CFLAGS) $(LIBS) main.o FileClassifier.o Util.o -o main.elf

main:
	$(CC) $(CFLAGS) $(LIBS) main.cpp -c

file_classifier:
	$(CC) $(CFLAGS) $(LIBS) FileClassifier.cpp -c

util:
	$(CC) $(CFLAGS) $(LIBS) Util.cpp -c

clean:
	rm *.o
	rm *.elf
