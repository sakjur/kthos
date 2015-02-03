CC=gcc
ASM=nasm
LD=ld
OUTDIR=bin/
LIBDIR=libs/

build: spock.out spock32.out hello.out isfile.out isfile_adv.out stat.out \
hello_nasm.out stat_lib.out

spock.out: spock.c bindir
	${CC} -g -o ${OUTDIR}spock.out -Wno-deprecated-declarations spock.c

spock32.out: spock.c bindir
	${CC} -g -m32 -o ${OUTDIR}spock32.out -Wno-deprecated-declarations spock.c

hello.out: hello.s bindir
	${CC} -g -s -o ${OUTDIR}hello.out hello.s

hello_nasm.out: hello_nasm.s bindir 
	${ASM} -f elf64 -o hello_nasm.o hello_nasm.s
	${LD} -s hello_nasm.o -o ${OUTDIR}hello_nasm.out
	rm hello_nasm.o

isfile.out: isfile.s bindir
	${CC} -m32 -g -s -o ${OUTDIR}isfile.out isfile.s

isfile_adv.out: isfile_adv.s bindir
	${ASM} -f elf64 -o isfile_adv.o isfile_adv.s
	${LD} isfile_adv.o -o ${OUTDIR}isfile_adv.out -lc \
		--dynamic-linker /usr/lib64/ld-2.20.so
	rm isfile_adv.o
	
stat.out: stat.c bindir
	${CC} -g -std=gnu11 -o ${OUTDIR}stat.out stat.c

stat_lib.out: stat_lib.c bindir
	${CC} -g -std=gnu11 -o ${OUTDIR}stat_lib.out stat_lib.c ${LIBDIR}libstat.o

libstat.o: libdir
	-wget http://cloud7.sics.se/is1350/libstat-64.o -O ${LIBDIR}libstat.o

libdir:
	-mkdir ${LIBDIR}

bindir:
	-mkdir ${OUTDIR}

clean:
	-rm ${OUTDIR}*.out
	-rmdir ${OUTDIR}
