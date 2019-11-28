# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: SPLFLIX

# Tool invocations

SPLFLIX: bin/main.o bin/Action.o bin/User.o bin/Watchable.o bin/Session.o 
	@echo 'Building target: SPLFLIX'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/SPLFLIX bin/main.o bin/Action.o bin/User.o bin/Watchable.o bin/Session.o $(LFLAGS)
	@echo 'Finished building target: SPLFLIX'
	@echo ' '
	
# Depends on the source and header files
bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp
	
# Depends on the source and header files
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
	
# Depends on the source and header files
bin/User.o: src/User.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp
	
# Depends on the source and header files
bin/Watchable.o: src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
	
# Depends on the source and header files
bin/Session.o: src/Session.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp
	
#Clean the build directory
clean: 
	rm -f bin/*
