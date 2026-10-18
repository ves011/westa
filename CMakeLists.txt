# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(MINIMAL_BUILD ON APPEND)
idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=esp_panic_handler" APPEND)

set(COMMON_REPO_DIR "${CMAKE_CURRENT_LIST_DIR}/esp32_common")

execute_process(
    COMMAND git -C "${COMMON_REPO_DIR}" describe --tags --long --dirty --always
    OUTPUT_VARIABLE COMMON_REPO_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    RESULT_VARIABLE GIT_DESCRIBE_RESULT
)

if(NOT GIT_DESCRIBE_RESULT EQUAL 0 OR COMMON_REPO_VERSION STREQUAL "")
    set(COMMON_REPO_VERSION "unknown")
endif()

project(westa)
