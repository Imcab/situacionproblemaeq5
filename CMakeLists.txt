cmake_minimum_required(VERSION 3.16)
project(situacion_problema_eq5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(main main.cpp)
target_compile_options(main PRIVATE -Wall -Wextra)

configure_file(${CMAKE_SOURCE_DIR}/resources/equipo5.csv
               ${CMAKE_BINARY_DIR}/equipo5.csv COPYONLY)
