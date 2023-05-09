cmake_minimum_required(VERSION 3.16)

project(sparsdr LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# Build options
option(FOOBAR "Example option" ON)
option(ENABLE_TESTING "Build testfiles" ON)

if(ENABLE_TESTING)
    enable_testing()
endif()

# Install options
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Build project libraries
# include_directories(${CMAKE_SOURCE_DIR}/src)

# target_link_libraries(${PROJECT_NAME} pthread)
# target_link_directories()

# add_executable(${PROJECT_NAME} src/main.cpp)
# add_library(${PROJECT_NAME} src/sparsdr.cpp)
add_subdirectory(src)
