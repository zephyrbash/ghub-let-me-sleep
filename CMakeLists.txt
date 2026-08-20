cmake_minimum_required(VERSION 4.3)
project(GHubLetMeSleep C)

set(CMAKE_C_STANDARD 11)

add_executable(GHubLetMeSleep main.c)

target_link_libraries(GHubLetMeSleep PRIVATE PowrProf)
