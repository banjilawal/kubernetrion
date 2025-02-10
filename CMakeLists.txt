cmake_minimum_required(VERSION 3.29)
project(kubernetrion C)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(CMAKE_C_STANDARD 23)

add_executable(kubernetrion
        include/file.h
        src/file/file.c
        include/directory.h
        src/directory/directory.c
        include/process.h
        src/process/process.c
        include/processor.h
        src/processor/processor.c
        include/process_queue.h src/queue/process_queue.c
        compute.c
        src/process/processGenerator.c
        src/compute/processTable.c
        include/processTable.h
)
