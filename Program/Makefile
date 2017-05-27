CPP      = /usr/bin/g++
CPPFLAGS = -ldl
BINARY   = ../corruptor
SOURCE   = $(wildcard *.cpp)

all: $(BINARY)

$(BINARY): $(SOURCE)
	$(CPP) $(CPPFLAGS) -o $(BINARY)
	@echo "Build complete"
	
.PHONY: all
