cmake_minimum_required(VERSION 3.10)
project(tans)

set(CMAKE_CXX_STANDARD 11)


include_directories(
        .
        ~/root/include
)

add_executable(tans
        hist.cpp
        io.cpp
        compile.C
        main.cxx
        hist_readfile.cxx
        central.cpp
        )