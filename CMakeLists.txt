cmake_minimum_required(VERSION 3.25)
project(nginx)

include_directories(./include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

# Uncomment to enable static linking
# set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE LOADLINKER-SERVER "src/server/*.cpp" "src/config/*.cpp")
file(GLOB_RECURSE LOADLINKER-AGENT "src/agent/*.cpp" "src/config/*.cpp")

add_executable(loadlinker-server ${LOADLINKER-SERVER})
add_executable(loadlinker-agent ${LOADLINKER-AGENT})
