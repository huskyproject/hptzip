# hptzip/Makefile
#
# This file is part of hptzip, part of the Husky fidonet software project
# Use with GNU make v.3.82 or later
# Requires: husky enviroment
#

# Object files of the library
# Please sort the list to make checking it by human easy
hptzip_OBJFILES = $(O)hptzip$(_OBJ) $(O)ioapi$(_OBJ) $(O)unzip$(_OBJ) \
                  $(O)zip$(_OBJ)

ifneq ($(findstring MINGW,$(OStype)),)
    hptzip_OBJFILES += $(O)iowin32$(_OBJ)
endif

hptzip_OBJS := $(addprefix $(hptzip_OBJDIR),$(hptzip_OBJFILES))

# Static and dynamic target libraries
hptzip_TARGETLIB := $(L)$(LIBPREFIX)$(hptzip_LIBNAME)$(LIBSUFFIX)$(_LIB)
hptzip_TARGETDLL := $(B)$(DLLPREFIX)$(hptzip_LIBNAME)$(DLLSUFFIX)$(_DLL)

ifeq ($(DYNLIBS), 1)
    ifeq ($(findstring Windows,$(OS)),)
        hptzip_TARGET = $(hptzip_TARGETDLL).$(hptzip_VER)
    else
        hptzip_TARGET = $(hptzip_TARGETDLL)
    endif
else
    hptzip_TARGET = $(hptzip_TARGETLIB)
endif

hptzip_TARGET_BLD = $(hptzip_BUILDDIR)$(hptzip_TARGET)
hptzip_TARGET_DST = $(LIBDIR_DST)$(hptzip_TARGET)

# ZLIBHDIR may be defined in huskymak.cfg.mgw
ifdef ZLIBHDIR
    IZLIBHDIR := -I$(ZLIBHDIR)
endif

hptzip_CDEFS := $(CDEFS)
ifeq ($(DYNLIBS),1)
    ifneq ($(findstring Windows,$(OS)),)
        hptzip_CDEFS += -D_MAKE_DLL
    endif
endif
hptzip_CDEFS += $(IZLIBHDIR) -I$(hptzip_ROOTDIR)$(hptzip_H_DIR) -I$(huskylib_ROOTDIR)

.PHONY: hptzip_build hptzip_install hptzip_install-dynlib \
        hptzip_uninstall hptzip_clean hptzip_distclean hptzip_depend \
        hptzip_rm_OBJS hptzip_rm_BLD hptzip_rm_DEP hptzip_rm_DEPS

hptzip_build: $(hptzip_TARGET_BLD)

ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), distclean)
ifneq ($(MAKECMDGOALS), uninstall)
    include $(hptzip_DEPS)
endif
endif
endif

# Make a hard link of the library in $(hptzip_BUILDDIR)
$(hptzip_TARGET_BLD): $(hptzip_OBJDIR)$(hptzip_TARGET)
	$(LN) $(LNHOPT) $< $(hptzip_BUILDDIR)

# Build the static library
# ZLIBL may be defined in huskymak.cfg.mgw
ifdef ZLIBL
$(hptzip_OBJDIR)$(hptzip_TARGETLIB): $(hptzip_OBJS) $(ZLIBL) | do_not_run_make_as_root
	$(CP) $(ZLIBL) $(hptzip_OBJDIR); cd $(hptzip_OBJDIR); \
    $(AR) $(AR_R) $(hptzip_TARGETLIB) $(^F)
    ifdef RANLIB
		cd $(hptzip_OBJDIR); $(RANLIB) $(hptzip_TARGETLIB)
    endif
else
$(hptzip_OBJDIR)$(hptzip_TARGETLIB): $(hptzip_OBJS) | do_not_run_make_as_root
	cd $(hptzip_OBJDIR); \
	$(AR) $(AR_R) $(hptzip_TARGETLIB) $(^F)
    ifdef RANLIB
		cd $(hptzip_OBJDIR); $(RANLIB) $(hptzip_TARGETLIB)
    endif
endif

# Build the dynamic library
ifeq ($(DYNLIBS),1)
$(hptzip_OBJDIR)$(hptzip_TARGET): $(hptzip_OBJS) | do_not_run_make_as_root
    ifeq ($(filter gcc clang,$(MKSHARED)),)
		$(LD) $(LFLAGS) -o $@ $^ -lz
    else
        ifeq ($(findstring Windows,$(OS)),)
			$(CC) $(LFLAGS) -shared -Wl,-soname,$(hptzip_TARGET) -o $@ $^ -lz
        else
			$(CC) $(LFLAGS) -shared -Wl,-soname,$(hptzip_TARGET) \
			-o $@ $^ $(ZLIBL)
        endif
    endif
endif

# Compile .c files
$(hptzip_OBJS): $(hptzip_OBJDIR)%$(_OBJ): $(hptzip_SRCDIR)%.c | $(hptzip_OBJDIR) do_not_run_make_as_root
	$(CC) $(CFLAGS) $(hptzip_CDEFS) -o $(hptzip_OBJDIR)$*$(_OBJ) $(hptzip_SRCDIR)$*.c

$(hptzip_OBJDIR): | $(hptzip_BUILDDIR) do_not_run_make_as_root
	[ -d $@ ] || $(MKDIR) $(MKDIROPT) $@

# Install
hptzip_install: hptzip_install-dynlib ;

ifneq ($(DYNLIBS), 1)
    hptzip_install-dynlib: ;
else
    ifneq ($(strip $(LDCONFIG)),)
        hptzip_install-dynlib: \
            $(LIBDIR_DST)$(hptzip_TARGETDLL).$(hptzip_VER)
		-@$(LDCONFIG) >& /dev/null || true
    else
        hptzip_install-dynlib: \
            $(LIBDIR_DST)$(hptzip_TARGETDLL).$(hptzip_VER) ;
    endif

    $(LIBDIR_DST)$(hptzip_TARGETDLL).$(hptzip_VER): \
        $(hptzip_BUILDDIR)$(hptzip_TARGETDLL).$(hptzip_VER) | \
        $(DESTDIR)$(LIBDIR)
		$(INSTALL) $(ILOPT) $< $(DESTDIR)$(LIBDIR); \
		cd $(DESTDIR)$(LIBDIR); \
		$(TOUCH) $(hptzip_TARGETDLL).$(hptzip_VER); \
		$(LN) $(LNOPT) $(hptzip_TARGETDLL).$(hptzip_VER) $(hptzip_TARGETDLL).$(hptzip_VERH); \
		$(LN) $(LNOPT) $(hptzip_TARGETDLL).$(hptzip_VER) $(hptzip_TARGETDLL)
endif


# Clean
hptzip_clean: hptzip_rm_OBJS
	-[ -d "$(hptzip_OBJDIR)" ] && $(RMDIR) $(hptzip_OBJDIR) || true

hptzip_rm_OBJS:
	-$(RM) $(RMOPT) $(hptzip_OBJDIR)*


# Distclean
hptzip_distclean: hptzip_clean hptzip_rm_BLD
	-[ -d "$(hptzip_BUILDDIR)" ] && $(RMDIR) $(hptzip_BUILDDIR) || true

hptzip_rm_BLD: hptzip_rm_DEP
	-$(RM) $(RMOPT) $(hptzip_BUILDDIR)$(hptzip_TARGETLIB)
	-$(RM) $(RMOPT) $(hptzip_BUILDDIR)$(hptzip_TARGETDLL)*

hptzip_rm_DEP: hptzip_rm_DEPS
	-[ -d "$(hptzip_DEPDIR)" ] && $(RMDIR) $(hptzip_DEPDIR) || true

hptzip_rm_DEPS:
	-$(RM) $(RMOPT) $(hptzip_DEPDIR)*


# Uninstall
ifeq ($(DYNLIBS), 1)
    hptzip_uninstall:
		-$(RM) $(RMOPT) $(DESTDIR)$(LIBDIR)$(DIRSEP)$(hptzip_TARGETDLL)*
else
    hptzip_uninstall: ;
endif
