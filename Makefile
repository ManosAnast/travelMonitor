all: clean main.o Interface.o TMCommands.o Commands.o serialize.o monitor.o Fifo.o TMInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o
	gcc main.o TMCommands.o Interface.o serialize.o Commands.o Fifo.o TMInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o -o travelMonitor
	gcc monitor.o serialize.o TMCommands.o Commands.o Fifo.o Interface.o TMInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o -o monitor

main.o: main.c
	gcc -g -c main.c

TMInterface.o: TMInterface.c
	gcc -g -c TMInterface.c

Interface.o: Interface.c
	gcc -g -c Interface.c

ExtrasImplementation.o: ExtrasImplementation.c
	gcc -g -c ExtrasImplementation.c

bloomfilterImplementation.o: bloomfilterImplementation.c
	gcc -g -c bloomfilterImplementation.c

LinkedList.o: LinkedList.c
	gcc -g -c LinkedList.c

SkiplistImplementation.o: SkiplistImplementation.c
	gcc -g -c SkiplistImplementation.c

HashImplementation.o: HashImplementation.c
	gcc -g -c HashImplementation.c

Fifo.o: Fifo.c
	gcc -g -c Fifo.c

monitor.o: monitor.c
	gcc -g -c monitor.c

Commands.o: Commands.c
	gcc -g -c Commands.c

TMCommands.o: TMCommands.c
	gcc -g -c TMCommands.c

serialize.o: serialize.o
	gcc -g -c serialize.c

clean:
	rm -f main.o Fifo.o Interface.o TMCommands.o Commands.o TMInterface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o serialize.o monitor monitor.o travelMonitor