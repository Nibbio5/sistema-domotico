cmake_minimum_required(VERSION 3.22)

project(sistema-domotico)
set(CMAKE_CXX_STANDARD  20)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native")

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB_RECURSE MY_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
set(SOURCES ${MY_SOURCE_FILES}  ${CMAKE_CURRENT_SOURCE_DIR}/include/*h)

add_executable(sistema-domotico ${MY_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/include)
