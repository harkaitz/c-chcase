.POSIX:
.SUFFIXES:
.PHONY: all clean install check

PROJECT   =c-chcase
VERSION   =1.0.0
EXE       =$(HOMEDRIVE:C:=.exe)
TPREFIX   =$(HOMEDRIVE:C:=x86_64-w64-mingw32-)
CC        =$(TPREFIX)cc
CFLAGS    =-Wall -g3 -std=c99

PREFIX    =/usr/local
DESTDIR   =$(HOMEDRIVE)

BUILDDIR ?=.build
PROGRAM   =$(BUILDDIR)/chcase$(EXE)
SOURCES   =chcase.c

all: $(PROGRAM)
clean:
	rm -f $(PROGRAM)
install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install -d $(DESTDIR)$(PREFIX)/share/chcase
	install -c -m 755 $(PROGRAM) $(DESTDIR)$(PREFIX)/bin
	install -c -m 655 chcase.dict $(DESTDIR)$(PREFIX)/share/chcase
check:

$(PROGRAM): $(SOURCES)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -DPREFIX=\"$(HOMEDRIVE)$(PREFIX)\" -o $@ $(SOURCES) $(LDFLAGS) $(LIBS)

## -- BLOCK:c --
clean: clean-c
clean-c:
	rm -f *.o
## -- BLOCK:c --
