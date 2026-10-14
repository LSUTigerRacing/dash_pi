cmake_minimum_required(VERSION 3.12.4)

project(dash)

set(CONFIG_LV_BUILD_DEMOS ON)

# default option is protocol mode
option(DESKTOP_MODE "Enable desktop emulator" OFF)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(lvgl)
add_subdirectory(src)

add_executable(
    racecar
    src/main.cpp
)

if(DESKTOP_MODE)
  find_package(SDL2 REQUIRED)
  target_compile_definitions(racecar PRIVATE DESKTOP_MODE)
  target_compile_definitions(lvgl PUBLIC DESKTOP_MODE)
  target_link_libraries(racecar PRIVATE SDL2::SDL2)
else()
  target_link_libraries(racecar PRIVATE Protocol ui)
endif()

target_link_libraries(racecar PRIVATE lvgl ui)

message("exit")
