cmake_minimum_required(VERSION 3.8)
project(Compilador)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SOURCE_FILES main.c src/analisadorlexico.c include/analisadorlexico.h include/erro.h src/erro.c include/analisadorsintatico.h src/analisadorsintatico.c include/gerenciador.h src/gerenciador.c)
add_executable(Compilador ${SOURCE_FILES})