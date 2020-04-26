CC?=gcc
CFLAGS?=-g
CFLAGS+=-std=c11 -Wall
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

DESTBINDIR = $(DESTDIR)$(PREFIX)/bin
install: all
	if [ ! -d $(DESTDIR)$(MANDIR)/man1 ];then	\
		mkdir -p $(DESTDIR)$(MANDIR)/man1;	\
	fi
	install -m644 ylva.1 $(DESTDIR)$(MANDIR)/man1/
	gzip -f $(DESTDIR)$(MANDIR)/man1/ylva.1
	if [ ! -d $(DESTBINDIR) ] ; then \
		mkdir -p $(DESTBINDIR) ; \
	fi
	install -m755 ylva $(DESTBINDIR)/

uninstall:
	rm $(PREFIX)/bin/ylva
	rm $(DESTDIR)$(MANDIR)/man1/ylva.1.gz
