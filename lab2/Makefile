CC=gcc
ASM=nasm
LD=ld
OUTDIR=bin/

build: printarray.out priority.out chell

printarray.out: printarray.c bindir
	${CC} -o ${OUTDIR}printarray.out -std=c11 printarray.c

priority.out: priority.c bindir
	${CC} -g -o ${OUTDIR}priority.out -std=c11 priority.c

chell: chell.c
	${CC} -g -o ${OUTDIR}chell -std=c11 chell.c

bindir:
	-mkdir ${OUTDIR}

clean:
	-rm ${OUTDIR}*.out
	-rm ${OUTDIR}chell
	-rmdir ${OUTDIR}
