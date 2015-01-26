CC=gcc
ASM=nasm

build: spock.out spock32.out hello.out isfile.out isfile_adv.out stat.out

spock.out: spock.c
	${CC} -g -o spock.out -Wno-deprecated-declarations spock.c

spock32.out: spock.c
	${CC} -g -m32 -o spock32.out -Wno-deprecated-declarations spock.c

hello.out: hello.s 
	${CC} -g -s -o hello.out hello.s

isfile.out: isfile.s
	${CC} -m32 -g -s -o isfile.out isfile.s

isfile_adv.out: isfile_adv.s
	${ASM} -f elf64 -o isfile_adv.o isfile_adv.s
	ld -s isfile_adv.o -o isfile_adv.out
	rm isfile_adv.o
	
stat.out: stat.c
	${CC} -g -std=gnu11 -o stat.out stat.c

clean:
	rm *.out
