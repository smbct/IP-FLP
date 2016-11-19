.PHONY: clean
CC= gcc
CFLAGS= -std=c99 -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -O3 #-pg

rapport:
	pdflatex Rapport/rapport
