cmake_minimum_required(VERSION 3.8)

set (SIMULATOR_INCLUDE_FILES
    "${SIMULATOR_INCLUDE_FILES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/simulator.hpp"
    PARENT_SCOPE
)

set (SIMULATOR_SOURCE_FILES
    "${SIMULATOR_SOURCE_FILES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    PARENT_SCOPE
)
