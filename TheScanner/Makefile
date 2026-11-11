CXX = g++
CXXFLAGS = -std=c++17 -O2 -Iinclude
SRC_DIR = src
BUILD_DIR = build
TARGET = scanner

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
 $(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS)
 $(CXX) $(OBJS) -o $(TARGET)

clean:
 rm -rf $(BUILD_DIR)/*.o $(TARGET)
