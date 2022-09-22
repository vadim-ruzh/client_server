cmake_minimum_required(VERSION 3.23)

project(TcpServer)

set(HEADERS include/СlientSessionGroup.hpp include/TcpServer.hpp include/ClientSession.hpp)
set(SOURCES src/СlientSessionGroup.cpp src/TcpServer.cpp src/ClientSession.cpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 COMPONENTS system thread REQUIRED)
if(Boost_FOUND)

	add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
	target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})


	target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/message/include)
	target_link_libraries(${PROJECT_NAME} PUBLIC message)

	target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		)
else()
	message(FATAL_ERROR "Boost not found")
endif()









