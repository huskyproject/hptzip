# Generic Makefile for hptzip

ifeq ($(DEBIAN), 1)
# Every Debian-Source-Paket has one included.
include /usr/share/husky/huskymak.cfg
else
include ../huskymak.cfg
endif

LIBNAME = hptzip
VER_MAJOR=0
VER_MINOR=1
VER_RELEASE=0
HEADERS = $(LIBNAME)$(DIRSEP)hptzip.h

ifeq ($(DEBUG), 1)

  CFLAGS = $(DEBCFLAGS) -I$(LIBNAME) -I$(INCDIR) $(WARNFLAGS)
  LFLAGS = $(DEBLFLAGS)
else
  CFLAGS = $(OPTCFLAGS) -I$(LIBNAME) -I$(INCDIR) $(WARNFLAGS)
  LFLAGS = $(OPTLFLAGS)
endif

LIBS  = -L$(LIBDIR)
SHAREDOPT= -shared -Wl
LIBPREFIX=lib
TARGETLIB = $(LIBPREFIX)$(LIBNAME)$(_LIB)
TARGETDLL = $(LIBPREFIX)$(LIBNAME)$(_DLL)

CDEFS=-D$(OSTYPE) -DUNAME=\"$(UNAME)\" $(ADDCDEFS)

OBJS    = hptzip$(_OBJ) ioapi$(_OBJ) unzip$(_OBJ) zip$(_OBJ)
SRC_DIR = src/
DESTDIR=1111

default: all

$(TARGETDLL): $(OBJS)
	$(CC) $(SHAREDOPT) $(LFLAGS) $(OBJS) $(LIBS) -o $(TARGETDLL)

$(TARGETLIB): $(OBJS)
	$(AR) $(AR_R) $(TARGETLIB) $(OBJS)

%$(_OBJ): $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(SRC_DIR)$*.c

clean:
	-$(RM) $(RMOPT) *$(_OBJ)
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) core

distclean: clean
	-$(RM) $(RMOPT) $(TARGETDLL)
	-$(RM) $(RMOPT) $(TARGETLIB)

all: $(TARGETDLL) $(TARGETLIB)

install: all
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(LIBDIR)
	$(INSTALL) $(IBOPT) $(TARGETLIB) $(DESTDIR)$(LIBDIR)
	$(INSTALL) $(IBOPT) $(TARGETDLL) $(DESTDIR)$(LIBDIR)
	$(LN) $(LNOPT) $(TARGETDLL) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(TARGETDLL).$(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)
	$(LN) $(LNOPT) $(TARGETDLL) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(TARGETDLL).$(VER_MAJOR).$(VER_MINOR)
	$(LN) $(LNOPT) $(TARGETDLL) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(TARGETDLL).$(VER_MAJOR)
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(INCDIR)$(DIRSEP)$(LIBNAME)
	$(CP) $(HEADERS) $(DESTDIR)$(INCDIR)$(DIRSEP)$(LIBNAME)

uninstall:
	$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(TARGETDLL)
	$(RM) $(RMOPT) $(LIBDIR)$(DIRSEP)$(TARGETLIB)
