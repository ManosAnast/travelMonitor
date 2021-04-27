all: clean main.o monitor.o Fifo.o Interface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o
	gcc main.o Fifo.o Interface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o -o travelMonitor
	gcc monitor.o Fifo.o Interface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o -o monitor

main.o: main.c
	gcc -g -c main.c

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

clean:
	rm -f main.o Fifo.o Interface.o ExtrasImplementation.o bloomfilterImplementation.o LinkedList.o SkiplistImplementation.o HashImplementation.o monitor monitor.o travelMonitor