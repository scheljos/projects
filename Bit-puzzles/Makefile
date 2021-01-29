#
# Makefile that builds btest and other helper programs for the CS:APP data lab
# 
CC = gcc
CFLAGS = -O -Wall
C89FLAGS = -std=c89 -pedantic -pedantic-errors
LIBS = -lm

all: btest fshow ishow

bits.o: bits.c
	$(CC) $(CFLAGS) $(C89FLAGS) -c bits.c

btest: btest.c bits.o decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) -o btest bits.o btest.c decl.c tests.c $(LIBS)

fshow: fshow.c
	$(CC) $(CFLAGS) -o fshow fshow.c

ishow: ishow.c
	$(CC) $(CFLAGS) -o ishow ishow.c

# Forces a recompile. Used by the driver program. 
btestexplicit:
	$(CC) $(CFLAGS) $(C89FLAGS) -c bits.c
	$(CC) $(CFLAGS) $(LIBS) -o btest bits.o btest.c decl.c tests.c

clean:
	rm -f *.o btest fshow ishow *~


