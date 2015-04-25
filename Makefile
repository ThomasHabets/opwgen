# opwgen/Makefile

all: opwgen
opwgen:
	gcc -Wall opwgen.c opie/btoe.c -o opwgen
clean:
	rm opwgen
check: opwgen
	./opwgen_test
