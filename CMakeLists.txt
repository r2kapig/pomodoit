cmake_minimum_required(VERSION 3.0)

project(argparse VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_C_FLAGS)
	set(CMAKE_C_FLAGS "-O3")
endif()
if(NOT CMAKE_C_FLAGS_DEBUG)
	