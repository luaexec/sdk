cmake_minimum_required(VERSION 3.23)
project(sdk)

set(CMAKE_CXX_STANDARD 23)

add_library(sdk SHARED library.cpp main.cpp common.h other/other.h other/types/color.h other/types/singleton.h other/types/utlvector.h other/types/vector.h other/types/vertex.h other/types/viewmatrix.h)
