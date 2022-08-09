cmake_minimum_required(VERSION 3.6)

project(sqlite)

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(sqlite3 ${src}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${inc})
target_link_libraries(sqlite3 PRIVATE pthread dl)

add_executable(sqlite3_shell ${src}/shell.c)
target_include_directories(sqlite3_shell PRIVATE ${inc})
target_link_libraries(sqlite3_shell PRIVATE sqlite3)

install(TARGETS sqlite3)
install(TARGETS sqlite3_shell)
install(DIRECTORY ${inc}/ DESTINATION include)
