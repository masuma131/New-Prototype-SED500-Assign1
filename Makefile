# Compiler
CC=gcc
# Compiler flags
CFLAGS=-I. -Wall -std=c++20 -D_DEFAULT_SOURCE -Wno-missing-braces -s -O1 
# Source files
FILES=main.cpp
# FILES+=./button/button.cpp
# Libraries
LIBS=-L./raylibLibrary -lraylib
# Target executable
TARGET=game


# Platform-specific flags and libraries
ifeq ($(OS),Windows_NT)
	# Windows specific flags and libraries #
	LIBS += -lopengl32 -lgdi32 -lkernel32 -luser32 -lshell32
	CC = gcc
else
	UNAME_S := $(shell uname -s)
	cc = clang++
	ifeq ($(UNAME_S),Darwin)
		# MacOS specific flags and libraries #
		LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	else
		# Linux specific flags and libraries #
		LIBS += -lGL -lm -lpthread -ldl -lrt -lX11
	endif
endif

# Build rule for the target
$(TARGET): $(FILES)
	$(CC) $(CFLAGS) $(FILES)  -o $(TARGET) $(LIBS)

# Clean rule
clean:
	rm -f *.o $(TARGET)

# Default rule (all) to build the target
all: $(TARGET)
