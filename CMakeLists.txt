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
include_directories(
    "vendor/FortSDK"
    "vendor/imgui"
    "vendor/minhook"
    "vendor/kiero"
)

# collect source files
set(SOURCES 
    "src/dllmain.cpp"
    "src/Abilities.cpp"
    "src/Globals.cpp"
    "src/Hooks.cpp"
    "src/Inventory.cpp"
    "src/Player.cpp"
    "src/Utils.cpp"
    "src/GUI.cpp"
    "src/ui/UIWindow.cpp"
    "src/ui/WatermarkWindow.cpp"
    "src/ui/CerberusMenu.cpp"
    "vendor/FortSDK/SDK/FortniteGame_functions.cpp" 
    "vendor/FortSDK/SDK/Basic.cpp" 
    "vendor/FortSDK/SDK/CoreUObject_functions.cpp"
    "vendor/FortSDK/SDK/Engine_functions.cpp"
    "vendor/FortSDK/SDK/GameplayAbilities_functions.cpp"
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/imgui_tables.cpp"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imgui_impl_dx11.cpp"
    "vendor/imgui/imgui_impl_win32.cpp"
    "vendor/imgui/imgui_stdlib.cpp"
    "vendor/kiero/kiero.cpp"
)

# build shared library (DLL on windows, .so on linux)
add_library(Cerberus SHARED ${SOURCES})

# optional: set output name for different platforms
set_target_properties(Cerberus PROPERTIES
    OUTPUT_NAME "cerberus"
)

# link MinHook static library
target_link_libraries(Cerberus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/minhook/libMinHook.x64.lib)