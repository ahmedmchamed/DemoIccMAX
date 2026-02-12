cmake_minimum_required(VERSION 3.20)

project(BasicICCDemo
        VERSION 0.1)

# Somehow fixes the following DLLs not being found (when run from CMD)
# libstdc++6.dll, libwinpthread-1.dll
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")

# Setting the necessary compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EntryPoint entrypoint.cpp)

add_subdirectory("${PROJECT_SOURCE_DIR}/IccProfLib")
target_link_libraries(EntryPoint PUBLIC IccProfLib)

add_subdirectory("${PROJECT_SOURCE_DIR}/Utils")
target_link_libraries(EntryPoint PUBLIC Utils)