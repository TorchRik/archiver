cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 20)

project(archiver VERSION 1.0)

configure_file(ArchiverConfig.h.in ArchiverConfig.h)

add_subdirectory(library)
add_subdirectory(tests)
find_library(archiver_library ./library)

add_executable(archiver archiver.cxx)

target_link_libraries(archiver archiver_library)

target_include_directories(archiver PUBLIC
        "${PROJECT_BINARY_DIR}"
        )