CC=gcc
CFLAGS=-g -ansi -c -Wall -pedantic
LIBS=-lm

vpath %.c src/
vpath %.o obj/
vpath %.h include/

.c.o:
	$(CC) $(CFLAGS) -c -I include/ -o obj/$@ $<

bin/rv: main.o search.o file.o arguments.o global.o tree.o
	$(CC) -o $@ obj/main.o obj/search.o obj/file.o obj/arguments.o obj/global.o obj/tree.o

main.o: main.c

search.o: search.c search.h

file.o: file.c file.h

arguments.o: arguments.c arguments.h

global.o: global.c global.h

tree.o: tree.c tree.h

clean::
	rm -f obj/*.o bin/rv *~
