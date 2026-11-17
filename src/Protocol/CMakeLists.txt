add_library(Protocol SPI.cpp ScreenGPIO.cpp uart.cpp)
target_include_directories(Protocol SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Protocol PUBLIC gpiodcxx) #additional necessary line
