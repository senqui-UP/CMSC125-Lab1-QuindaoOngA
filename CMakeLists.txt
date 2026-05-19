cmake_minimum_required(VERSION 4.1)
project(CMSC125_LAB1 C)

set(CMAKE_C_STANDARD 11)

add_executable(CMSC125_LAB1
        main.c
        parser.c
        executor.c
        builtins.c
        shell.h)