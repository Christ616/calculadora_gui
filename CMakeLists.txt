cmake_minimum_required(VERSION 3.20)
project(calc_app C)

set(CMAKE_C_STANDARD 99)

# Buscar GTK+ y ATK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(ATK REQUIRED atk-1.0)

# Incluir las cabeceras
include_directories(${GTK3_INCLUDE_DIRS} /opt/homebrew/include/atk-1.0)
link_directories(${GTK3_LIBRARY_DIRS} /opt/homebrew/lib)

# Evitar advertencias
add_definitions(${GTK3_CFLAGS_OTHER})

# Agregar el ejecutable primero
add_executable(calc calc.tab.c calc.lex.c calcfuncs.c gui_calc.c)

# Ahora, después de crear el ejecutable, vinculamos las bibliotecas
target_link_libraries(calc PUBLIC ${GTK3_LIBRARIES} atk-1.0 m)