EXE = byValue
OBJS = byValue.o ../Cube.o
CLEAN_RM =

include ../../_make/generic.mk

byRef: byRef.cpp
	$(LD) $^ $(LDFLAGS) Cube.o -o $@

byPointer: byPointer.cpp
	$(LD) $^ $(LDFLAGS) Cube.o -o $@

all: byRef byPointer
CLEAN_RM += byRef byPointer


