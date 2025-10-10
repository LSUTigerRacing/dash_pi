cmake_minimum_required(VERSION 3.12.4)

project(dash)


set(lvglconf /home/Dash/Documents/dash_pi/lv_conf.h)
set(lv_driversconf /home/Dash/Documents/dash_pi/lv_drv_conf.h)
set(CONFIG_LV_BUILD_DEMOS ON)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(lvgl)
include_directories(src)
add_subdirectory(src/Protocol)
add_executable (racecar src/main.cpp)

target_link_libraries(racecar PRIVATE lvgl   Protocol)
message("exit")
