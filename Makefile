# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/main/*.cpp) \
       $(wildcard $(SRC_DIR)/server/*.cpp) \
       $(wildcard $(SRC_DIR)/database/queries/*.cpp) \
	   $(wildcard $(SRC_DIR)/database/functions/*.cpp) \
       $(wildcard $(SRC_DIR)/models/entities/*.cpp) 

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include directories
INCLUDES = -I$(SRC_DIR)/constants \
           -I$(SRC_DIR)/database/header \
		   -I$(SRC_DIR)/database/queries/ \
           -I$(SRC_DIR)/models/header \
           -I$(SRC_DIR)/server \
           -I$(SRC_DIR)/states

# Target executable
TARGET = rubik-server

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ -lsqlite3

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build directory
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean