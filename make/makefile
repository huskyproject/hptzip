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
ifeq ($(PLATFORM),win32)
  SRCS=hptzip.c zip.c unzip.c iowin32.c
else
  SRCS=hptzip.c zip.c unzip.c ioapi.c
endif

TARGET=$(call mkshlib,$(LIBNAME)) $(call mkstlib,$(LIBNAME))
all: dirs $(TARGET)

include makefile.prg
