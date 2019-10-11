cmake_minimum_required(VERSION 3.8.0)
project(Gomoku)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. source_dict)
add_executable(Gomoku ${source_dict})