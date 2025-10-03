#ifndef SPI_HPP
#define SPI_HPP


#include <cstdint>
#include <string>
#include <vector>

class SPIDevice{
    private:
        int file_descriptor;
        std:: string file_path;
        uint32_t clk_Speed;
        uint8_t bits;

    public:
    SPIDevice(std:: string file_path, uint32_t clk_Speed, uint8_t mode, const uint8_t bits, bool read, bool write);
    ~SPIDevice();
    std::vector<uint8_t> spiRead(uint8_t reg, size_t len);
    int getFD();
    uint32_t getClk();
};

#endif