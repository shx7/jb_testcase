CC=g++
CFLAGS=-std=c++11 -Wall -Wextra
LIBS=-lboost_filesystem -lboost_system -lssl -lcrypto -lboost_program_options

all: main
	$(CC) $(CFLAGS) $(LIBS) main.o FileClassifier.o -o main.elf

main: main.cpp file_classifier
	$(CC) $(CFLAGS) $(LIBS) main.cpp -c

file_classifier:
	$(CC) $(CFLAGS) $(LIBS) FileClassifier.cpp -c

clean:
	rm *.o
	rm *.elf
