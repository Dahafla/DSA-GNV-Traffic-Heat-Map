cmake_minimum_required(VERSION 3.25)

project(DSA_GNV_Traffic_Heat_Map)

set(CMAKE_CXX_STANDARD 14)


# Add the executable
add_executable(DSA_GNV_Traffic_Heat_Map
        main.cpp
        README.md
        cmake-build-debug/Traffic_Crashes.csv
        bryHashMap.h
        bryHashMap.cpp
        ALBTree.cpp
        RBTree.cpp)

