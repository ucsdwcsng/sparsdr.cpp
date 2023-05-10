set(CMAKE_INCLUDE_CURRENT_DIR ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

if(MSVC)
    # force std::complex<> typedefs in liquiddsp
    add_definitions(-D_LIBCPP_COMPLEX)

    # enable math definitions in math.h
    add_definitions(-D_USE_MATH_DEFINES)

    # build a graphical application without the console
    option(BUILD_WIN32 "Build win32 app, false for console" TRUE)

    if(BUILD_WIN32)
        set(EXE_ARGS WIN32)
        set(CMAKE_EXE_LINKER_FLAGS "/entry:mainCRTStartup ${CMAKE_EXE_LINKER_FLAGS}")
    endif(BUILD_WIN32)
endif(MSVC)

if(NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-O2")
endif(NOT CMAKE_CXX_FLAGS)

# list(APPEND sparsdr_sources)
find_package(FFTW REQUIRED)
find_package(Liquid REQUIRED)

include_directories(
    ${FFTW_INCLUDES}
    ${LIQUID_INCLUDES}
)

list(APPEND sparsdr_common_sources
    sparsdr.cpp
    fftengine.cpp
    util.cpp
    iface.cpp
    window.cpp)

add_executable(${PROJECT_NAME}_compress ${sparsdr_common_sources} main_compress.cpp)
add_executable(${PROJECT_NAME}_reconstruct ${sparsdr_common_sources} main_reconstruct.cpp)
add_executable(${PROJECT_NAME}_filter ${sparsdr_common_sources} main_filter.cpp)

target_link_libraries(${PROJECT_NAME}_compress
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
target_link_libraries(${PROJECT_NAME}_reconstruct
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
target_link_libraries(${PROJECT_NAME}_filter
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)

include(CTest)

include_directories(${CMAKE_SOURCE_DIR}/src)
list(APPEND sparsdr_test_sources "${sparsdr_common_sources}")

add_executable(identity_test ${sparsdr_test_sources} ./tests/identitytest.cpp)
target_link_libraries(identity_test
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
add_test(NAME identity_test COMMAND identity_test)

add_executable(window_test ${sparsdr_test_sources} ./tests/windowtest.cpp)
target_link_libraries(window_test
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
add_test(NAME window_test COMMAND window_test)

add_executable(staggered_buf_test ${sparsdr_test_sources} ./tests/staggeredbuftest.cpp)
target_link_libraries(staggered_buf_test
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
add_test(NAME staggered_buf_test COMMAND staggered_buf_test)