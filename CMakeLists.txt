cmake_minimum_required(VERSION 3.20)

project(IccColourConversion
        VERSION 0.1)

# Somehow fixes the following DLLs not being found (when run from CMD)
# libstdc++6.dll, libwinpthread-1.dll
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static -lpthread")

# Setting the necessary compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# These subdirectories have their own cmake files that specify the lib's name, used in target_link_libraries() below
add_subdirectory("${PROJECT_SOURCE_DIR}/IccProfLib")
add_subdirectory("${PROJECT_SOURCE_DIR}/Utils")

# Create ConvertBulk exe and link all required code
add_executable(ConvertBulk ConvertBulk.cpp)
target_link_libraries(ConvertBulk PUBLIC IccProfLib Utils)

# Create Convert exe and link all required code
add_executable(ConvertSingle ConvertSingle.cpp)
target_link_libraries(ConvertSingle PUBLIC IccProfLib Utils)