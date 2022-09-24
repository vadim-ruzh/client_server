project(TcpClient)

set(HEADERS include/TcpClient.hpp)
set(SOURCES src/TcpClient.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Boost 1.76.0 COMPONENTS system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)

target_link_libraries(${PROJECT_NAME} PUBLIC Packet)

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)



