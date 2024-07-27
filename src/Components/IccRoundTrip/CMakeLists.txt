file(GLOB_RECURSE sources_list
        "${CMAKE_CURRENT_LIST_DIR}/CmpEntry/IccRoundTripCmpEntry.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/Private/IccRoundTrip.cpp"
)

set(LIBRARY_NAME IccRoundTrip)

add_library(${LIBRARY_NAME} ${sources_list}
        Public/CommandLineUtility.h
        Private/CommandLineUtility.cpp
        Public/IccRoundTripCmpEntry.h)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Public)

target_link_libraries(${LIBRARY_NAME} PUBLIC Utilities)