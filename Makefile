CXX := c++
CXXFLAGS := -O -g -std=c++98 -Wall -pedantic
INCLUDES := -Isrc

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
  # Windows needs a hack for mkdir
  mkdir = $(shell mkdir $(subst /,\,$(1)/dummy.mkdir) && rmdir $(subst /,\,$(1)/dummy.mkdir))
else
  # Linux and Mac OS X have better mkdir
  mkdir = $(shell mkdir -p $(1))
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
	rm -rf build html $(CLIENTBIN) $(SERVERBIN)
clean_deps:
	rm -f $(FILES_DEP)
clean_html:
	rm -rf html

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
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM $< -MT $@ > $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $< -c -o $@
