
# University of Illinois
# CS 400 - MOOC 2 - Week 1

# Original Makefile created by Wade Fagen-Ulmschneider <waf@illinois.edu>
# A few tweaks for CS 400 by Eric Huber

ZIP_FILE = LinkedList_submission.zip
COLLECTED_FILES = LinkedListExercises.h

# Add standard object files (HSLAPixel, PNG, and LodePNG)
OBJS +=  

# Use ./.objs to store all .o file (keeping the directory clean)
OBJS_DIR = .objs

# Use all .cpp files in /tests/
OBJS_TEST = $(filter-out $(EXE_OBJ), $(OBJS))
CPP_TEST = $(wildcard tests/*.cpp)
CPP_TEST += uiuc/catch/catchmain.cpp
OBJS_TEST += $(CPP_TEST:.cpp=.o)

# Config
CXX_CLANG = clang++
CXX_GCC = g++
CXX_WHICH = $(CXX_GCC)
CXX = $(CXX_WHICH)
LD = $(CXX_WHICH)
# STDVERSION = -std=c++1y # deprecated nomenclature
STDVERSION = -std=c++14 # proper but requires newer compiler versions (for better or worse)
STDLIBVERSION_CLANG = -stdlib=libc++ # Clang's version; not present on default AWS Cloud9 instance
STDLIBVERSION_GCC =   # blank on purpose; default GNU library
ifeq ($(CXX_WHICH),$(CXX_CLANG))
STDLIBVERSION = $(STDLIBVERSION_CLANG)
else
STDLIBVERSION = $(STDLIBVERSION_GCC)
endif
WARNINGS = -pedantic -Wall -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
# ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer
# NO_COPY_ELISION = -fno-elide-constructors
CXXFLAGS = $(CS400) $(STDVERSION) $(STDLIBVERSION) -g -O0 $(WARNINGS) -MMD -MP -msse2 -c $(ASANFLAGS) $(NO_COPY_ELISION)
LDFLAGS = $(CS400) $(STDVERSION) $(STDLIBVERSION) -lpthread $(ASANFLAGS) $(NO_COPY_ELISION)

ifneq ($(strip $(ASANFLAGS)),)
# This is displayed if ASANFLAGS is not blank.
ASANWARNING = "\n >>>>>>>>>> Note: ASAN is in use. May not be supported on Cloud9. <<<<<<<<<<"
endif

#  Rules for first executable
$(EXE):
	$(LD) $^ $(LDFLAGS) -o $@
	@echo
	@echo " Built the main executable program file for the project: " $(EXE)
	@echo " (Make sure you try \"make test\" too!)"
	@echo $(ASANWARNING)

# Rule for `all`
all: $(EXE) $(TEST)

# Pattern rules for object files
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/uiuc
	@mkdir -p $(OBJS_DIR)/uiuc/catch
	@mkdir -p $(OBJS_DIR)/tests

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Rules for executables
$(TEST):
	$(LD) $^ $(LDFLAGS) -o $@
	@echo
	@echo " Built the test suite program: " $(TEST)
	@echo $(ASANWARNING)

# Executable dependencies
$(EXE): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS))
$(TEST): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_TEST))

# Include automatically generated dependencies
-include $(OBJS_DIR)/*.d
-include $(OBJS_DIR)/uiuc/*.d
-include $(OBJS_DIR)/uiuc/catch/*.d
-include $(OBJS_DIR)/tests/*.d

clean:
	rm -rf $(EXE) $(TEST) $(OBJS_DIR) $(CLEAN_RM)

tidy: clean
	rm -rf doc

zip:
	@echo "!!! Preparing submission zip with student code..."
	@echo "!!! Make sure you have already tried compiling and testing your code"
	@echo "!!! thoroughly before submitting the zip on Coursera!"
	@echo ""
	@echo "Removing any previous version of zip file..."
	rm -rf $(ZIP_FILE)
	@echo "Creating new file..."
	zip $(ZIP_FILE) $(COLLECTED_FILES)
	@echo "Created zip file: " $(ZIP_FILE)

.PHONY: all tidy clean zip
