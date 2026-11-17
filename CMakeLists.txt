cmake_minimum_required(VERSION 3.20)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(stft src/main.cpp
    src/stft.cpp)
target_compile_options(stft PRIVATE -Wall -Wextra)
target_include_directories(stft PRIVATE include)
