all:
	mkdir -p bin
	gcc -c src/utils/print_state.s -o src/utils/print_state.o
	gcc src/kompression.c src/utils/huffman.c src/utils/files.c src/utils/print_state.o -o bin/komp -lgmp

debug:
	gcc -c src/utils/print_state.s -o src/utils/print_state.o -g
	gcc -c src/utils/read_from_file.s -o src/utils/read_from_file.o -g
	gcc -g src/kompression.c src/utils/huffman.c src/utils/files.c src/utils/print_state.o -o bin/komp -lgmp
	gdb bin/komp

install:
	make
	cp bin/komp /usr/bin/komp

windows:
	gcc src/kompression.c src/utils/huffman.c src/utils/files.c -o bin/komp -lgmp