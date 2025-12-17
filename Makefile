CC=gcc
CFLAGS=-fopenmp -O2
LDFLAGS=-lgomp
EXE=sum_taskloop
SRCS=sum_taskloop.c
ARGS=3 4 42 0 10 2 1 1 1

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) $(LDFLAGS)

run:
	./$(EXE) $(ARGS)

clean:
	rm -f $(EXE)