# My third makefile

# Name of the project
PROJ_NAME=main

# .c files
CC_SOURCE=$(wildcard ./source/*.cc)

# .h files
H_SOURCE=$(wildcard ./headers/*.h)

# Object files
OBJ=$(subst .cc,.o,$(subst source,objects,$(CC_SOURCE)))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -ansi      \
         -pedantic  \
		 -g         \
		 -I ./headers \
         -std=c++11
# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./source/%.cc ./headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

./objects/main.o: ./source/main.cc $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean
