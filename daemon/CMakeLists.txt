cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(ned) # Название проекта

set(SOURCE_EXE main.c) # Установка переменной со списком исходников для исполняемого файла

include(FindPCAP.cmake)
set(SOURCE_LIB daemonization.c)
set(SOURCE_NDSERVER_LIB ndsrv.c)
set(SOURCE_SNIFFER_LIB sniffer.c)
set(SOURCE_IPSTORE_LIB ipstore.c)
find_library(PTHREAD_LIBRARY pthread)

add_library(daemonize STATIC ${SOURCE_LIB})
add_library(ndsrv STATIC ${SOURCE_NDSERVER_LIB})
add_library(sniffer STATIC ${SOURCE_SNIFFER_LIB})
add_library(ipstore STATIC ${SOURCE_IPSTORE_LIB})

add_executable(ned ${SOURCE_EXE}) # Создает исполняемый файл с именем main

target_link_libraries(sniffer ${PCAP_LIBRARY} ipstore)
target_link_libraries(ndsrv sniffer ipstore)	
target_link_libraries(ned daemonize ndsrv ${PTHREAD_LIBRARY})	
