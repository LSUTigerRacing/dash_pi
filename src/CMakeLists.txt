if(NOT DESKTOP_MODE)
  add_subdirectory(Protocol)
endif()

add_subdirectory(ui)
#Modified by Anthony Meiers - Commented out lines cause compilation errors due to duplicated src main.cpp main builds. 
# add_library(src main.cpp) 