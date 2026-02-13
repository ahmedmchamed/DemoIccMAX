file(GLOB sources_list
        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/*.h)

set(LIBRARY_NAME IccProfLib)
add_library(${LIBRARY_NAME} STATIC ${sources_list})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})