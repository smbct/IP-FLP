.PHONY: clean
CC= gcc
CFLAGS= -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 #-pg

rapport:
	pdflatex Rapport/rapport

main: src/main.c src/probleme.o
	$(CC) $(CFLAGS) src/main.c src/probleme.o -o main

src/probleme.o: src/probleme.c
	$(CC) $(CFLAGS) src/probleme.c -c -o src/probleme.o
