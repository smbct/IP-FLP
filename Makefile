.PHONY: clean
CC= gcc
CFLAGS= -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 -Ofast #-pg

OBJECTS_FILES= src/probleme.o src/solution.o src/bornes.o src/tri.o src/solver.o src/listeAffectation.o src/aco.o src/tabu.o

rapport:
	pdflatex Rapport/rapport

main: src/main.c $(OBJECTS_FILES)
	$(CC) $(CFLAGS) src/main.c $(OBJECTS_FILES) -lglpk -o main

solverGlpk: src/solverGlpk.c $(OBJECTS_FILES)
	$(CC) $(CFLAGS) src/solverGlpk.c $(OBJECTS_FILES) -lglpk -o solverGlpk

# Compilation des fichiers object
%.o: %.c %.h
	$(CC) $(CFLAGS) $< -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	@rm -rf src/*.o solverGlpk main
