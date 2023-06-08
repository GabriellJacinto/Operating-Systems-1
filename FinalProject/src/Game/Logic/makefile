# Makefile Definitions
include ../../makedefs

# Name of the project
PROJ_NAME = Main

# .cc files
CC_SOURCE = $(wildcard ./*.cc)

# Object files
OBJ = $(patsubst %.cc,$(OBJDIR)/%.o,$(notdir $(CC_SOURCE)))

# Compiler and linker
CC = g++

# Command used at clean target
RM = rm -rf

#
# Compilation
#
all: compileObjects $(OBJ)

$(OBJDIR)/%.o: %.cc
	@echo 'Building target using GCC compiler: $<'
	$(CC) $(CC_FLAGS) $< -o $@
	@echo ' '

compileObjects:
	@mkdir -p $(OBJDIR)

clean:
	@$(RM) $(OBJDIR)/*.o *~

.PHONY: all clean