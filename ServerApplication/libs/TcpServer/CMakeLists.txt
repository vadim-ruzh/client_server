cmake_minimum_required(VERSION 3.23)

project(TcpServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(HEADERS include/TcpServer.hpp)
set(SOURCES src/СlientSessionGroup.hpp src/СlientSessionGroup.cpp src/TcpServer.cpp src/ClientSession.hpp src/ClientSession.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 COMPONENTS system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)

target_link_libraries(${PROJECT_NAME} PUBLIC Message)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        )









