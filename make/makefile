#
# Universal makefile for GNU make for hptzip
# HUSKY project (c) 1998-2003 HUSKY Team.
#
# $Id$
#

include makefile.inc

MODULENAME=hptzip
VER_MAJOR=0
VER_MINOR=1
VER_RELEASE=0

CFLAGS+= -I../hptzip -I$(INCDIR)
LFLAGS+= -L../../build/$(PLATFORM)/lib -I$(LIBDIR)

SRCS_libhptzip=hptzip.c zip.c unzip.c
ifeq ($(PLATFORM),win32)
   SRCS_libhptzip+= iowin32.c
else
   SRCS_libhptzip+= ioapi.c
endif
OBJS_libhptzip=$(call src2obj, $(SRCS_libhptzip))
LIBS_libhptzip=z husky
CFLAGS_libhptzip=
LFLAGS_libhptzip=

LIBRARIES=hptzip
BINARIES=
HEADERS=

include makefile.prg
