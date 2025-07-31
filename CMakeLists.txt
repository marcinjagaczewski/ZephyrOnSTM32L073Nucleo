cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(L073RZ C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(inc)

target_link_libraries(app PRIVATE stdc++)

target_sources(app PRIVATE 
    src/main.cpp 
    src/gpio.cpp 
    src/threads.cpp
)
target_compile_options(app PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
)
