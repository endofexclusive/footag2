# This file requires GNU Make

# OPTIONS
LIBNAME          = footag
OUTDIR          ?= build/
INSTALLDIR      ?= dist/
AR		 = ar

# Specifies which level of log messages to compile. Default is everything
# except debug messages. Levels which are specified at compile time can be
# dynamically enabled or disabled at run-time.
# FOOLOG           = -DFOOLOG_MASK=FOOLOG_ALL

ifneq ($(strip $(OUTDIR)),)
  ifneq ($(strip $(OUTDIR)),$(dir $(strip $(OUTDIR))))
    override OUTDIR:=$(OUTDIR)/
  endif
endif

CCOPT            =
CCOPT           += -O2

CPPFLAGS         =
CPPFLAGS        += -MMD

CFLAGS           =
CFLAGS          += -g
CFLAGS          += $(CCOPT)
CFLAGS          += -std=c99
CFLAGS          += -Wall
CFLAGS          += -Wextra
CFLAGS          += -Wmissing-prototypes
CFLAGS          += -Wstrict-prototypes
CFLAGS          += -Wmissing-declarations
CFLAGS          += -Wundef
CFLAGS          += -pedantic
CFLAGS          += -Iinclude
CFLAGS          += -Isrc
CFLAGS          += $(FOOLOG)

LIBFILE = $(OUTDIR)lib$(LIBNAME).a

LIBSRCS  =
LIBSRCS += wiz.c
LIBSRCS += wiz_setref.c
LIBSRCS += hint.c
LIBSRCS += chip.c
LIBSRCS += chiparray.c
LIBSRCS += molded.c
LIBSRCS += capae.c
LIBSRCS += soic.c
LIBSRCS += sod.c
LIBSRCS += soj.c
LIBSRCS += qfp.c
LIBSRCS += son.c
LIBSRCS += qfn.c
LIBSRCS += pson.c
LIBSRCS += pqfn.c
LIBSRCS += bga.c
LIBSRCS += sot223.c
LIBSRCS += sot23.c
LIBSRCS += ipc7351b/ipc7351b.c
LIBSRCS += ipc7351b/table.c
LIBSRCS += ipc7251draft1/ipc7251draft1.c
LIBSRCS += pga.c
LIBSRCS += dip.c
LIBSRCS += sip.c

LIBOBJS := $(LIBSRCS:.c=.o)
LIBOBJS := $(addprefix $(OUTDIR),$(LIBOBJS))

.PHONY: all
all: $(LIBFILE)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)%.o: src/%.c | $(OUTDIR)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIBFILE): $(LIBOBJS)
	$(AR) rv $@ $^

.PHONY: install
install: $(LIBFILE)
	mkdir -p $(INSTALLDIR)
	install $(LIBFILE) $(INSTALLDIR)
	tar cf - include | (cd $(INSTALLDIR) ; tar xvf -)

.PHONY: clean
clean:
	$(RM) $(LIBOBJS)
	$(RM) $(LIBOBJS:%.o=%.d)
	$(RM) $(LIBFILE)
	-rmdir $(OUTDIR)/* 2> /dev/null || true
	-rmdir $(OUTDIR) 2> /dev/null || true

# Dependency files are generated with -MMD compiler option
-include $(LIBOBJS:%.o=%.d)

