CC=clang
OUTDIR=bin
CSTD=c11
FLAGS=-g -Wall -Wextra -std=${CSTD}

.SUFFIXES: .c .h .o .out .so

.c.o:
	${CC} ${FLAGS} -c -o $@ $?

.c.out:
	${CC} ${FLAGS} -o ${OUTDIR}/$@ $?

FILES = usr1counter pipe server client

all: outdir compile

compile: ${FILES:%=%.out}

objects: ${FILES:%=%.o}

outdir:
	mkdir -p bin

clean:
	-rm ${OUTDIR}/*.out
	-rmdir ${OUTDIR}

