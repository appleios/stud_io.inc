all: run
	
test.o: test.asm
	nasm -f elf32 test.asm

test: test.o
	ld -m elf_i386 test.o -o test

run: test
	./test
	
