#include "SPI.hpp"
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>
#include <linux/types.h>
#include <system_error>

SPIDevice::SPIDevice(const std::string file_path, const uint32_t clk_Speed, const uint8_t mode, const uint8_t bits,bool read, bool write): file_path(file_path),file_descriptor(-1), clk_Speed(clk_Speed),bits(bits) {
    //file path should be /dev/spidevB.C B is the bus while C is the chip select for that device
    //when opening it gives access to reading and writing permissions to that file
    file_descriptor = open(file_path.c_str(), O_RDWR);
    if (file_descriptor < 0)
    {
        throw std::system_error();
    }
    //depending on what is needed for the device enable read and/or write for the device for this project each device only has one job and doesn't need both
    if(read){
        ioctl(file_descriptor, SPI_IOC_RD_MODE,SPI_MODE_0);
        ioctl(file_descriptor, SPI_IOC_RD_MAX_SPEED_HZ,&clk_Speed);
        ioctl(file_descriptor, SPI_IOC_RD_BITS_PER_WORD, &bits);
    }
    if(write){
        ioctl(file_descriptor, SPI_IOC_WR_MODE, SPI_MODE_0);
        ioctl(file_descriptor, SPI_IOC_WR_MAX_SPEED_HZ,&clk_Speed);
        ioctl(file_descriptor,SPI_IOC_WR_BITS_PER_WORD,&bits);
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
//ioctl wrapper for spi reading you cannot use the normal read() function becasue in SPI you need to send dummy data to be able to read data
// this is because since the master calls when to read it sends a serial clock pulse to notify the device to send the data
std::vector<uint8_t> SPIDevice::spiRead(uint8_t reg, size_t len){
    //dummy data to send the peripheral device to initiate the data transfer
    std::vector<uint8_t> txData(len + 1, 0x00);
    std::vector<uint8_t> rxData (txData.size(),0);
    //setting the values to 0 just uses the default that was set by the constructor
    struct spi_ioc_transfer spiTran = {};
        spiTran.rx_buf = reinterpret_cast<unsigned long>(rxData.data());
        spiTran.tx_buf = reinterpret_cast<unsigned long>(txData.data());
        spiTran.len = txData.size();
        spiTran.speed_hz = 0;
        spiTran.delay_usecs = 0;
        spiTran.bits_per_word = 0;
        spiTran.cs_change = true;
    //pulls in data and puts it into the rxData vector
    ioctl(file_descriptor,SPI_IOC_MESSAGE(1), &spiTran);
    //returns the rxData vector except the first element because it is always nothing 0x00 
    return std::vector<uint8_t>(rxData.begin() + 1,rxData.end());    
}