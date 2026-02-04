cmake_minimum_required(VERSION 3.12.4)

project(dash)

set(CONFIG_LV_BUILD_DEMOS ON)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(lvgl)
add_subdirectory(src)
add_executable (racecar src/main.cpp)

target_link_libraries(racecar PRIVATE lvgl   Protocol)
message("exit")
