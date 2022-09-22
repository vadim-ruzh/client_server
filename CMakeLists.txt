cmake_minimum_required(VERSION 3.23)

project(ClientAndServer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


find_package(Boost 1.76.0)
if(NOT Boost_FOUND)
	# Download automatically, you can also just copy the conan.cmake file
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
		message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
		file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
	endif()
	include(${CMAKE_BINARY_DIR}/conan.cmake)
	conan_cmake_run(REQUIRES boost/1.76.0@conan/stable BASIC_SETUP BUILD missing)
endif()

add_subdirectory(Message)
set(message_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/message/include)
set(message_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(message_LIBRARIES Message)

add_subdirectory(ProcSignalGuard)
set(ProcSignalGuard_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ProcSignalGuard/include)
set(ProcSignalGuard_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(ProcSignalGuard_LIBRARIES ProcSignalGuard)

add_subdirectory(ServerApplication)
add_subdirectory(ClientApplication)


