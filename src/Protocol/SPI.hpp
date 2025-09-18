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
        uint8_t mode;
        uint8_t bits;

    public:
    SPIDevice(std:: string file_path, uint32_t clk_Speed, uint8_t mode, const uint8_t bits);
    ~SPIDevice();
    ssize_t write(const std::vector<uint8_t>& rx );
    ssize_t read(const std::vector<uint8_t>& tx);
    bool setSpeed(uint32_t speed);
    bool setMode(uint8_t mode);
    bool setBits(uint8_t bits);
    int getFD();
};

#endif