cmake_minimum_required(VERSION 3.17)

project(BackupUtils CXX)

SET(BACKUP_SRC src/backup)
SET(BACKUP_INC include/backup)

include_directories(${BACKUP_INC})
add_compile_options(-std=c++20)


add_executable(my_backup utils/my_backup.cpp ${BACKUP_SRC}/argument_parser.cpp ${BACKUP_SRC}/backup.cpp)

add_subdirectory(tests)