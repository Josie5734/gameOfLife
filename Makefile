CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g $(shell pkg-config --cflags raylib)
LDFLAGS := $(shell pkg-config --libs raylib)

SRC_DIR := src
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TARGET := game_of_life

# Default target: just running `make` builds this
all: $(TARGET)

# Link step: combine all object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# Compile step: turn each .cpp into a .o, only if the .cpp changed
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make sure build/ exists before trying to put .o files there
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all run clean
