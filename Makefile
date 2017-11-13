all:
	gcc -c src/utils/print_state.s -o src/utils/print_state.o
	gcc src/kompression.c src/utils/trees.c src/utils/files.c src/utils/print_state.o -o bin/komp
	./bin/komp

debug:
	gcc -c src/utils/print_state.s -o src/utils/print_state.o -g
	gcc -g src/kompression.c src/utils/trees.c src/utils/files.c src/utils/print_state.o -o bin/komp
	gdb bin/komp

install:
	cp bin/komp /usr/bin/komp