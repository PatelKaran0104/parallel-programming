CC=gcc
CFLAGS=-fopenmp -O2 -lgomp
EXE=pi
SRCS=pi.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE).exe

run:
	.\$(EXE).exe

clean:
	-@del $(EXE).exe 2>nul || echo File not found