cmake_minimum_required(VERSION 3.10)

project(regex)

include_directories("include")

file(GLOB SRC_FILES "src/*.cpp" )  


if (EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../web)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")
else() 
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
endif()

add_executable(regex ${SRC_FILES})