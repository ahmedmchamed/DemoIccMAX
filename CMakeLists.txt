cmake_minimum_required(VERSION 3.20)

project(BasicICCDemo
        VERSION 0.1)

function(add_source_directories)
    add_executable(EntryPoint ${CMAKE_CURRENT_LIST_DIR}/src/entrypoint.cpp)

    cmake_parse_arguments(
        PARSE_ARGV 0
        ARG
        "" "TARGETS" "LIBRARY_DIRECTORIES"
    )

    foreach (library ${ARG_LIBRARY_DIRECTORIES})
        message(${library})
        add_subdirectory(${library})
    endforeach ()

    foreach (target ${ARG_TARGETS})
        message(${target})
        target_link_libraries(EntryPoint PUBLIC ${target})
    endforeach ()

endfunction()

# Setting the necessary compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the targets list - i.e. the requisite components
set(TARGETS_LIST
    Utilities)

add_source_directories(
    TARGETS
        "${TARGETS_LIST}"
    LIBRARY_DIRECTORIES
        "${PROJECT_SOURCE_DIR}/src/Utilities"
)