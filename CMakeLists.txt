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

#target_link_libraries(merkle_tree XXX)

add_executable(unit-tests test/test-merkle-tree.cpp)
set_property(TARGET unit-tests PROPERTY CXX_STANDARD 98)
set_property(TARGET unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET unit-tests PROPERTY CXX_EXTENSIONS OFF)

# Turn on maximum warnings
if (CMAKE_COMPILER_IS_GNUCXX)
target_compile_options(unit-tests PUBLIC -Wall -Wextra -Werror
    -Wno-unused-parameter)
endif()

target_link_libraries(unit-tests merkle_tree gtest gmock_main)

add_test(NAME merke-tree-tests COMMAND unit-tests)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(Doxyfile.in Doxyfile @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating doxygen documentation"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()
