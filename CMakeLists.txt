cmake_minimum_required(VERSION 3.25)
project(nginx)

include_directories(./include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE LOADLINKER "src/server/*.cpp" "src/config/*.cpp")
file(GLOB_RECURSE LOADLINKERAGENT "src/agent/*.cpp" "src/config/*.cpp")

add_executable(LoadLinker ${LOADLINKER})
add_executable(LoadLinkerAgent ${LOADLINKERAGENT})
