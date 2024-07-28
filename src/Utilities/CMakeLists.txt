file(GLOB_RECURSE sources_list
        ${CMAKE_CURRENT_LIST_DIR}/IccUtility/Private/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IccUtility/Public/*.h
        ${CMAKE_CURRENT_LIST_DIR}/ProjectUtility/Private/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ProjectUtility/Public/*.h)

set(LIBRARY_NAME Utilities)

add_library(${LIBRARY_NAME} STATIC ${sources_list})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/IccUtility/Private/)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/IccUtility/Public/)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ProjectUtility/Private/)
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ProjectUtility/Public/)
