CC=gcc
ASM=nasm
LD=ld
OUTDIR=bin/

build: printarray.out priority.out

printarray.out: printarray.c bindir
	${CC} -o ${OUTDIR}printarray.out -std=c11 printarray.c

priority.out: priority.c bindir
	${CC} -g -o ${OUTDIR}priority.out -std=c11 priority.c

bindir:
	-mkdir ${OUTDIR}

clean:
	-rm ${OUTDIR}*.out
	-rm ${OUTDIR}chell
	-rmdir ${OUTDIR}
