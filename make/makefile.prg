.PRECIOUS : $(OBJPATH)/%$(OBJEXT)

all: dirs build_binaries build_libraries

include makefile.tgt

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
		
define build_bin
   		@echo -ne "$(1): $(OBJS_$(notdir $(1)))\n\t\t@$(CC) -o $(1) $(LFLAGS) $(OBJS_$(notdir $(1)))" > makefile.tgt
   		@make $(1)
endef

define build_lib_static
   		@echo -e "$(call mkstlib,$(1)): $(OBJS_$(LIBPREFIX)$(basename $(notdir $(1))))" > makefile.tgt
		@echo -e "\t\t@echo -ne \"archiving $(notdir $(call mkstlib,$(1))) ... \"" >> makefile.tgt
		@echo -e "\t\t@$(AR) $(AR_R) $(call mkstlib,$(1)) $(OBJS_$(LIBPREFIX)$(basename $(notdir $(1))))" >> makefile.tgt
		@echo -e "\t\t@echo \"done\"" >> makefile.tgt
		@make $(call mkstlib,$(1))
endef

define build_lib_shared
   		@echo -e "$(call mkshlib,$(1)): $(OBJS_$(LIBPREFIX)$(basename $(notdir $(1))))" > makefile.tgt
		@echo -e "\t\t@echo -ne \"linking $(notdir $(call mkshlib,$(1))) ... \"" >> makefile.tgt
		@echo -e "\t\t@$(CC) $(SHAREDOPT)$(call mkshlib,$(1)) -o $(call mkshlib,$(1)) $(OBJS_$(LIBPREFIX)$(basename $(notdir $(1))))" >> makefile.tgt
		@echo -e "\t\t@echo \"done\"" >> makefile.tgt
		@make $(call mkshlib,$(1))
endef

$(OBJPATH)/%$(OBJEXT): $(SRCDIR)/%.c
	@echo -n "compiling $(@F) ... "
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo done

dirs:
	@$(MKDIR) -p $(OBJPATH)
	@$(MKDIR) -p $(BUILTBINPATH)
	@$(MKDIR) -p $(BUILTLIBPATH)

clean:
		$(foreach BIN, $(BINARIES), -$(RM) $(OBJS_$(BIN)))
		$(foreach LIB, $(LIBRARIES), -$(RM) $(OBJS_$(LIBPREFIX)$(LIB)))

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

