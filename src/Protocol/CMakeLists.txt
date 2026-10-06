#File Modified by Anthony Meiers, gpiodxcc library was not linked in this file which caused a bunch of errors.
add_library(Protocol SPI.cpp ScreenGPIO.cpp uart.cpp)
target_include_directories(Protocol SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Protocol PUBLIc gpiodxcc) #Anthony Meiers - gpiodxcc library linker