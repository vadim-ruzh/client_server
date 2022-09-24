project(ProcSignalGuard)

set(HEADERS include/ProcSignalGuard.hpp)
set(SOURCES src/ProcSignalGuard.cpp)

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include)