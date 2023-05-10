cmake_minimum_required(VERSION 3.16)

project(sparsdr LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# Build options
option(FOOBAR "Example option" ON)
option(ENABLE_TESTING "Build testfiles" ON)

if(ENABLE_TESTING)
    enable_testing()
endif()

include_directories(src)
add_subdirectory(src)

# if(ENABLE_TESTING)
# add_subdirectory(test)
# endif()

# Install options
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)