cmake_minimum_required(VERSION 3.29)
project(kubernetrion C)

set(CMAKE_C_STANDARD 23)

add_executable(kubernetrion main.c file.h file.c directory.h directory.c process.h process.c processor.h
        processor.c round_robin.h round_robin.c
)
