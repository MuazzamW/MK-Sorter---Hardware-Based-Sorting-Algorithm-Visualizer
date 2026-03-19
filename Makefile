CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

TARGET = cpulator
SRC = cpulator.c

# Default target
all: $(TARGET)

# Build the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run with user-provided files
combine: $(TARGET)
	@if [ -z "$(FILES)" ]; then \
		echo "Usage: make combine FILES=\"file1.c file2.h ...\""; \
	else \
		./$(TARGET) $(FILES); \
	fi

# Clean build artifacts
clean:
	rm -f $(TARGET) combined_output.c

.PHONY: all combine clean