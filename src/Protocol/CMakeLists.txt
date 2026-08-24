add_library(Protocol SPI.cpp ScreenGPIO.cpp)
target_include_directories(Protocol SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})