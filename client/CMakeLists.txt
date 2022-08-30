project(client)

add_executable(${PROJECT_NAME} main.cpp ConnectionResponsible.cpp ConnectionResponsible.hpp ClientApplication.cpp ClientApplication.hpp MyClient.cpp MyClient.hpp TcpClient.hpp)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.76.0 COMPONENTS system REQUIRED)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})