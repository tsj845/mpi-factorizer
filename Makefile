# Makefile for Pipeline Performance Demo
# For parallel computing class demonstration

CC = mpicc
CFLAGS = -g -Wall
LIBS = 
TARGET = factorizer
SOURCE = factorizer.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Built $(TARGET) with flags: $(CFLAGS)"
	@echo "Ready to run: ./$(TARGET)"

clean:
	rm -f $(TARGET)

.PHONY: all clean
