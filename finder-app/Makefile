# Define the compiler
ifeq ($(CROSS_COMPILE),)
	CC = gcc
else
	CC = $(CROSS_COMPILE)gcc
endif

# Compile flags
CFLAGS = -Wall -Werror

# Default target to build the writer application
all: writer

# Rule to build the writer executable
writer: writer.o
	$(CC) $(CFLAGS) -o writer writer.o

# Rule to compile the source file and build the object file
writer.o: writer.c
	$(CC) $(CFLAGS) -c writer.c -o writer.o

# Clean target to remove the writer application and all .o files
clean:
	rm -rf writer *.o

# Phony targets to prevent conflicts
.PHONY: all clean
