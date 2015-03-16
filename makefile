CC=gcc
CFLAGS=-g -ansi -c -Wall -pedantic
LIBS=-lm

vpath %.c src/
vpath %.o obj/
vpath %.h include/

.c.o:
	$(CC) $(CFLAGS) -c -I include/ -o obj/$@ $<

bin/rv: main.o search.o file.o arguments.o global.o tree.o astat.o outdata.o pstat.o dstat.o gstat.o
	$(CC) -o $@ obj/main.o obj/search.o obj/file.o obj/arguments.o obj/global.o obj/tree.o obj/astat.o obj/outdata.o obj/pstat.o obj/dstat.o obj/gstat.o

main.o: main.c

search.o: search.c search.h

file.o: file.c file.h

arguments.o: arguments.c arguments.h

global.o: global.c global.h

tree.o: tree.c tree.h

astat.o: astat.c astat.h

outdata.o: outdata.c outdata.h

pstat.o: pstat.c pstat.h

dstat.o: dstat.c dstat.h

gstat.o: gstat.c gstat.h

clean::
	rm -f obj/*.o bin/rv *~
