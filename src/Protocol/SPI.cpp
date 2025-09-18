#include "SPI.hpp"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <linux/types.h>
#include <system_error>

SPIDevice::SPIDevice(const std::string file_path, const uint32_t clk_Speed, const uint8_t mode, const uint8_t bits): file_path(file_path),clk_Speed(clk_Speed), mode(mode),bits(bits),file_descriptor(-1) {
    file_descriptor = open(file_path.c_str(), O_RDWR);
    if (file_descriptor < 0)
    {
        throw std::system_error();
    }
    
}

SPIDevice::~SPIDevice(){
    if (file_descriptor > 0){
        close(file_descriptor);
    }
}

int SPIDevice:: getFD(){
    return file_descriptor;
}