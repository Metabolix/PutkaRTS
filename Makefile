CXX := c++
CXXFLAGS := -O -g -std=c++98 -Wall -pedantic
LINKFLAGS := -O -g
INCLUDE_DIRS := -Isrc -isystem ext/include
LIB_DIRS := -Lext/lib

PUTKARTS_VERSION :=
CXX_VER = "-DPUTKARTS_VERSION=\"$(PUTKARTS_VERSION)\""

FILES_CPP := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp)
FILES_HPP := $(wildcard src/*.hpp) $(wildcard src/*/*.hpp) $(wildcard src/*/*/*.hpp) $(wildcard src/*/*/*/*.hpp)
FILES_DEP := $(patsubst src/%,build/%.dep,$(FILES_CPP))

CLI_SRC := $(filter-out src/gui/%,$(FILES_CPP))
CLI_BIN := bin/PutkaRTS-cli
CLI_LIBS := -lboost_chrono -lboost_filesystem -lboost_system -lboost_thread -llua

GUI_SRC := $(filter-out src/cli/%,$(FILES_CPP))
GUI_BIN := bin/PutkaRTS
GUI_LIBS := $(CLI_LIBS) -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

# Hack for OS differences.
# On Windows, echo '1' produces literally '1' instead of 1.
ifeq "$(shell echo '1')" "'1'"
  WIN32 := 1
  mkdir = $(shell mkdir $(subst /,\,$(1)/dummy.mkdir) && rmdir $(subst /,\,$(1)/dummy.mkdir))
  rm_rf = $(foreach F,$(subst /,\,$(1)),$(shell rmdir /Q /S $(F) 2>NUL >NUL || del /Q /S $(F) 2>NUL >NUL)) $(1)

  # Get the version number from git; we must first check manually that git.exe exists
  ifeq "$(findstring git,$(shell for %%i in (git.exe) do @echo.%%~$$PATH:i))" "git"
    PUTKARTS_VERSION := $(strip $(shell git describe 2>NUL))
  endif
else
  mkdir = $(shell mkdir -p $(1))
  rm_rf = $(shell rm -rf $(1)) $(1)

  # Get the version number from git
  PUTKARTS_VERSION := $(strip $(shell git describe 2>/dev/null))

  # Something needs pthread.
  CLI_LIBS += -lpthread
  GUI_LIBS += -lpthread

  # Mac OS X needs different libraries
  ifeq "$(shell uname -s)" "Darwin"
    CLI_LIBS := $(patsubst -l%,-framework %,$(CLI_LIBS))
    GUI_LIBS := $(patsubst -l%,-framework %,$(GUI_LIBS))
  endif

  ifeq "$(findstring mingw,$(CXX))" "mingw"
    WIN32 := 1
  endif
endif

ifdef WIN32
  CLI_BIN := $(CLI_BIN).exe
  GUI_BIN := $(GUI_BIN).exe
  CLI_LIBS := $(CLI_LIBS) -lws2_32
  GUI_LIBS := $(GUI_LIBS) -lws2_32
  CXXFLAGS2 := -D_WIN32_WINNT=0x0501
endif

# Abstract build rules.
all: cli gui
gui: $(GUI_BIN)
cli: $(CLI_BIN)
clean:
	@echo [RM] $(call rm_rf,build html bin/PutkaRTS*)
clean_deps:
	@echo [RM] $(call rm_rf,$(FILES_DEP))
clean_html:
	@echo [RM] $(call rm_rf,html)

# Documentation build with Doxygen
html: Doxyfile $(FILES_CPP) $(FILES_HPP)
	@echo [DOXYGEN]
	@$(call mkdir,html)
	@doxygen > html/doxygen.log

# Build rules for binaries.
$(CLI_BIN): $(patsubst src/%,build/%.o,$(CLI_SRC))
$(GUI_BIN): $(patsubst src/%,build/%.o,$(GUI_SRC))

$(GUI_BIN):
	@echo [LINK] $@
	@$(call mkdir,$(dir $@))
	@$(CXX) $(LINKFLAGS) $(LIB_DIRS) -o $@ $(filter %.o,$^) $(GUI_LIBS)

$(CLI_BIN):
	@echo [LINK] $@
	@$(call mkdir,$(dir $@))
	@$(CXX) $(LINKFLAGS) $(LIB_DIRS) -o $@ $(filter %.o,$^) $(CLI_LIBS)

# Include dependencies; generation rules are below.
-include $(FILES_DEP)

# Dependency generation.
build/%.dep: src/%
	@echo [DEPEND] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS2) $(INCLUDE_DIRS) $(CXX_VER) -MM $< -MT $@ -MP > $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(CXXFLAGS2) $(INCLUDE_DIRS) $(CXX_VER) $< -c -o $@

# Always regenerate program version string
build/ProgramInfo.cpp.o: $(FILES_CPP) $(FILES_HPP)
