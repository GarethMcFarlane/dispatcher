CFLAGS= -Wall -std=c99
main: hostd.o pcb.o mab.o
	gcc hostd.o pcb.o mab.o -o dispatch
	rm *.o
hostd.o: hostd.c hostd.h
	gcc -c hostd.c
pcb.o: pcb.c pcb.h
	gcc -c pcb.c
mab.o: mab.c mab.h
	gcc -c mab.c
