# Compiler and flags
CC := gcc
CFLAGS := -Iinclude
LDFLAGS := -Llib
LIBS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# Source and build directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Source files and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Output executable
TARGET := $(BIN_DIR)/main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)