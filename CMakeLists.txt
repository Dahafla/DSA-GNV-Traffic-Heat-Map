cmake_minimum_required(VERSION 3.12)

project(DSA_GNV_Traffic_Heat_Map)

set(CMAKE_CXX_STANDARD 14)

# Find Package
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Python3 COMPONENTS NumPy)

if (NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python3 interpreter not found.")
endif()

if (NOT Python3_NumPy_FOUND)
    message(FATAL_ERROR "NumPy not found. Please install NumPy or adjust your CMake configuration.")
endif()

# Set the path to the matplotlibcpp.h header file
set(MATPLOTLIBCPP_PATH "absolute/path/to/matplotlibcpp.h")

# Include the directory containing matplotlibcpp.h
include_directories(${Python3_Interpreter_INCLUDE_DIRS})
include_directories(${Python3_NumPy_INCLUDE_DIRS})  # Include NumPy include directories

# Add the executable
add_executable(DSA_GNV_Traffic_Heat_Map
        main.cpp
        README.md
        cmake-build-debug/Traffic_Crashes.csv
        bryHashMap.h
        bryHashMap.cpp
        ALBTree.cpp
        ALBTree.h)

# Link against Python libraries
target_link_libraries(DSA_GNV_Traffic_Heat_Map Python3::Python)
