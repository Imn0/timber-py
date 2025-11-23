cmake_minimum_required(VERSION 3.15)
project(timber_python)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/timber-c/.git")
    message(STATUS "Initializing timber-c submodule...")
    execute_process(
        COMMAND git submodule update --init --recursive timber-c
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMODULE_RESULT
    )
    if(NOT GIT_SUBMODULE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to initialize timber-c submodule")
    endif()
endif()
add_subdirectory(timber-c)

set_target_properties(timber-static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

Python_add_library(_timber MODULE
    timber/_timber.c
    WITH_SOABI
)

target_link_libraries(_timber PRIVATE timber-static)

target_include_directories(_timber PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/timber-c/include
)

set_target_properties(_timber PROPERTIES
    OUTPUT_NAME "_timber"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

if(CMAKE_CONFIGURATION_TYPES)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${config} config_upper)
        set_target_properties(_timber PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
            RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        )
    endforeach()
else()
    set_target_properties(_timber PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    )
endif()
