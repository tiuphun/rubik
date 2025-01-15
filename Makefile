# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Isrc/constants -Isrc/database/header -Isrc/database/queries/ -Isrc/models/header -Isrc/server -Isrc/states -Isrc/messages -Isrc/services -Isrc/repositories -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/main/*.cpp) \
       $(wildcard $(SRC_DIR)/server/*.cpp) \
       $(wildcard $(SRC_DIR)/database/queries/*.cpp) \
       $(wildcard $(SRC_DIR)/database/functions/*.cpp) \
       $(wildcard $(SRC_DIR)/models/entities/*.cpp) \
       $(wildcard $(SRC_DIR)/services/*.cpp) \
       $(wildcard $(SRC_DIR)/repositories/*.cpp) \
       $(wildcard $(SRC_DIR)/messages/*.cpp)  # Ensure these exist

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Include directories
INCLUDES = -I$(SRC_DIR)/constants \
           -I$(SRC_DIR)/database/header \
           -I$(SRC_DIR)/database/queries/ \
           -I$(SRC_DIR)/models/header \
           -I$(SRC_DIR)/server \
           -I$(SRC_DIR)/states \
           -I$(SRC_DIR)/messages \
           -I$(SRC_DIR)/services \
           -I$(SRC_DIR)/repositories \
           -Iinclude

# Libraries
LIBS = -lsqlite3 -lcrypto

# Target executable
TARGET = rubik-server

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)  # Create directories as needed
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
