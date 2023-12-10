# Generated automatically from Makefile.in by configure.
#
#   This file is part of CONCORDE
#
#   (c) Copyright 1995--1999 by David Applegate, Robert Bixby,
#   Vasek Chvatal, and William Cook
#
#   Permission is granted for academic research use.  For other uses,
#   contact the authors for licensing options.
#
#   Use at your own risk.  We make no guarantees about the
#   correctness or usefulness of this code.
#
CPLEXDIR      = /Applications/CPLEX_Studio129/cplex/include/ilcplex

SHELL = /bin/sh
SRCROOT = ..
BLDROOT = ..
CCINCDIR=$(SRCROOT)/INCLUDE

srcdir = .

CC = gcc
CFLAGS = -g -O2  -I$(BLDROOT)/INCLUDE -I$(CCINCDIR) -I$(CPLEXDIR)
LDFLAGS = -g -O2
LIBS = -lm
RANLIB = ranlib
LPSOLVER_INTERFACE = lpcplex8.c

OBJ_SUFFIX = o
o = $(OBJ_SUFFIX)

THISLIB=lp.a

LIBSRCS=$(LPSOLVER_INTERFACE)

ALLSRCS=lpcplex8.c

all: $(THISLIB)

everything: all

clean:
	-rm -f *.$o $(THISLIB)

OBJS=$(LIBSRCS:.c=.o)

$(THISLIB): $(OBJS)
	$(AR) $(ARFLAGS) $(THISLIB) $(OBJS)
	$(RANLIB) $(THISLIB)

.PHONY: $(BLDROOT)/concorde.a
$(BLDROOT)/concorde.a: $(OBJS)
	$(AR) $(ARFLAGS) $(BLDROOT)/concorde.a $(OBJS)
	$(RANLIB) $(BLDROOT)/concorde.a

include ../INCLUDE/Makefile.common

# DO NOT DELETE THIS LINE -- make depend depends on it.

I=$(CCINCDIR)
I2=$(BLDROOT)/INCLUDE

# lp_none.$o:  lp_none.c  $(I)/machdefs.h $(I2)/config.h  $(I)/util.h     \
#         $(I)/macrorus.h $(I)/lp.h       
# lpcplex4.$o: lpcplex4.c $(I)/machdefs.h $(I2)/config.h  $(I)/util.h     \
#         $(I)/macrorus.h $(I)/lp.h       
# lpcplex5.$o: lpcplex5.c $(I)/machdefs.h $(I2)/config.h  $(I)/util.h     \
#         $(I)/macrorus.h $(I)/lp.h       
lpcplex8.$o: lpcplex8.c $(I)/machdefs.h $(I2)/config.h  $(I)/util.h     \
        $(I)/macrorus.h $(I)/lp.h       
