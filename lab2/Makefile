CC=gcc
ASM=nasm
LD=ld
OUTDIR=bin/

build: printarray.out

printarray.out: printarray.c bindir
	${CC} -o ${OUTDIR}printarray.out -std=c11 printarray.c

bindir:
	-mkdir ${OUTDIR}

clean:
	-rm ${OUTDIR}*.out
	-rm ${OUTDIR}chell
	-rmdir ${OUTDIR}
