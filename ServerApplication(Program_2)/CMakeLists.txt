project(ServerApplication)

set(HEADERS include/ServerAppConfig.hpp include/ServerApplication.hpp)
set(SOURCES src/main.cpp src/ServerAppConfig.cpp src/ServerApplication.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

find_package(Boost 1.76.0 COMPONENTS system log REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system Boost::log_setup Boost::log)

target_link_libraries(${PROJECT_NAME} PRIVATE ProcSignalGuard)

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/TcpServer)
target_link_libraries(${PROJECT_NAME} PRIVATE TcpServer)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

