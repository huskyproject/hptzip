#
# Universal makefile for GNU make for hptzip
# HUSKY project (c) 1998-2003 HUSKY Team.
#
# $Id$
#

include ../../huskymak.cfg
include makefile.inc

TOP=../..

LIBNAME=hptzip
VER_MAJOR=0
VER_MINOR=1
VER_RELEASE=0

LIBS+=husky z

LINKLIBS=$(addprefix -l,$(LIBS))
LIBPATHS=$(addprefix -L,$(LIBDIR))

CFLAGS+= -I$(INCDIR) -I../hptzip
LFLAGS+= $(LIBPATHS) $(LINKLIBS)

SRCDIR=../src
SRCS=hptzip.c zip.c unzip.c
ifeq ($(PLATFORM),win32)
   SRCS+= iowin32.c
else
  SRCS+= ioapi.c
endif

#BINARIES=test
LIBRARIES=hptzip
#OBJS_test=$(call src2obj, $(SRCS) test.c)
OBJS_libhptzip=$(call src2obj, $(SRCS))

include makefile.prg
