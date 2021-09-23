# Where are our moduleS?
MODULES = ../travelMonitor

# Set -I. so that include files in this directory are always found (even when compiling the modules in other dirs)
CFLAGS = -g -Wall -I. -I./src

# set the name of the executable file to compile here
PROGRAM = travelMonitor

PROGRAMM = monitor


# List object files needed to compile the program.

CODE = ./src/main.c  ./src/General.c ./src/MCImplementation.c ./src/signal.c ./src/Interface.c ./src/serialize.c ./src/Commands.c ./src/Fifo.c ./src/ExtrasImplementation.c ./src/bloomfilterImplementation.c ./src/LinkedList.c ./src/HashImplementation.c

OBJS = main.o  General.o MCImplementation.o signal.o Interface.o serialize.o Commands.o Fifo.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o HashImplementation.o

CODEM = ./src/monitor.c ./src/serialize.c ./src/General.c ./src/signal.c ./src/MonitorCommands.c ./src/Fifo.c ./src/MonitorInterface.c ./src/ExtrasImplementation.c ./src/bloomfilterImplementation.c ./src/LinkedList.c ./src/HashImplementation.c

OBJSM = monitor.o serialize.o General.o signal.o MonitorCommands.o Fifo.o MonitorInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o HashImplementation.o


$(PROGRAM): clean  $(CODE) $(CODEM)
	gcc -c $(CODE) -lm
	gcc $(OBJS) -o $(PROGRAM) -lm
	gcc -c $(CODEM) -lm
	gcc $(OBJSM) -o $(PROGRAMM) -lm



clean: cleanm
	rm -f $(PROGRAM) $(OBJS)

cleanm:
	rm -f $(PROGRAMM) $(OBJSM)

run: $(PROGRAM)
	./$(PROGRAM)


