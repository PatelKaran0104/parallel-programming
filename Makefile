CC=gcc
CFLAGS=-fopenmp -O2
LDFLAGS=-lgomp
EXE=piWithReduction
SRCS=piWithReduction.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(EXE) $(LDFLAGS)

run:
	./$(EXE)

clean:
	rm -f $(EXE)