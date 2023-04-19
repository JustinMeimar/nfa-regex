cmake_minimum_required(VERSION 3.10)

project(regex)

include_directories("include")

file(GLOB SRC_FILES "src/*.cpp" )  

if (EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../web)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --bind")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPILE_WEB=1")
    add_executable(regex_web ${SRC_FILES})
else() 
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
    add_executable(regex ${SRC_FILES})
endif()
