cmake_minimum_required(VERSION 3.10)

# project name and version
project(Cerberus VERSION 1.0)

# tell CMake to use C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# setting to unicode character set in Visual Studio
add_definitions(-DUNICODE -D_UNICODE)

# making sure /bigobj is specified
add_compile_options(/bigobj)

# include directory
include_directories(include)

# collect source files
set(SOURCES 
    "src/dllmain.cpp" 
    "include/FortSDK/SDK/FortniteGame_functions.cpp" 
    "include/FortSDK/SDK/Basic.cpp" 
    "include/FortSDK/SDK/CoreUObject_functions.cpp"
    "include/FortSDK/SDK/Engine_functions.cpp"
    "include/FortSDK/SDK/GameplayAbilities_functions.cpp"
)

# build shared library (DLL on windows, .so on linux)
add_library(Cerberus SHARED ${SOURCES})

# optional: set output name for different platforms
set_target_properties(Cerberus PROPERTIES
    OUTPUT_NAME "cerberus"
)

# link MinHook static library
target_link_libraries(Cerberus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libMinHook.x64.lib)