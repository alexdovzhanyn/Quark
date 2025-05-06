CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -Isrc

SRC_DIR := src
BUILD_DIR := build
LIB := $(BUILD_DIR)/libquark.a

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
