CC=clang

build: spock spock32 hello_asm isfile stat

spock:
	${CC} -g -o spock.out -Wno-deprecated-declarations spock.c

spock32:
	${CC} -g -m32 -o spock32.out -Wno-deprecated-declarations spock.c

hello_asm:
	${CC} -g -s -o hello.out hello.s

isfile:
	${CC} -m32 -g -s -o isfile.out isfile.s

stat:
	${CC} -g -std=gnu11 -o stat.out stat.c
#	gcc -g -o stat.out libstat.o stat.c

clean:
	rm *.out
