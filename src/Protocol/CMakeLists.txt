add_library(SPI SPI.cpp)
add_library(UART uart.cpp)
target_include_directories(SPI PUBLIC .)
target_include_directories(UART PUBLIC .)
