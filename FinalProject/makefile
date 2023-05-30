# Makefile Definitions
include makedefs

# Name of the project
PROJ_NAME = Main

# Paths to other makefiles
CONCURRENCY_MAKEFILE := $(SRC)/Concurrency/makefile
GAME_MAKEFILE := $(SRC)/Game/makefile
MAIN_MAKEFILE := $(SRC)/makefile

# Compiler and linker
CC = g++

# Command used at clean target
RM = rm -rf

# Default target
all: build

# Object files
OBJ = $(OBJDIR)/main.o $(wildcard $(OBJDIR)/*.o)

# Creates Main executable
$(PROJ_NAME): $(OBJ)
	@echo 'Building binary using GCC linker: $@'
	$(CC) $^ $(LIBS) -o $@
	@echo 'Finished building binary: $@'
	@echo ' '

build: compileSource
	@$(MAKE) -s $(PROJ_NAME)

compileSource:
	@mkdir -p $(OBJDIR)
	@$(MAKE) -s -C $(SRC)/Game -f $(GAME_MAKEFILE)
	@$(MAKE) -s -C $(SRC)/Concurrency -f $(CONCURRENCY_MAKEFILE)
	@$(MAKE) -s -C $(SRC) -f $(MAIN_MAKEFILE)

clean:
	@$(MAKE) -s -C $(SRC)/Concurrency -f $(CONCURRENCY_MAKEFILE) clean
	@$(MAKE) -s -C $(SRC)/Game -f $(GAME_MAKEFILE) clean
	@rm -rf $(PROJ_NAME) $(OBJDIR) *~

.PHONY: all clean