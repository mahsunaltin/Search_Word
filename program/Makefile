all: runAll

runAll: psearch1a psearch1b psearch1c psearch2a psearch2b psearch3
	./psearch1a.o name 2 input.txt input2.txt psearch1a.txt
	./psearch1b.o name 2 input.txt input2.txt psearch1b.txt
	./psearch1c.o name 2 input.txt input2.txt psearch1c.txt

psearch1a: psearch1a.c
	gcc psearch1a.c -o psearch1a.o

psearch1b: psearch1b.c
	gcc psearch1b.c -o psearch1b.o

psearch1c: psearch1c.c
	gcc psearch1c.c -o psearch1c.o


psearch2a: psearch2a.c
	gcc psearch2aslave.c -o a
	gcc psearch2a.c -o psearch2a.o
	./psearch2a.o name 2 input.txt input2.txt psearch2a.txt

psearch2b: psearch2b.c
	gcc psearch2bslave.c -o a
	gcc psearch2b.c -o psearch2b.o
	./psearch2b.o name 2 input.txt input2.txt psearch2b.txt

psearch3: psearch3.c
	gcc psearch3.c -o psearch3.o -pthread
	./psearch3.o name 2 input.txt input2.txt psearch3.txt

clean:
	rm -rf *o hello