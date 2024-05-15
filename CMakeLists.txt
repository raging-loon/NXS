cmake_minimum_required(VERSION 3.10)

project(nxs VERSION 2.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)
add_executable(nxs
    src/main.cpp
    src/utils.cpp
    src/scanner.cpp
)
target_link_libraries(nxs PRIVATE "pthread")