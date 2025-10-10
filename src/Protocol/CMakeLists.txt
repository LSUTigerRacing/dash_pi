add_library(Protocol SPI.cpp)

target_include_directories(Protocol SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})