cmake_minimum_required(VERSION 3.15)
project(timber_python)

find_package(Python COMPONENTS Interpreter Development REQUIRED)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)

add_subdirectory(timber-c)

set_target_properties(timber-static PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

Python_add_library(_timber MODULE
    timber/_timber.c
)

target_link_libraries(_timber PRIVATE timber-static)

target_include_directories(_timber PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/timber-c/include
)

set_target_properties(_timber PROPERTIES
    OUTPUT_NAME "_timber"
)

if(DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set_target_properties(_timber PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    )
endif()
