CC?=gcc
CFLAGS?=-g
CFLAGS+=-std=c99 -Wall
PREFIX?=/usr/
MANDIR?=$(PREFIX)/share/man
LIBS=-lcrypto -lsqlite3 -lrt
PROG=ylva
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
HEADERS=$(wildcard *.h)

all: $(PROG)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f $(PROG)

install: all
	if [ ! -d $(DESTDIR)$(MANDIR)/man1 ];then	\
		mkdir -p $(DESTDIR)$(MANDIR)/man1;	\
	fi
	cp ylva.1 $(DESTDIR)$(MANDIR)/man1/
	gzip -f $(DESTDIR)$(MANDIR)/man1/ylva.1
	cp ylva $(DESTDIR)$(PREFIX)/bin/

uninstall:
	rm $(PREFIX)/bin/ylva
	rm $(DESTDIR)$(MANDIR)/man1/ylva.1.gz
