CC = gcc
SOURCES = tri.c probleme.c solution.c tabu.c bornes.c listeAffectation.c solver.c aco.c
CFLAGS = -Wall -O3 -std=c99
LDLIBS = -lpthread -lglpk -lm
.PHONY = clean

all: main

main: $(SOURCES:.c=.o)

clean: 
	rm ./*.o