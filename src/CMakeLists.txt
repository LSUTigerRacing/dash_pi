add_library(src main.cpp)
add_subdirectory(Protocol)
target_include_directories(src SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})