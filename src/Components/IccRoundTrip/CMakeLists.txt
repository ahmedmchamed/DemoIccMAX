file(GLOB_RECURSE sources_list
    ${CMAKE_CURRENT_LIST_DIR}/Private/IccRoundTrip.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Public/IccRoundTrip.h
)

set(LIBRARY_NAME IccRoundTrip)

add_library(${LIBRARY_NAME} ${sources_list})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public)

target_link_libraries(${LIBRARY_NAME} PUBLIC Utilities)