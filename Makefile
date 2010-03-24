# opwgen/Makefile

all:
	gcc -Wall opwgen.c opie/btoe.c -o opwgen
clean:
	rm opwgen
