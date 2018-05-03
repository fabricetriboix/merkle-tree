cmake_minimum_required(VERSION 3.1)
project(merkle-tree LANGUAGES CXX)
enable_testing()

add_subdirectory(googletest)
config_compiler_and_linker()

add_library(merkle_tree STATIC
    include/merkle-tree/merkle-tree.hpp
    src/merkle-tree/merkle-tree.cpp)

set_property(TARGET merkle_tree PROPERTY CXX_STANDARD 98)
set_property(TARGET merkle_tree PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET merkle_tree PROPERTY CXX_EXTENSIONS OFF)

# Turn on maximum warnings
if (CMAKE_COMPILER_IS_GNUCXX)
target_compile_options(merkle_tree PUBLIC -Wall -Wextra -Werror
    -Wno-unused-parameter)
endif()

target_include_directories(merkle_tree
    PUBLIC include)

#target_link_libraries(merkle_tree
#    XXX)
