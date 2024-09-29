.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-chcase
VERSION   =1.0.0
EXE      ?=$(shell uname -s | awk '/Windows/ || /MSYS/ || /CYG/ { print ".exe" }')
TPREFIX  ?=$(if $(EXE),x86_64-w64-mingw32-)
CC        = $(TPREFIX)gcc -Wall -g3 -std=c99
PROGS     = 
PREFIX    =/usr/local
BUILDDIR ?=.build
PROGRAM   =$(BUILDDIR)/chcase$(EXE)

all: $(PROGRAM)
clean:
	rm -f $(PROGRAM)
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin $(DESTDIR)$(PREFIX)/share/chcase
	cp $(PROGRAM) $(DESTDIR)$(PREFIX)/bin
	cp chcase.dict $(DESTDIR)$(PREFIX)/share/chcase
check:

$(PROGRAM): chcase.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -DPREFIX=\"$(PREFIX)\" -o $@ $^ $(LDFLAGS) $(LIBS)

## -- BLOCK:c --
clean: clean-c
clean-c:
	rm -f *.o
## -- BLOCK:c --
