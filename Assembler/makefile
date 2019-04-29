assembler: assembler.o AssSym.o misc.o first.o second.o AssData.o
	gcc -g -ansi -pedantic -Wall assembler.o AssSym.o misc.o first.o second.o AssData.o -o assembler
assembler.o: assembler.c assembler.h
	gcc -c -ansi -pedantic -Wall assembler.c -o assembler.o
AssSym.o: AssSym.c AssSym.h
	gcc -c -ansi -pedantic -Wall AssSym.c -o AssSym.o
misc.o: misc.c misc.h
	gcc -c -ansi -pedantic -Wall misc.c -o misc.o
first.o: first.c first.h
	gcc -c -ansi -pedantic -Wall first.c -o first.o
second.o: second.c second.h
	gcc -c -ansi -pedantic -Wall second.c -o second.o
AssData.o: AssData.c AssData.h
	gcc -c -ansi -pedantic -Wall AssData.c -o AssData.o






