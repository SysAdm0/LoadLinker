cmake_minimum_required(VERSION 3.25)
project(nginx)

include_directories(./include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++)

file(GLOB_RECURSE NGINX "src/*.cpp")

add_executable(nginx ${NGINX})