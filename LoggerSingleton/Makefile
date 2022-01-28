#for testing purposes
CC=g++
CFLAGS=-std=c++11 -pedantic-errors -Wall -Wextra -g -I../include -fPIC
LDFLAGS=-L. -Wl,-rpath=. -ldl -lpthread 

all: ImplicitLoggerSingletonTest ExplicitLoggerSingletonTest

libLogger.so: 
	$(CC) $(LDFLAGS) Logger.cpp LoggerWrapper.cpp $(CFLAGS) -L. -Wl,-rpath=. -shared $^ -ldl -o $@ 

ImplicitLoggerSingletonTest: ImplicitLoggerSingletonTest.cpp LoggerWrapper.cpp libLogger.so
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -lLogger -o $@

ExplicitLoggerSingletonTest: ExplicitLoggerSingletonTest.cpp LoggerWrapper.cpp Logger.cpp 
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -rdynamic -o $@

clean:
	rm -f log/log.txt *.o ImplicitLoggerSingletonTest ExplicitLoggerSingletonTest libLogger.so