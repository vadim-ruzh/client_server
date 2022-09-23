cmake_minimum_required(VERSION 3.23)

project(ClientApplication)

set(HEADERS include/ClientApplication.hpp include/ClientAppConfig.hpp)

set(SOURCES src/main.cpp src/ClientApplication.cpp src/ClientAppConfig.cpp
        src/UserInputProcessor/UserInputProcessor.hpp src/UserInputProcessor/UserInputProcessor.cpp
        src/NetworkTrafficProcessor/NetworkTrafficProcessor.hpp src/NetworkTrafficProcessor/NetworkTrafficProcessor.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 COMPONENTS system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/TcpClient)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/TcpClient/include)
target_link_libraries(${PROJECT_NAME} PRIVATE TcpClient)

target_link_libraries(${PROJECT_NAME} PRIVATE ProcSignalGuard)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/src/UserInputProcessor
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/src/NetworkTrafficProcessor
        )





