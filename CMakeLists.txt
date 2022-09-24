cmake_minimum_required(VERSION 3.23)

project(ClientAndServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "Release"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY CMAKE_LIBRARY_OUTPUT_DIRECTORY)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 REQUIRED)
if(NOT Boost_FOUND)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif()
    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_run(REQUIRES boost/1.76.0@conan/stable BASIC_SETUP BUILD missing)

    SET(BOOST_ROOT ${CONAN_BOOST_ROOT})
    SET(BOOST_INCLUDEDIR ${CONAN_INCLUDE_DIRS_BOOST})
    set(BOOST_LIBRARYDIR ${CONAN_LIB_DIRS_BOOST})
    set(BOOST_LIBRARY_DIR ${CONAN_LIB_DIRS_BOOST})
endif()

add_subdirectory(Packet)
add_subdirectory(ProcSignalGuard)
add_subdirectory("ClientApplication(Program_1)")
add_subdirectory("ServerApplication(Program_2)")

