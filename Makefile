all: clean main.o MCImplementation.o General.o Interface.o signal.o MonitorCommands.o Commands.o serialize.o monitor.o Fifo.o MonitorInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o HashImplementation.o
	gcc main.o  General.o MCImplementation.o signal.o Interface.o serialize.o Commands.o Fifo.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o HashImplementation.o -o travelMonitor
	gcc monitor.o serialize.o General.o signal.o MonitorCommands.o Fifo.o MonitorInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o HashImplementation.o -o monitor

main.o: main.c
	gcc -g -c main.c

MonitorInterface.o: MonitorInterface.c
	gcc -g -c MonitorInterface.c

Interface.o: Interface.c
	gcc -g -c Interface.c

ExtrasImplementation.o: ExtrasImplementation.c
	gcc -g -c ExtrasImplementation.c

bloomfilterImplementation.o: bloomfilterImplementation.c
	gcc -g -c bloomfilterImplementation.c

LinkedList.o: LinkedList.c
	gcc -g -c LinkedList.c

HashImplementation.o: HashImplementation.c
	gcc -g -c HashImplementation.c

Fifo.o: Fifo.c
	gcc -g -c Fifo.c

monitor.o: monitor.c
	gcc -g -c monitor.c

Commands.o: Commands.c
	gcc -g -c Commands.c

MonitorCommands.o: MonitorCommands.c
	gcc -g -c MonitorCommands.c

serialize.o: serialize.c
	gcc -g -c serialize.c

signal.o: signal.c
	gcc -g -c signal.c

General.o: General.c
	gcc -g -c General.c

MCImplementation.o: MCImplementation.o
	gcc -g -c MCImplementation.c

clean:
	rm -f main.o MCImplementation.o General.o Fifo.o signal.o Interface.o MonitorCommands.o Commands.o MonitorInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o serialize.o monitor monitor.o travelMonitor