cmake_minimum_required(VERSION 3.22)
project(Test)

set(CMAKE_CXX_STANDARD 23)

add_executable(Test main.cpp Huffman.cpp Huffman.h)
