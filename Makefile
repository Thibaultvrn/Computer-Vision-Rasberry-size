# Makefile pour compiler Algorith.C avec OpenCV
# Usage: make
# Pour WSL/Linux uniquement

CC = gcc
CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
TARGET = algorith
SRC = Algorith.C

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) raspberry.jpg 0.5

.PHONY: all clean run
