# Compiler
CC := gcc
CFLAGS := -Wall -ansi -pedantic -g -I"Header Files"
SRCS := $(wildcard Source_Files/*.c)
OBJS := $(SRCS:Source_Files/%.c=Source_Files/%.o)
LIBS := $(wildcard Header_Files/*.h)
PFILES := $(wildcard *.am) $(wildcard *.ob) $(wildcard *.ent) $(wildcard *.ext) # Files created by the assembler

# Executable
TARGET := assembler

# Default target
all: $(TARGET)

# Compile source files into object files
Source_Files/%.o: Source_Files/%.c $(LIBS)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Clean up object files and executable
clean:
	rm -r $(OBJS) $(PFILES)
