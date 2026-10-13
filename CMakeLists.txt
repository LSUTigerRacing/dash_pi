
cmake_minimum_required(VERSION 3.12.4)

project(dash)

# Disable unnecessary LVGL demos and examples
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${CMAKE_SOURCE_DIR})

# LVGL graphics library
add_subdirectory(lvgl)

# Only build the dashboard UI, not Raspberry Pi hardware
add_subdirectory(src/ui)

# Desktop executable
add_executable(
    racecar
    src/main.cpp
)

# SDL2 window support
find_package(SDL2 REQUIRED)

target_link_libraries(racecar PRIVATE
    lvgl
    ui
    SDL2::SDL2
)

