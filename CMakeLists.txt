cmake_minimum_required(VERSION 3.16)

project(sparsdr LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# Build options
option(FOOBAR "Example option" ON)
option(ENABLE_TESTING "Build testfiles" ON)

if(ENABLE_TESTING)
    enable_testing()
endif()

add_subdirectory(src)

# Install options
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)