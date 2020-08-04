cmake_minimum_required(VERSION 3.11...3.16)

project("Jack Compiler"
  VERSION 1.0
  DESCRIPTION "A Jack compiler built in C++"
  LANGUAGES CXX
)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)

  if (BUILD_TESTING)
    add_subdirectory(test)
  endif()
endif()