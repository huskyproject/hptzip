.PHONY: dirs all clean distclean

$(OBJPATH)/%$(OBJEXT): $(SRCDIR)/%.c dirs
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJPATH)/%$(SHLIBSUFFIX): $(OBJS)
	$(CC) $(SHAREDOPT)$@ -o $@ $(OBJS) $(LFLAGS)

$(OBJPATH)/%$(STLIBSUFFIX): $(OBJS)
	$(AR) $(AR_R) $@ $(OBJS)

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

