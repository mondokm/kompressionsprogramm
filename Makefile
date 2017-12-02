all:
	gcc -c src/utils/print_state.s -o src/utils/print_state.o
	gcc -c src/utils/read_from_file.s -o src/utils/read_from_file.o
	gcc src/kompression.c src/utils/huffman.c src/utils/files.c src/utils/print_state.o src/utils/read_from_file.s -o bin/komp -lgmp
	./bin/komp

debug:
	gcc -c src/utils/print_state.s -o src/utils/print_state.o -g
	gcc -c src/utils/read_from_file.s -o src/utils/read_from_file.o -g
	gcc -g src/kompression.c src/utils/huffman.c src/utils/files.c src/utils/print_state.o src/utils/read_from_file.s -o bin/komp -lgmp
	gdb bin/komp

install:
	cp bin/komp /usr/bin/komp