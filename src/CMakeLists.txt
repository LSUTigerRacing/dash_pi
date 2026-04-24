add_library(
    ui
    ui/ui.cpp
)
add_subdirectory(Protocol)
target_include_directories(
    ui
    SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lvgl
)
