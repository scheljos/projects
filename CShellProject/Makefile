CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

commando : commando.o cmd.o cmdcol.o util.o
	$(CC) -o commando commando.o cmd.o cmdcol.o util.o

commando.o : commando.c commando.h
	$(CC) -c commando.c

cmdcol.o : cmdcol.c commando.h
	$(CC) -c $<

cmd.o : cmd.c commando.h
	$(CC) -c $<

util.o : util.c commando.h
	$(CC) -c $<

clean:
	rm -f commando *.o

include test_Makefile
