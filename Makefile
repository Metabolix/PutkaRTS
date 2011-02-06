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

# Hack for mkdir differences (Windows / Linux).
# On Windows, echo "1" produces literally "1", while on Linux it produces 1.
ifeq "$(shell echo '1')" "1"
%.mkdir:
	@mkdir -p $(dir $@)
else
%.mkdir:
	@mkdir $(subst /,\,$@)
	@rmdir $(subst /,\,$@)
endif

# Build rules for binaries.
$(SERVERBIN): $(patsubst src/%,build/%.o,$(SERVERSRC))
$(CLIENTBIN): $(patsubst src/%,build/%.o,$(CLIENTSRC))

$(SERVERBIN) $(CLIENTBIN): bin/dummy.mkdir
	@echo [LINK] $@
	@$(CXX) $(CXXFLAGS) -o $@ $(filter %.o,$^) $(LIBS)

# Include dependencies; generation rules are below.
-include $(DEP)

# Dependency generation.
build/%.dep: src/% build/%.mkdir
	@echo [DEPS] $<
	@$(CXX) $(CXXFLAGS) -MM $< -MF $@ -MT $@

# Compilation
build/%.o: src/% build/%.dep build/%.mkdir
	@echo [CXX] $<
	@$(CXX) $(CXXFLAGS) $< -c -o $@
