CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -Isrc

SRC_DIR := src
BUILD_DIR := build
LIB := $(BUILD_DIR)/libquark.a

SRC := $(shell find $(SRC_DIR) -name "*.cpp")

OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
