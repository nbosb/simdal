
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GoogleTest REQUIRED)

set(src "${CMAKE_CURRENT_SOURCE_DIR}")
set(inc "${CMAKE_CURRENT_SOURCE_DIR}")

macro(add_simdal_test test)
    add_executable(${test} ${src}/${test}.cpp)
    target_include_directories(${test} PRIVATE ${inc})
    target_link_libraries(${test} simdal_testing)
    add_test(NAME ${test} COMMAND ${test} ${CMAKE_CURRENT_SOURCE_DIR})
    set_tests_properties(${test} PROPERTIES TIMEOUT 5)
    install(TARGETS ${test} DESTINATION tests/)
endmacro()

add_library(simdal_testing ${src}/testing.cpp)
target_include_directories(simdal_testing PRIVATE ${inc})
target_link_libraries(simdal_testing PUBLIC simdal gtest gmock)

add_simdal_test(db_test)
add_simdal_test(csv_file_test)
