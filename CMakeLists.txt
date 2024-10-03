cmake_minimum_required(VERSION 3.29)
project(FusionLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(FusionLang src/main.cpp
        src/tokenization.hpp
        src/parser.hpp
        src/generation.hpp
        src/tokenization.hpp
        src/generation.hpp)
