cmake_minimum_required(VERSION 3.10)
project(lab1)

set(INCLUDE_DIR include)
set(SOURCE_DIR src)

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE CHILD LIST_DIRECTORIES false ${SOURCE_DIR}/child.c)
file(GLOB_RECURSE PARENT LIST_DIRECTORIES false ${SOURCE_DIR}/parent.c)

add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_DIR}/functions.c)

# --------------------PARENT-------------------------
add_executable(parent_exe ${PARENT})
target_link_libraries(parent_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)

# -------------------CHILD---------------------------
add_executable(child_exe ${CHILD})
target_link_libraries(child_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)