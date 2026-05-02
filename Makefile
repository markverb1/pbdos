
CC     = wcc
LINK   = wlink
CFLAGS = -0 -w4 -e25 -zq -od -d2 -bt=dos
INC    = -i="$(WATCOM)\h"
BUILDDIR = build
OBJS   = $(BUILDDIR)/beep.obj $(BUILDDIR)/main.obj $(BUILDDIR)/pbutil.obj
TARGET = $(BUILDDIR)/pbdos.exe

$(TARGET) : $(OBJS)
	@if not exist "$(BUILDDIR)" mkdir "$(BUILDDIR)"
	$(LINK) name $(BUILDDIR)/pbdos sys dos d all op maxe=25,q,symf option map=$(BUILDDIR)/pbdos.map FIL {$(OBJS)}

$(BUILDDIR)/beep.obj : src\beep.c
	$(CC) src\beep.c $(CFLAGS) $(INC) -fo=$(BUILDDIR)/beep.obj

$(BUILDDIR)/main.obj : src\main.c
	$(CC) src\main.c $(CFLAGS) $(INC) -fo=$(BUILDDIR)/main.obj

$(BUILDDIR)/pbutil.obj : src\pbutil.c
	$(CC) src\pbutil.c $(CFLAGS) $(INC) -fo=$(BUILDDIR)/pbutil.obj


clean : .SYMBOLIC
	del /Q "$(BUILDDIR)"\*.obj "$(BUILDDIR)"\*.exe "$(BUILDDIR)"\*.map "$(BUILDDIR)"\*.sym
    