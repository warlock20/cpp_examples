cmake_minimum_required(VERSION 3.20)
project(03_LoggerModule)

set(CMAKE_CXX_STANDARD 14)

include_directories("src/*.cpp")

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

