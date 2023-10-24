cmake_minimum_required(VERSION 3.20)

project(charvec)

add_compile_options(-Wall -Wpedantic -Wextra -ggdb)

add_executable(test_string test_string.c string.c)
