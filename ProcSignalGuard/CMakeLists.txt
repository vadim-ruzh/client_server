cmake_minimum_required(VERSION 3.23)

project(ProcSignalGuard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(HEADERS include/ProcSignalGuard.hpp)
set(SOURCES src/ProcSignalGuard.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include)