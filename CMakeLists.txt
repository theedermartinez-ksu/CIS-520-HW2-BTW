cmake_minimum_required(VERSION 2.8)

# ALL OF THE SPECIFIED NAMES IN THIS CMAKE ARE REQUIRED.
# Set the project name variable to hw2.
# THIS IS REQUIRED
# INSTRCUTRIONS
# Set Minimun CMake version
# Set project
# Set compiler flag
# Add library "target"
# add sources to that same library
# TARGET include directories
# add executable target
# add sources to the executable
# link to the library
project(hw2) #project does checks to ensure the envrionment is good for building software
# such as checking the RUNTIME ENVIRONMENT
# project should always be the second command run

# Add your CMAKE_C_FLAGS AND CMAKE_CXX_FLAGS here
# Make SURE to use -std=c11 and -std=c++11 where relevant. (Done)
# The options specified below are required, but additional options
#  may be used.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

# Add our include directory to CMake's search paths
# THIS IS REQUIRED
include_directories(include)

# Create library from dyn_array so we can use it later
add_library(dyn_array)

#target_sources(dyn_array
# Compile the analysis executable
add_executable(analysis src/analysis.c)
target_sources(
        dyn_array
        PRIVATE
            src/dyn_array.c
)

# link the dyn_array library we compiled against our analysis executable
target_include_directories(analysis
    PUBLIC
        dyn_array
)

target_link_libraries(analysis PRIVATE dyn_array)

# Compile the tester executable
add_executable(${PROJECT_NAME}_test test/tests.cpp)

target_compile_definitions(${PROJECT_NAME}_test PRIVATE)

# Link ${PROJECT_NAME}_test with dyn_array and gtest and pthread libraries
target_link_libraries(${PROJECT_NAME}_test gtest pthread dyn_array)
