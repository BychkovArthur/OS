cmake_minimum_required(VERSION 3.10)
project(lab5-7)

# Задание стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Установка флагов для компиляции
add_compile_options(-g -rdynamic -Wall -Werror -Wextra -Wpedantic)

# Настройка переменных для папок
set(INCLUDE_DIR include)
set(SOURCE_DIR src)

# Подключение директории с заголовочными файлами
include_directories(${INCLUDE_DIR})

# Переменные с исходными файлами
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SOURCE_DIR}/functions.cpp)

# Делаем библиотеку из наших исходников
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCES})


#---------------main.cpp------------------

# Делаем исполняемый файл, в котором просто будем спрашивать пользователя входные данные
add_executable(server_exe ${SOURCE_DIR}/server.cpp)
add_executable(worker_exe ${SOURCE_DIR}/worker.cpp)

# "Привязываем" (линкуем) бинарник main.cpp с библиотекой, в которой расположена функция
# find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq)
target_link_libraries(server_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib cppzmq)
target_link_libraries(worker_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib cppzmq)
