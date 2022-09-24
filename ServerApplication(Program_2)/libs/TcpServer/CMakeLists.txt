project(TcpServer)

set(HEADERS include/TcpServer.hpp)
set(SOURCES src/ClientSessionGroup.hpp src/ClientSessionGroup.cpp src/TcpServer.cpp src/ClientSession.hpp src/ClientSession.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Boost 1.76.0 COMPONENTS system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)

target_link_libraries(${PROJECT_NAME} PUBLIC Packet)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )

target_include_directories(${PROJECT_NAME}
        PRIVATE ${PROJECT_SOURCE_DIR}/src
        )









