cmake_minimum_required(VERSION 3.29)
project(JugaadLang)

set(CMAKE_CXX_STANDARD 20)

add_executable(JugaadLang src/main.cpp
        src/tokenization.hpp
        src/parser.hpp
        src/generation.hpp)
