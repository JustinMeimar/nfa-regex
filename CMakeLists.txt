cmake_minimum_required(VERSION 3.10)

project(nfa-regex)

include_directories(
    "./include"
)

set (btree_src_files
    "./src/main.cpp"
    "./src/Node.cpp"
    "./src/Parser.cpp"
    "./src/Token.cpp"
    
)  

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(regex ${btree_src_files})