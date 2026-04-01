cmake_minimum_required(VERSION 3.10)

project(DyAA-P5-Algoritmos-Aproximados)

file(GLOB_RECURSE SOURCES src/*.cc)

add_executable(MS-CFLP-CI-solver ${SOURCES})

target_include_directories(MS-CFLP-CI-solver PRIVATE include)
target_compile_features(MS-CFLP-CI-solver PRIVATE cxx_std_17)