all:
	gcc opwgen.c -lopie -o opwgen
test:
	dd if=/dev/random bs=8 count=3 | ./opwgen
clean:
	rm opwgen
