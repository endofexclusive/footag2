# This file requires GNU Make

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
HERE=$(mkfile_dir)

FOOTDIR=$(HERE)/..
FOOTLIB = dist/libfootag.a

CCOPT            =
CCOPT           += -O2

CFLAGS           =
CFLAGS          += $(CCOPT)
CFLAGS          += -g
CFLAGS          += -std=c99
CFLAGS          += -Wall
CFLAGS          += -Wextra
CFLAGS          += -Wmissing-prototypes
CFLAGS          += -Wstrict-prototypes
CFLAGS          += -Wmissing-declarations
CFLAGS          += -pedantic
CFLAGS          += -Idist/include

LDFLAGS          =
LDFLAGS         += -lm
LDLIBS           =
LDLIBS          += $(FOOTLIB)

PROGS            =
PROGS           += ex

.PHONY: all
all: $(PROGS)

ex: ex.o $(FOOTLIB)
	$(CC) $(LDFLAGS) ex.o $(LDLIBS) -o $@

%.o: %.c $(FOOTLIB)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: $(FOOTLIB)
$(FOOTLIB):
	$(MAKE) -C $(FOOTDIR) OUTDIR=$(HERE)/build INSTALLDIR=$(HERE)/dist install

.PHONY: valgrind
valgrind: ex
	valgrind --leak-check=full ./$<

.PHONY: clean
clean:
	$(MAKE) -C $(FOOTDIR) OUTDIR=$(HERE)/build INSTALLDIR=$(HERE)/dist clean
	$(RM) $(FOOTLIB)
	$(RM) -r dist
	$(RM) *.o
	$(RM) $(PROGS)
	$(RM) -r myproj-out
	$(RM) -r random-out

