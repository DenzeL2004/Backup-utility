cmake_minimum_required(VERSION 3.17)

project(BackupUtils CXX)

include_directories(include)
add_compile_options(-std=c++20)


add_executable(my_backup utils/my_backup.cpp src/argument_parser.cpp)

add_subdirectory(tests)