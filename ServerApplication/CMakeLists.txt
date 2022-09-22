cmake_minimum_required(VERSION 3.23)

project(ServerApplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(HEADERS include/ServerAppConfig.hpp include/ServerApplication.hpp)
set(SOURCES src/main.cpp src/ServerAppConfig.cpp src/ServerApplication.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 COMPONENTS system log REQUIRED)


set(CMAKE_C_FLAGS " -lboost_thread ")
set(CMAKE_CXX_FLAGS " -lboost_thread ")
set(CMAKE_C_FLAGS " -lpthread")
set(CMAKE_CXX_FLAGS " -lpthread")
find_library(pthread STATIC)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

target_include_directories(${PROJECT_NAME} PUBLIC ${ProcSignalGuard_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${ProcSignalGuard_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ProcSignalGuard_LIBRARIES})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::log_setup Boost::log)

add_subdirectory(libs/TcpServer)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/TcpServer/include)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE TcpServer)