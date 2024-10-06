# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Output binary
OUTPUT = mm_test

# Source files
SRCS = mm_implemented.c test_mm.c test_util.c syscall.c

# Header files (optional but good practice to include for dependency tracking)
HEADERS = mm_manager.h syscall.h test_util.h

# Object files (corresponding to the source files)
OBJS = $(SRCS:.c=.o)

# Default rule to build the project
all: $(OUTPUT)

# Rule to link the object files into the final binary
$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to compile each source file into object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the binary
clean:
	rm -f $(OBJS) $(OUTPUT)

# Phony targets to avoid name conflicts with files
.PHONY: all clean
