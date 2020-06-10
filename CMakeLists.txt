# cmake_minimum_required(VERSION <specify CMake version here>)
project(QEnergyLoggerDecoder_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(QEnergyLoggerDecoder_master
        src/eldatahandler.cpp
        src/eldatahandler.h
        src/elfileparser.cpp
        src/elfileparser.h
        src/main.cpp
        src/qenergyloggerdecoder.cpp
        src/qenergyloggerdecoder.h)
