cmake_minimum_required(VERSION 3.10)

# Project name
project(RubiksServer)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
file(GLOB SERVER_SRC "src/server/*.cpp")
file(GLOB DATABASE_SRC "src/database/*.cpp")
file(GLOB UTILS_SRC "src/utils/*.cpp")
file(GLOB MODELS_SRC "src/models/*.cpp")

# Include directories
include_directories(include)
include_directories(src)

# SQLite
find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

# Target executable
add_executable(server ${SERVER_SRC} ${DATABASE_SRC} ${UTILS_SRC} ${MODELS_SRC})

# Link SQLite
target_link_libraries(server ${SQLite3_LIBRARIES})
