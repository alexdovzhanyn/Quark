CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g -I../src
LDFLAGS := ../build/libquark.a

BUILD_DIR := build
TARGET := $(BUILD_DIR)/example_app

SRC := main.cpp $(wildcard controllers/*.cpp)
OBJ := $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.cpp=.o)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/example_app $(OBJ)

.PHONY: all clean
