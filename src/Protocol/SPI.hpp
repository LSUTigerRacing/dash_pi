#ifndef SPI_HPP
#define SPI_HPP


#include <cstdint>
#include <string>
#include <vector>

class SPIDevice{
    private:
        int file_descriptor;
        std::string file_path;
        uint32_t clk_Speed;
        uint8_t bits;

    public:
    SPIDevice(std:: string file_path, uint32_t clk_Speed, uint8_t mode, const uint8_t bits, bool read, bool write);
    ~SPIDevice();
    int SPI_Read(uint8_t reg, size_t len);
    int SPI_Write(const uint8_t* data, size_t len);
    int Get_FD();
    uint32_t Get_Clk();
};

#endif