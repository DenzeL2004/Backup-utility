cmake_minimum_required(VERSION 3.17)

project(Utils CXX)

SET(BACKUP_SRC src/backup)
SET(BACKUP_INC include/backup)

SET(RESTORE_SRC src/restore)
SET(RESTORE_INC include/restore)

include_directories(${BACKUP_INC} ${RESTORE_INC} include)
add_compile_options(-std=c++20)


add_executable(my_backup utils/my_backup.cpp src/generals.cpp ${BACKUP_SRC}/argument_parser.cpp ${BACKUP_SRC}/backup.cpp)

add_executable(my_restore utils/my_restore.cpp src/generals.cpp ${RESTORE_SRC}/argument_parser.cpp ${RESTORE_SRC}/restore.cpp)

add_subdirectory(tests)