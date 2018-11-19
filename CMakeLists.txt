cmake_minimum_required(VERSION 3.12)
project(Kuznyechik)

set(CMAKE_CXX_STANDARD 17)

add_executable(Kuznyechik main.cpp s_conversion.h utils.h lr_conversion.h keys.h tests.h OMAC.h kuznyechick.h)