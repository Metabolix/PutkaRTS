CXX := c++
CXXFLAGS := -O -g -std=c++98 -Wall -pedantic
INCLUDES := -Isrc

PUTKARTS_VERSION :=
CXX_VER = "-DPUTKARTS_VERSION=\"$(PUTKARTS_VERSION)\""

FILES_CPP := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp)
FILES_HPP := $(wildcard src/*.hpp) $(wildcard src/*/*.hpp) $(wildcard src/*/*/*.hpp) $(wildcard src/*/*/*/*.hpp)
FILES_DEP := $(patsubst src/%,build/%.dep,$(FILES_CPP))

SERVERSRC := $(filter-out src/client/%,$(FILES_CPP))
SERVERBIN := bin/PutkaRTSd
SERVERLIBS := -lsfml-system -lsfml-network -lboost_filesystem -lboost_system

CLIENTSRC := $(filter-out src/server/%,$(FILES_CPP))
CLIENTBIN := bin/PutkaRTS
CLIENTLIBS := $(SERVERLIBS) -lsfml-window -lsfml-graphics -lsfml-audio

# Hack for OS differences.
# On Windows, echo '1' produces literally '1' instead of 1.
ifeq "$(shell echo '1')" "'1'"
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

  # Mac OS X needs different libraries
  ifeq "$(shell uname -s)" "Darwin"
    SERVERLIBS := $(patsubst -l%,-framework %,$(SERVERLIBS))
    CLIENTLIBS := $(patsubst -l%,-framework %,$(CLIENTLIBS))
  endif

  # MinGW needs .exe suffices.
  ifeq "$(findstring mingw,$(CXX))" "mingw"
    SERVERBIN := $(SERVERBIN).exe
    CLIENTBIN := $(CLIENTBIN).exe
  endif
endif

# Abstract build rules.
all: client server
client: $(CLIENTBIN)
server: $(SERVERBIN)
clean:
	@echo [RM] $(call rm_rf,build html bin/PutkaRTS*)
clean_deps:
	@echo [RM] $(call rm_rf,$(FILES_DEP))
clean_html:
	@echo [RM] $(call rm_rf,html)

# Documentation build with Doxygen
html: Doxyfile $(FILES_CPP) $(FILES_HPP)
	doxygen

# Build rules for binaries.
$(SERVERBIN): $(patsubst src/%,build/%.o,$(SERVERSRC))
$(CLIENTBIN): $(patsubst src/%,build/%.o,$(CLIENTSRC))

$(CLIENTBIN):
	@echo [LINK] $@
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) -o $@ $(filter %.o,$^) $(CLIENTLIBS)

$(SERVERBIN):
	@echo [LINK] $@
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) -o $@ $(filter %.o,$^) $(SERVERLIBS)

# Include dependencies; generation rules are below.
-include $(FILES_DEP)

# Dependency generation.
build/%.dep: src/%
	@echo [DEPEND] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(CXX_VER) -MM $< -MT $@ > $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(CXX_VER) $< -c -o $@

# Always regenerate program version string
build/ProgramInfo.cpp.o: $(FILES_CPP) $(FILES_HPP)
