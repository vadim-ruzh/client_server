cmake_minimum_required(VERSION 3.23)

project(ClientApplication)

set(HEADERS include/ClientApplication.hpp include/ClientAppConfig.hpp include/NetworkProcessor.hpp include/UserInputProcessor.hpp)
set(SOURCES src/main.cpp src/ClientApplication.cpp src/ClientAppConfig.cpp src/NetworkProcessor.cpp src/UserInputProcessor.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/TcpClient)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/TcpClient/include)
target_link_libraries(${PROJECT_NAME} PRIVATE TcpClient)

target_include_directories(${PROJECT_NAME} PUBLIC ${ProcSignalGuard_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${ProcSignalGuard_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ProcSignalGuard_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	)




