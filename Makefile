CC=g++
CFLAGS=-std=c++17
DEPS = graph.h gng.h
OBJ = GNG/gng.o main.o graph.o

%.0: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
