cmake_minimum_required(VERSION 3.23)

project(Message)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SOURCES src/Message.cpp)
set(HEADERS include/Message.hpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)


target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )

