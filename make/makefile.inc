# $Id$
#
# Common makefiles part (include-file) for hptzip

# Use defines before including this file:
# _OBJ	object files suffix begins with point (".o", ".obj", ... )
# _LIB	library files suffix begins with point (".a", ".lib", ... )
# _DLL	dinamic linking library files suffix begins with point (".dll", ".so", ...)
# LIBPREFIX  library filename prefix ("lib", "", ...)
# LIBSUFFIX  library filename suffix (compiler ID, three characters, somewhere empty)
# DLLPREFIX  dinamic linking library prefix (usually empty)
# DLLSUFFIX  dinamic linking library suffix (compiler ID or empty)
# O, B, L, H prefixes to use with names of objects, binaries, libraries and headers respectively

# Library name
LIBNAME = hptzip

# Version
VERMAJOR= 0
VERMINOR= 1
VERPATCH= 0
VERH	= $(VERMAJOR).$(VERMINOR)
VER	= $(VERH).$(VERPATCH)

# Sources directory
_SRC_DIR= ../src/
# Headers directory
H_DIR	= ../$(LIBNAME)/

# Header files (please sort list to easy check by human)
HEADERS	= $(H)hptzip.h $(H)hptzipdl.h $(H)ioapi.h $(H)iowin32.h $(H)unzip.h $(H)crypt.h

# Object files: library (please sort list to easy check by human)
OBJS    = $(O)hptzip$(_OBJ) $(O)ioapi$(_OBJ) $(O)iowin32$(_OBJ) $(O)unzip$(_OBJ) $(O)zip$(_OBJ)

# Object files: programs
#PRGOBJS	= $(O)test(_OBJ)

# Binary file(s) to build from sources
#PROGRAMS= $(B)test$(_EXE)

# Usually compose from LIBPREFIX, LIBSUFFIX and LIB.
TARGETLIB = $(L)$(LIBPREFIX)$(LIBNAME)$(LIBSUFFIX)$(_LIB)
TARGETDLL = $(B)$(DLLPREFIX)$(LIBNAME)$(DLLSUFFIX)$(_DLL)

# List of HUSKY libraries required to build binary file(s)
LIBS	=
