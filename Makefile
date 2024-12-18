# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_GNU_SOURCE

# Target executable
TARGET = main.exe

# Source files
SRC = main.c

# Object files
OBJ = $(SRC:.c=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	del -f $(OBJ) $(TARGET)

# Convenience rule to run the program after building
run: all
	./$(TARGET)

.PHONY: all clean run
