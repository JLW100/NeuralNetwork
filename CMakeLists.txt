cmake_minimum_required(VERSION 3.16)
project(matrix_calculate C)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(source SRC_LIST)
include_directories(include)

LINK_LIBRARIES(m)

add_executable(main ${SRC_LIST})