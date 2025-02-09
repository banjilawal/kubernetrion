cmake_minimum_required(VERSION 3.29)
project(kubernetrion C)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_C_STANDARD 23)

add_executable(kubernetrion include/file.h file.c include/directory.h directory.c include/process.h cmake-build-debug/src/process/process.c include/processor.h
        processor.c include/queue.h queue.c compute.c
        cmake-build-debug/src/process/processGenerator.c
)
