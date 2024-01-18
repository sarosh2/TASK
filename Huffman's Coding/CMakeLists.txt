cmake_minimum_required(VERSION 3.17)
project(Huffman_s_Coding)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_executable(Huffman_s_Coding main.cpp TextCompressor.cpp MinHeap.cpp HashTable.cpp Queue.cpp TextDecompressor.cpp)