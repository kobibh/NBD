#COMPILER
CC		= g++

CFLAGS	+= -std=c++11 -pedantic-errors -Wall -Wextra -g -lpthread -I../include -fPIC
OBJS = DriverError.o DriverData.o NBDDriverCommunicator.o RAMStorage.o StorageError.o
LDFLAGS:= -shared

all:  main 

DriverError:
	$(CC) $(CFLAGS) DriverError.cpp	-c

DriverData: DriverError
	$(CC) $(CFLAGS) DriverData.cpp	-c

NBDDriverCommunicator: NBDDriverCommunicator.cpp DriverData
	$(CC) $(CFLAGS) NBDDriverCommunicator.cpp	-c

StorageError: StorageError.cpp
	$(CC) $(CFLAGS) StorageError.cpp -c

RAMStorage: RAMStorage.cpp StorageError
	$(CC) $(CFLAGS) RAMStorage.cpp -c

main: NBDDriverCommunicator RAMStorage 
	$(CC) $(OBJS) Logger.cpp main.cpp  $(CFLAGS) -o main

#for testing purposes
RAMStorageTest : RAMStorage DriverData
	$(CC) $(OBJS) RAMStorageTest.cpp $(CFLAGS) -o RAMStorageTest

#for testing purposes
logger_app: Logger.o
	$(CC) Logger.o singleton_test.cpp -o logger_app $(CFLAGS)

#for testing purposes
libLogger.so: 
	$(CC) $(LDFLAGS) Logger.cpp LoggerWrapper.cpp $(CFLAGS) -L. -Wl,-rpath=. $^ -ldl -o $@ 

clean:
	rm -f $(OBJS) main
