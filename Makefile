CXX := g++

GDB := -ggdb

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations -Wredundant-decls \
			-Winline -Wno-long-long -Wconversion

CXXFLAGS := -std=c++2a -c $(WARNINGS) $(GDB) -fcoroutines

EXEC := twitchbot

LIBS := -lsockpp

SRCFILES := $(shell find src -type f -name "*.cpp")

DIRS := build/commands build

all: $(DIRS) $(EXEC)

build/commands:
	mkdir -p $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

OBJECTS := $(patsubst src/%.cpp,build/%.o,$(SRCFILES))

$(EXEC): $(OBJECTS)
	$(CXX) -O2 -o $@ $^ $(LIBS)
	@echo "Succesfully build"

# Phony targets:
clean: # Remove all object files
	-rm -r $(DIRS)

veryclean:
	-rm -r $(EXEC) $(DIRS)

# Set the phony targets
.PHONY: clean veryclean build/commands
