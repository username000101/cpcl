cmake_minimum_required(VERSION 3.30)
project(CPL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cpcl INTERFACE)
target_include_directories(cpcl INTERFACE "include")