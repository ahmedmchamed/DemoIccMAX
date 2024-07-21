file(GLOB_RECURSE sources_list
        ${CMAKE_CURRENT_LIST_DIR}/Private/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Public/*.h)

set(LIBRARY_NAME Utilities)

add_library(${LIBRARY_NAME} STATIC ${sources_list})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Private/)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public/)
