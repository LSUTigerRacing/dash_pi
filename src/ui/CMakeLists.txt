add_library(ui ui.cpp)

target_include_directories(ui SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/lvgl
)
