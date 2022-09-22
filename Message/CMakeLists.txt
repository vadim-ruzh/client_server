cmake_minimum_required(VERSION 3.23)

project(message)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


set(SOURCES ./src/Message.cpp)
set(HEADERS ./include/Message.hpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 REQUIRED)
if(Boost_FOUND)

    add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

    target_include_directories(${PROJECT_NAME} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            )
else()
    message(FATAL_ERROR "Boost not found")
endif()

