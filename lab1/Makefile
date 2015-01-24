build: spock spock32 hello_asm isfile stat

spock:
	gcc -g -o spock.out spock.c

spock32:
	gcc -g -m32 -o spock32.out spock.c

hello_asm:
	gcc -g -s -o hello.out hello.s

isfile:
	gcc -m32 -g -s -o isfile.out isfile.s

stat:
	gcc -g -o stat.out libstat.o stat.c

clean:
	rm *.out
