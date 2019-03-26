cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(ned) # Название проекта

set(SOURCE_EXE main.c) # Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB daemonization.c) # Тоже самое, но для библиотеки

add_library(daemonize STATIC ${SOURCE_LIB}) # Создание статической библиотеки с именем foo

add_executable(main ${SOURCE_EXE}) # Создает исполняемый файл с именем main

target_link_libraries(main daemonize)	
