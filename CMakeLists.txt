cmake_minimum_required(VERSION 3.25)
project(TSP_Problem C)

set(CMAKE_C_STANDARD 11)

add_executable(TSP_Problem main.c GPX.h LK.h SA2OPT.h TSPUTILS.h VNS.h)
