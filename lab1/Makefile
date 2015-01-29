CC=gcc
ASM=nasm
LD=ld

build: spock.out spock32.out hello.out isfile.out isfile_adv.out stat.out \
hello_nasm.out

spock.out: spock.c
	${CC} -g -o spock.out -Wno-deprecated-declarations spock.c

spock32.out: spock.c
	${CC} -g -m32 -o spock32.out -Wno-deprecated-declarations spock.c

hello.out: hello.s 
	${CC} -g -s -o hello.out hello.s

hello_nasm.out: hello_nasm.s 
	${ASM} -f elf64 -o hello_nasm.o hello_nasm.s
	${LD} -s hello_nasm.o -o hello_nasm.out
	rm hello_nasm.o

isfile.out: isfile.s
	${CC} -m32 -g -s -o isfile.out isfile.s

isfile_adv.out: isfile_adv.s
	${ASM} -f elf64 -o isfile_adv.o isfile_adv.s
	${LD} isfile_adv.o -o isfile_adv.out -lc \
		--dynamic-linker /usr/lib64/ld-2.20.so
	rm isfile_adv.o
	
stat.out: stat.c
	${CC} -g -std=gnu11 -o stat.out stat.c

clean:
	rm *.out
