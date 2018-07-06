EXE = main
OBJS = main.o Cube.o
CLEAN_RM =

include ../_make/generic.mk

# Directory-specific rules:
heap1: heap1.cpp
	$(LD) $^ $(LDFLAGS) .objs/Cube.o -o $@

heap2: heap2.cpp
	$(LD) $^ $(LDFLAGS) .objs/Cube.o -o $@

all: heap1 heap2
CLEAN_RM += heap1 heap2
