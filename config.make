###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

################################## VARIABLES ##################################
# Operational System
ARCH     :=Linux
# Directories
SRCDIR   :=src
HDRDIR   :=include
OBJDIR   :=build
BINDIR   :=bin
TESTDIR  :=tests
DEPDIR   :=.deps
MAKEDIR  :=$(BINDIR) $(SRCDIR) $(HDRDIR)
# Compiler Flags
LDLIBS   :=`pkg-config --libs gtk+-3.0` -static-libgcc -static-libstdc++\
 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
LDLIBS   :=`pkg-config --libs gtk+-3.0`
LDFLAGS  :=
CXXFLAGS :=-std=c++14 -Wall `pkg-config --cflags --libs gtk+-3.0` 
INCLUDE  :=-I$(HDRDIR)
# Executable & main files
ifeq ($(ARCH), Windows)
    EXT  :=.exe
endif
EXEC     :=$(BINDIR)/execute_me$(EXT)
MAIN     :=main
