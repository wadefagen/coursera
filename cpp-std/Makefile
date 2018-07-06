EXE = main
OBJS = main.o Cube.o
CLEAN_RM =

include ../_make/generic.mk

# Directory-specific rules:
cout: cout.cpp
	$(LD) $^ $(LDFLAGS) -o $@

cout2: cout2.cpp
	$(LD) $^ $(LDFLAGS) -o $@

all: cout cout2
CLEAN_RM += cout cout2
