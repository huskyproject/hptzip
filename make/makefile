#
# Universal makefile for GNU make for hptzip
# HUSKY project (c) 1998-2003 HUSKY Team.
#
# $Id$
#

include osdep.inc

TOP=../..

# this is ugly, but don't know now how to make correctly. 
# possibly use huskymak.cfg?
# PLEASE DEFINE CORRECT PATHS HERE NOW !!!
INCDIR+=../../huskylib
LIBDIR+=../../huskylib
# end ugly

CFLAGS+= -I../hptzip -I$(INCDIR)
LFLAGS+= -L$(LIBDIR) -lhusky

SRCDIR=../src
ifeq ($(PLATFORM),win32)
  SRCS=hptzip.c zip.c unzip.c iowin32.c
else
  SRCS=hptzip.c zip.c unzip.c ioapi.c
endif
LIBNAME=hptzip
VER_MAJOR=0
VER_MINOR=1
VER_RELEASE=0

OBJPATH=$(TOP)/build/$(PLATFORM)
TARGET=$(OBJPATH)/$(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX).$(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)
OBJS=$(addprefix $(OBJPATH)/,$(SRCS:.c=.o))

all: dirs $(TARGET)

$(OBJPATH)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(SHAREDOPT)$(TARGET) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(TARGET)

dirs:
	@mkdir -p $(OBJPATH)
