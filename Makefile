###############################################################################
######################## Copyright 2016 Marleson Graf #########################
############################ <aszdrick@gmail.com> #############################
###############################################################################

################################## VARIABLES ##################################
# Operational System
ARCH     :=$(shell uname)
# Directories
SRCDIR   :=src
HDRDIR   :=include
OBJDIR   :=build
BINDIR   :=bin
TESTDIR  :=tests
DEPDIR   :=.deps
MAKEDIR  :=$(BINDIR) $(SRCDIR) $(HDRDIR)

ifeq ($(ARCH), windows)
CXX      :=x86_64-w64-mingw32-g++
LDLIBS   :=`pkg-config --libs gtk+-3.0` -static-libgcc -static-libstdc++\
 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
EXEC     :=$(BINDIR)/execute_me.exe
else
LDLIBS   :=`pkg-config --libs gtk+-3.0`
EXEC     :=$(BINDIR)/execute_me
endif

LDFLAGS  :=
CXXFLAGS :=-std=c++14 -Wall `pkg-config --cflags --libs gtk+-3.0` 
INCLUDE  :=-I$(HDRDIR)
# MaKefile eXtension variables
MKXFILES :=$(wildcard *.mkx)
MKXCLEAN :=$(patsubst %.mkx,clean_%,$(MKXFILES))
# Files
SRC      :=$(shell find $(SRCDIR) -name '*.cpp')
DEP      :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(SRC))
OBJ      :=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRC))
MAIN     :=main
# Test files
PUREOBJ  :=$(filter-out $(OBJDIR)/$(SRCDIR)/$(MAIN).o,$(OBJ))
TSRC     :=$(shell find $(TESTDIR) -name '*.cpp' 2> /dev/null)
TMAIN    :=$(wildcard $(TESTDIR)/*.cpp)
TDEP     :=$(patsubst %.cpp,$(DEPDIR)/%.d,$(TSRC))
TOBJ     :=$(patsubst %.cpp,$(OBJDIR)/%.o,$(TSRC))
TPUREOBJ :=$(filter-out $(patsubst %.cpp,$(OBJDIR)/%.o,$(TMAIN)),$(TOBJ))
TEXEC    :=$(patsubst $(TESTDIR)/%.cpp,$(BINDIR)/%,$(TMAIN))
TCALL    :=$(patsubst %.cpp,%,$(notdir $(TMAIN)))

.PHONY: all makedir clean clean_deps clean_all tests shit win_install

################################# MAIN RULES ##################################
all: makedir $(EXEC)

win_install:
	cp external/libraties/windows/* bin/
	cp *.ui bin/

-include $(MKXFILES)

$(EXEC): $(OBJ)
	@echo "[linking] $@"
	@$(CXX) $(OBJ) -o $@ $(LDLIBS) $(LDFLAGS)


ifeq ($(ARCH), windows)
$(EXEC): export PKG_CONFIG_PATH:=/opt/crosstools/gtk_deps/lib/pkgconfig
$(OBJ): export PKG_CONFIG_PATH:=/opt/crosstools/gtk_deps/lib/pkgconfig
endif

$(OBJDIR)/%.o: %.cpp
	@echo "[  $(CXX)  ] $< -> .o"
	@mkdir -p $(OBJDIR)/$(*D)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# For each .cpp file, creates a .d file with all dependencies of .cpp,
# including .d as target (to ensure up-to-date dependencies, in case of
# new includes being added)
$(DEPDIR)/%.d: %.cpp
	@echo "[makedep] $< -> .d"
	@mkdir -p $(DEPDIR)/$(*D)
	@$(CXX) -MM -MP -MT "$(OBJDIR)/$*.o $@" -MF "$@" $< $(INCLUDE) $(CXXFLAGS)

makedir: | $(MAKEDIR)

$(MAKEDIR):
	@echo "[ mkdir ] Creating directory '$@'"
	@mkdir -p $@

################################ TESTS RULES ##################################
tests: makedir $(TCALL)

$(foreach t,$(TCALL),$(t): $(OBJDIR)/$(TESTDIR)/$(t).o $(BINDIR)/$(t))

$(TEXEC): $(PUREOBJ) $(TPUREOBJ)
	@echo "[linking] $@"
	@$(CXX) $(PUREOBJ) $(OBJDIR)/$(TESTDIR)/$(@F).o -o $@ $(LDLIBS) $(LDFLAGS)

################################ CLEAN RULES ##################################
# Only remove object files
clean: $(MKXCLEAN)
	@$(RM) -r $(OBJDIR)

# Remove object, binary and dependency files
clean_all: clean
	@$(RM) -r $(BINDIR)
	@$(RM) -r $(DEPDIR)

################################ PREREQUISITES ################################
# Do not include list of dependencies when they are going to be deleted, i.e.,
# when the target is clean_all
ifneq ($(MAKECMDGOALS), clean_all)
  -include $(DEP)
  ifeq ($(MAKECMDGOALS), tests)
    -include $(TDEP)
  endif
endif
