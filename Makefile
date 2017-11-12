all:
	gcc src/kompression.c src/utils/trees.c src/utils/files.c -o bin/komp
	./bin/komp

debug:
	gcc -g src/kompression.c src/utils/trees.c src/utils/files.c -o bin/komp
	gdb bin/komp

install:
	cp bin/komp /usr/bin/komp