all:
	gcc kompression.c utils/trees.c utils/files.c -o komp
	./komp

debug:
	gcc -g kompression.c utils/trees.c utils/files.c -o komp
	gdb komp

install:
	cp komp /usr/bin/komp