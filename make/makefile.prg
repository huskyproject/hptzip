include makefile.inc

.PRECIOUS : $(OBJPATH)/%$(OBJEXT)

-include makefile.tgt

# public targets
all: dirs build_binaries build_libraries rm_makefile_tgt
install: install_libraries install-h

# macroses
BUILTLIBPATH=$(OBJPATH)/lib
BUILTBINPATH=$(OBJPATH)/bin
OBJPATH=$(TOP)/build/$(PLATFORM)

mkshlib=$(BUILTLIBPATH)/$(LIBPREFIX)$(1)$(SHLIBSUFFIX)
mksh3lib=$(BUILTLIBPATH)/$(LIBPREFIX)$(1)$(SHLIBSUFFIX).$(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)
mksh2lib=$(BUILTLIBPATH)/$(LIBPREFIX)$(1)$(SHLIBSUFFIX).$(VER_MAJOR).$(VER_MINOR)
mksh1lib=$(BUILTLIBPATH)/$(LIBPREFIX)$(1)$(SHLIBSUFFIX).$(VER_MAJOR)
mkstlib=$(BUILTLIBPATH)/$(LIBPREFIX)$(1)$(STLIBSUFFIX)
src2obj=$(addprefix $(OBJPATH)/,$(1:.c=$(OBJEXT)))
getobjs=$(OBJS_$(1)) $(call src2obj,$(SRCS_$(1)))

define build_bin
   		@echo -ne "$(1): $(OBJS_$(notdir $(1)))\n\t\t@$(CC) -o $(1) $(LFLAGS) $(OBJS_$(notdir $(1)))" > makefile.tgt
   		@make $(1)
endef

define build_lib_static
   		echo -e "$(call mkstlib,$(1)): $(call getobjs,lib$(1))" > makefile.tgt
		echo -e "\t\t@echo -ne \"archiving $(notdir $(call mkstlib,$(1))) ... \"" >> makefile.tgt
		echo -e "\t\t@$(AR) $(AR_R) $(call mkstlib,$(1)) $(OBJS_lib$(basename $(notdir $(1))))" >> makefile.tgt
		echo -e "\t\t@echo \"done\"" >> makefile.tgt
		make $(call mkstlib,$(1))
endef

define build_lib_shared
   		echo -e "$(call mkshlib,$(1)): $(OBJS_lib$(basename $(notdir $(1))))" > makefile.tgt
		echo -e "\t\t@echo -ne \"linking $(notdir $(call mkshlib,$(1))) ... \"" >> makefile.tgt
		echo -e "\t\t@$(CC) $(SHAREDOPT)$(call mkshlib,$(1)) -o $(call mkshlib,$(1)) $(OBJS_lib$(basename $(notdir $(1))))" >> makefile.tgt
		echo -e "\t\t@echo \"done\"" >> makefile.tgt
		make $(call mkshlib,$(1))
endef

define install_lib
		$(MV) $(call mkshlib,$(1)) $(call mksh3lib,$(1))
		$(LN) $(notdir $(call mksh3lib,$(1))) $(call mksh2lib,$(1))
		$(LN) $(notdir $(call mksh3lib,$(1))) $(call mksh1lib,$(1))
		$(LN) $(notdir $(call mksh3lib,$(1))) $(call mkshlib,$(1))
endef

# targets
build_binaries:
		$(if $(BINARIES),$(foreach BIN, $(addprefix $(BUILTBINPATH)/, $(BINARIES)), $(call build_bin,$(BIN))))

ifeq ($(DYNLIBS),1)
build_libraries:
		$(if $(LIBRARIES),$(foreach LIB, $(LIBRARIES), $(call build_lib_static,$(LIB))))
		$(if $(LIBRARIES),$(foreach LIB, $(LIBRARIES), $(call build_lib_shared,$(LIB))))
else
build_libraries:
		$(if $(LIBRARIES),$(foreach LIB, $(LIBRARIES), $(call build_lib_static,$(LIB))))
endif

install_libraries: build_libraries
		$(if $(LIBRARIES),$(foreach LIB, $(LIBRARIES), $(call install_lib,$(LIB))))
		
		
$(OBJPATH)/%$(OBJEXT): $(SRCDIR)/%.c
	echo -n "compiling $(@F) ... "
	$(CC) -c $(CFLAGS) $< -o $@
	echo done

dirs:
	@$(MKDIR) -p $(OBJPATH)
	@$(MKDIR) -p $(BUILTBINPATH)
	@$(MKDIR) -p $(BUILTLIBPATH)

clean: rm_makefile_tgt
		$(foreach BIN, $(BINARIES), -$(RM) $(OBJS_$(BIN)))
		$(foreach LIB, $(LIBRARIES), -$(RM) $(OBJS_lib$(LIB)))

distclean: clean
		$(foreach BIN, $(addprefix $(BUILTBINPATH)/, $(BINARIES)), -$(RM) $(BIN))
		$(foreach LIB, $(LIBRARIES), -$(RM) $(call mkshlib,$(LIB)))
		$(foreach LIB, $(LIBRARIES), -$(RM) $(call mkstlib,$(LIB)))


FORCE:

install-h-dir: FORCE
	-$(MKDIR) $(MKDIROPT) $(INCDIR)$(DIRSEP)$(_H_DIR)

%.h: FORCE
	-$(INSTALL) $(IIOPT) $(_H_DIR)$@ $(INCDIR)$(DIRSEP)$(_H_DIR)

install-h: install-h-dir $(HEADERS)

makefile.tgt:
		@echo -n > makefile.tgt

rm_makefile_tgt:
	-@$(RM) makefile.tgt
