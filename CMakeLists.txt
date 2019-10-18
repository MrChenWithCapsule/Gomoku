cmake_minimum_required(VERSION 3.8.0)
project(Gomoku)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
aux_source_directory(src SourceDir)
add_executable(Gomoku ${SourceDir})