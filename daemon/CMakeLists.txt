cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(ned) # Название проекта

set(SOURCE_EXE main.c) # Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB daemonization.c)
set(SOURCE_NDSERVER_LIB ndsrv.c)

add_library(daemonize STATIC ${SOURCE_LIB})
add_library(ndsrv STATIC ${SOURCE_NDSERVER_LIB})

add_executable(ned ${SOURCE_EXE}) # Создает исполняемый файл с именем main

target_link_libraries(ned daemonize ndsrv)	
