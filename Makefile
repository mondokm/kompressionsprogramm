all:
	gcc kompression.c trees.c files.c -o komp
	./komp

debug:
	gcc -g kompression.c trees.c files.c -o komp
	gdb komp