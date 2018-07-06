EXE = puzzle1
OBJS = puzzle1.o
CLEAN_RM =

include ../_make/generic.mk

# Directory-specific rules:

puzzle2: puzzle2.cpp
	$(LD) $^ $(LDFLAGS) -o $@

puzzle3: puzzle3.cpp
	$(LD) $^ $(LDFLAGS) -o $@

puzzle4: puzzle4.cpp
	$(LD) $^ $(LDFLAGS) -o $@

all: puzzle2 puzzle3 puzzle4
CLEAN_RM += puzzle2 puzzle3 puzzle4
