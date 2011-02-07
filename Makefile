CXXFLAGS := -Isrc -O -g -std=c++98 -Wall -pedantic

SRC := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
DEP := $(patsubst src/%,build/%.dep,$(SRC))

SERVERSRC := $(filter-out src/client/%,$(SRC))
SERVERBIN := bin/PutkaRTSd
SERVERLIBS := -lsfml-system -lsfml-network

CLIENTSRC := $(filter-out src/server/%,$(SRC))
CLIENTBIN := bin/PutkaRTS
CLIENTLIBS := $(SERVERLIBS) -lsfml-window -lsfml-graphics -lsfml-audio

# Abstract build rules.
all: client server
client: $(CLIENTBIN)
server: $(SERVERBIN)
clean:
	rm -rf build $(CLIENTBIN) $(SERVERBIN)
clean_deps:
	rm -f $(DEPS)

# Documentation build with Doxygen
html: Doxyfile $(SRC)
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
-include $(DEP)

# Dependency generation.
build/%.dep: src/%
	@echo [DEPS] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) -MM $< -MF $@ -MT $@

# Compilation
build/%.o: src/% build/%.dep
	@echo [CXX] $<
	@$(call mkdir,$(dir $@))
	@$(CXX) $(CXXFLAGS) $< -c -o $@
