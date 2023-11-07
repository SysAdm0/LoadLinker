cmake_minimum_required(VERSION 3.25)
project(nginx)

include_directories(./include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

file(GLOB_RECURSE NGINX-AGENT "src/agent/*.cpp" "src/config/*.cpp")
file(GLOB_RECURSE NGINX-SERVER "src/server/*.cpp" "src/config/*.cpp")

add_executable(nginx-agent ${NGINX-AGENT})
add_executable(nginx-server ${NGINX-SERVER})
