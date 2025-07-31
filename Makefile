CC=gcc
CFLAGS=-Wall -std=c99 -I.
LDFLAGS=

SRC_DIR = src
BUILD_DIR = build

HEADERS = $(addprefix $(SRC_DIR)/, net.h http.h tree.h type.h hashmap.h)
SOURCES = $(addprefix $(SRC_DIR)/, net.c http.c tree.c type.c hashmap.c main.c)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))
TARGET = $(BUILD_DIR)/main

.PHONY: default all clean run

default: all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)