cmake_minimum_required(VERSION 3.23)
project(sdk)

set(CMAKE_CXX_STANDARD 23)

add_library(sdk SHARED library.cpp main.cpp common.h)
