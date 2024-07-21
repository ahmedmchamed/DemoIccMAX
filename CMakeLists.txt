cmake_minimum_required(VERSION 3.20)

project(BasicICCDemo
        VERSION 0.1)

function(add_source_directories)

    cmake_parse_arguments(
            PARSE_ARGV 0
            ARG
            "" "CMP_ENTRY_POINTS" "SOURCE_DIRECTORIES"
    )

    foreach (source ${ARG_SOURCE_DIRECTORIES})
        message(${source})
        add_subdirectory(${source})
    endforeach ()

endfunction()

# Setting the necessary compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_source_directories(
    SOURCE_DIRECTORIES
        "${PROJECT_SOURCE_DIR}/src/Utilities"
        "${PROJECT_SOURCE_DIR}/src/Components/IccRoundTrip"
)