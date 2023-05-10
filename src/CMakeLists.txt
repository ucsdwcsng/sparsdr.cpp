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

list(APPEND sparsdr_sources
    main.cpp
    sparsdr.cpp
    fftengine.cpp
    util.cpp
)

add_executable(${PROJECT_NAME} ${sparsdr_sources})

target_link_libraries(${PROJECT_NAME}
    ${FFTW_LIBRARIES}
    ${LIQUID_LIBRARIES}
)
