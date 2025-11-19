CC=gcc
CFLAGS=-fopenmp -O2
LDFLAGS=-lgomp
EXE=blockpi
SRCS=blockpi.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) $(LDFLAGS)

run:
	./$(EXE)

clean:
	rm -f $(EXE)