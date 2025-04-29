CXX := /usr/bin/clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -g

TARGET := quark_server
SRC_DIR := src
BUILD_DIR := build

SRCS := main.cpp $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
FINAL_TARGET := $(BUILD_DIR)/$(TARGET)

all: $(FINAL_TARGET)

$(FINAL_TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
