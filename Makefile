PROGS := mcdcli mcdui

# Comment these out for a non-debugging build
#CFLAGS := \
	-g \
	-Wall \
#	-ansi \
	-pedantic-errors

LDFLAGS := -ldb

# Uncomment these for a non-debug build
CFLAGS := \
	-ggdb \
        -Wall \
        -pedantic-errors \
	-O2 

#LDFLAGS := \
	-I. \
	-ldb \
        -lefence

all : $(PROGS)

.PHONY : clean dist zip

mcdcli : mcdcli.o mcddb.o mcdutil.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

mcdui : mcdui.o mcdscr.o mcddb.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -lcurses

mcddb.o : mcddb.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS)

mcdutil.o : mcdutil.c
	$(CC) -c $(CFLAGS) $< -o $@

mcdscr.o : mcdscr.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	$(RM) $(PROGS) *.o *~ core mcd.db

dist : clean
	tar czf mcd-0.1.tar.gz *.c *.h Makefile

zip : clean
	zip 215122cd.zip *.c *.h Makefile
