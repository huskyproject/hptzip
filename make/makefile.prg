.PRECIOUS : $(OBJPATH)/%$(OBJEXT)
 
$(OBJPATH)/%$(OBJEXT): $(SRCDIR)/%.c
	@echo -n "building $(@F) ... "
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo done

$(OBJPATH)/%$(SHLIBSUFFIX): $(OBJS)
	@echo -n "linking $(@F) ... "
	@$(CC) $(SHAREDOPT)$(@F) $? $(LFLAGS) -o $@
	@echo done

$(OBJPATH)/%$(STLIBSUFFIX): $(OBJS)
	@echo -n "archiving $(@F) ... "
	@$(AR) $(AR_R) $@ $(OBJS)
	@echo done


dirs:
	@$(MKDIR) -p $(OBJPATH)
	@$(MKDIR) -p $(BUILTBINPATH)
	@$(MKDIR) -p $(BUILTLIBPATH)

clean:
	-@$(RM) $(OBJS)

distclean: clean
	-@$(RM) $(TARGET)

FORCE:

install-h-dir: FORCE
	-$(MKDIR) $(MKDIROPT) $(INCDIR)$(DIRSEP)$(_H_DIR)

%.h: FORCE
	-$(INSTALL) $(IIOPT) $(_H_DIR)$@ $(INCDIR)$(DIRSEP)$(_H_DIR)

install-h: install-h-dir $(HEADERS)

