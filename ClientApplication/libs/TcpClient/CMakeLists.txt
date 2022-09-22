cmake_minimum_required(VERSION 3.23)

project(TcpClient)

set(HEADERS include/TcpClient.hpp)
set(SOURCES src/TcpClient.cpp)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.76.0 COMPONENTS system REQUIRED)
if(Boost_FOUND)
	add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
	target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

	target_include_directories(${PROJECT_NAME} PUBLIC ${message_INCLUDE_DIRS})
	target_link_directories(${PROJECT_NAME} PRIVATE ${message_LIBRARY_DIR})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${message_LIBRARIES})

	target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		)
else()
	message(FATAL_ERROR "Boost not found")
endif()



