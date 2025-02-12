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
        include/process_queue.h src/process/process_queue.c
        compute.c
        src/process/process_generator.c
        include/process_b_tree.h
        include/process_queue.h
        include/process_queue.h
        include/file_b_tree.h
        include/process_stats.h
        src/compute/compute.c
        include/process_generator.h
        include/graph_vertex.h
        src/graph_vertex.c
        src/processor/processor.c
)
