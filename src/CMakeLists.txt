#Modified by Anthony Meiers - Commented out lines cause compilation errors due to duplicated src main.cpp main builds. 
# add_library(src main.cpp) 
add_subdirectory(Protocol)
add_subdirectory(ui)
# target_include_directories(
#    src
#    SYSTEM PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}
#)
