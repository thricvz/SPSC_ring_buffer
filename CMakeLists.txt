cmake_minimum_required(VERSION 3.26)
project("Memory allocator from scratch")

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include/)

add_subdirectory(commands/)
add_executable(main main.cpp)

target_link_libraries(main commands)


