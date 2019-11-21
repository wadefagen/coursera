#
# This is a generic Makefile designed to compile a sample directory of code.
# This file depends on variables having been set before calling:
#   EXE: The name of the result file
#   OBJS: Array of objects files (.o) to be generated
#   CLEAN_RM: Optional list of additional files to delete on `make clean`
#
# @author Wade Fagen-Ulmschneider, <waf@illinois.edu>
# @author Jeffrey Tolar
# @author Eric Huber (edits made for CS Fundamentals MOOC)
#


# Compiler/linker config and object/depfile directory:
CXX = g++
LD  = g++
OBJS_DIR = .objs

# -MMD and -MP asks clang++ to generate a .d file listing the headers used in the source code for use in the Make process.
#   -MMD: "Write a depfile containing user headers"
#   -MP : "Create phony target for each dependency (other than main file)"
#   (https://clang.llvm.org/docs/ClangCommandLineReference.html)
DEPFILE_FLAGS = -MMD -MP

# Provide lots of helpful warning/errors:
# (Switching from clang++ to g++ caused some trouble here. Not all flags are identically between the compilers.)
#WARNINGS_AS_ERRORS = -Werror # Un-commenting this line makes compilation much more strict.
GCC_EXCLUSIVE_WARNING_OPTIONS =  # -Wno-unused-but-set-variable
CLANG_EXCLUSIVE_WARNING_OPTIONS =  # -Wno-unused-parameter -Wno-unused-variable
ifeq ($(CXX),g++)
EXCLUSIVE_WARNING_OPTIONS = $(GCC_EXCLUSIVE_WARNING_OPTIONS)
else
EXCLUSIVE_WARNING_OPTIONS = $(CLANG_EXCLUSIVE_WARNING_OPTIONS)
endif
# ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer # for debugging, if supported on the OS
WARNINGS = -pedantic -Wall $(WARNINGS_AS_ERRORS) -Wfatal-errors -Wextra $(EXCLUSIVE_WARNING_OPTIONS)

# Flags for compile:
CXXFLAGS += -std=c++14 -O0 $(WARNINGS) $(DEPFILE_FLAGS) -g -c $(ASANFLAGS)

# Flags for linking:
LDFLAGS += -std=c++14 $(ASANFLAGS)

# Rule for `all` (first/default rule):
all: $(EXE)

# Rule for linking the final executable:
# - $(EXE) depends on all object files in $(OBJS)
# - `patsubst` function adds the directory name $(OBJS_DIR) before every object file
$(EXE): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS))
	$(LD) $^ $(LDFLAGS) -o $@

# Ensure .objs/ exists:
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/uiuc

# Rules for compiling source code.
# - Every object file is required by $(EXE)
# - Generates the rule requiring the .cpp file of the same name
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Additional dependencies for object files are included in the clang++
# generated .d files (from $(DEPFILE_FLAGS)):
-include $(OBJS_DIR)/*.d
-include $(OBJS_DIR)/uiuc/*.d


# Standard C++ Makefile rules:
clean:
	rm -rf $(EXE) $(TEST) $(OBJS_DIR) $(CLEAN_RM) *.o *.d

tidy: clean
	rm -rf doc

.PHONY: all tidy clean
