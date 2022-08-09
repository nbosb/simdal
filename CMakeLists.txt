cmake_minimum_required(VERSION 3.6)

project(simdal)

option(SIMDAL_BUILD_TESTS "Build SIMDAL unit tests" OFF)

add_compile_definitions(SQLITE_THREADSAFE=0) # disable mutexes in sqlite
add_compile_definitions(SQLITE_DEFAULT_MEMSTATUS=0) # disbale tracking of mem usage
add_compile_definitions(SQLITE_UNTESTABLE) # omit extra testing logic
add_subdirectory(deps/sqlite EXCLUDE_FROM_ALL)

set(src "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(inc "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(sources
    ${src}/simdal/utils.cpp
    ${src}/simdal/db/db.cpp
    ${src}/simdal/db/db_sqlite.cpp
    ${src}/simdal/db/db_csv.cpp
    ${src}/simdal/db_entry/entry_bp_hit.cpp
    ${src}/simdal/db_entry/entry_bp_hit_desc.cpp
    ${src}/simdal/db_entry/entry_call_stack.cpp
    ${src}/simdal/db_entry/entry_cout.cpp
    ${src}/simdal/db_entry/entry_event.cpp
    ${src}/simdal/db_entry/entry_exception.cpp
    ${src}/simdal/db_entry/entry_interrupt.cpp
    ${src}/simdal/db_entry/entry_quantum.cpp
    ${src}/simdal/db_entry/entry_sc_proc.cpp
    ${src}/simdal/db_entry/entry_sc_sched.cpp
    ${src}/simdal/db_entry/entry_sniffer.cpp
    ${src}/simdal/db_entry/entry_task_switch.cpp
    ${src}/simdal/db_entry/entry_transaction.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_irq.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_pci.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_sd_cmd.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_sd_data.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_spi.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_tlm.cpp
    ${src}/simdal/db_entry/entry_vcml_payload_virtio.cpp
    ${src}/simdal/db_entry/entry_target.cpp
    ${src}/simdal/db_entry/entry_wait_event.cpp
    ${src}/simdal/csv/csv_file.cpp)

add_library(simdal ${sources})

target_include_directories(simdal PUBLIC ${inc})
target_link_libraries(simdal PUBLIC sqlite3)

install(TARGETS simdal)
install(DIRECTORY ${inc}/ DESTINATION include)

if(SIMDAL_BUILD_TESTS)
    message("Building simdal tests")
    enable_testing()
    add_subdirectory(tests)
endif()
