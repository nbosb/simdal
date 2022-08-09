
if(NOT TARGET gtest OR NOT TARGET gmock)
    if(NOT EXISTS ${GTEST_HOME})
        set(GTEST_HOME $ENV{GTEST_HOME})
    endif()

    if(NOT EXISTS ${GTEST_HOME})
        find_package(Git REQUIRED)
        set(GTEST_HOME ${CMAKE_CURRENT_BINARY_DIR}/googletest)
        set(GTEST_REPO "https://github.com/google/googletest/")
        set(GTEST_TAG "release-1.12.1")
        message(STATUS "Fetching googletest from ${GTEST_REPO}")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone ${GTEST_REPO} ${GTEST_HOME}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        ERROR_QUIET)

        if(EXISTS "${GTEST_HOME}/CMakeLists.txt")
            set(BUILD_GMOCK ON) 
            set(INSTALL_GTEST OFF)

            add_subdirectory(${GTEST_HOME} googletest EXCLUDE_FROM_ALL)
        endif()
    endif()
endif()
