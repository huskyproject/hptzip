OSDEFS=osdefs/
include $(OSDEFS)generic

ifeq ($(findstring EMX, $(PATH)), EMX)
  include $(OSDEFS)emx
else
ifneq ($(DJGPP),)
  include $(OSDEFS)djg
else
ifeq ($(TERM),cygwin)
  include $(OSDEFS)cyg
else
ifeq ($(OSTYPE),beos) # BeOS build
  include $(OSDEFS)be
else
ifeq ($(shell uname),Linux)
  include $(OSDEFS)lnx
ifeq ($(shell uname),FreeBSD)
  include $(OSDEFS)bsd
endif  
endif  
endif
endif
endif
endif
