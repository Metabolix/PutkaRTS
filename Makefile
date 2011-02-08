CXXFLAGS := -Isrc -O -g -std=c++98 -Wall -pedantic

FILES_CPP := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp)
FILES_HPP := $(wildcard src/*.hpp) $(wildcard src/*/*.hpp) $(wildcard src/*/*/*.hpp) $(wildcard src/*/*/*/*.hpp)
FILES_DEP := $(patsubst src/%,build/%.dep,$(FILES_CPP))

SERVERSRC := $(filter-out src/client/%,$(FILES_CPP))
SERVERBIN := bin/PutkaRTSd
SERVERLIBS := -lsfml-system -lsfml-network

CLIENTSRC := $(filter-out src/server/%,$(FILES_CPP))
CLIENTBIN := bin/PutkaRTS
CLIENTLIBS := $(SERVERLIBS) -lsfml-window -lsfml-graphics -lsfml-audio

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

# Hack for mkdir differences (Windows / Linux).
# On Windows, echo '1' produces literally '1', while on Linux it produces 1.
ifeq "$(shell echo '1')" "1"
mkdir = $(shell mkdir -p $(1))
else
mkdir = $(shell mkdir $(subst /,\,$(1)/dummy.mkdir) && rmdir $(subst /,\,$(1)/dummy.mkdir))
endif

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
	@$(CXX) $(CXXFLAGS) -MM $< -MF $@ -MT $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $< -c -o $@
