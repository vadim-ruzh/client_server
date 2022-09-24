project(Packet)

set(SOURCES src/Packet.cpp)
set(HEADERS include/Packet.hpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

find_package(Boost 1.76.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)


target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )

