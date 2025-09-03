#include "SPI.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main(){
    SPIDevice teensy("/dev/spidev0.0",1000000,0,8);
    SPIDevice display("/dev/spidev0.1",1000000,0,8);
    bool carON = true;
    while(carON){
        std:: vector<uint8_t> buffer(64);
        ssize_t teensyData = read(teensy.getFD(),buffer.data(),sizeof(buffer));
        if(teensyData < 0 ){
            std::cout << "Failed read :(" << std::endl;
        }
        else{
        ssize_t displayData = write(display.getFD(),&teensyData,sizeof(teensyData));
        if (displayData < 0 ){
            std::cout << "Failed write :(" << std::endl;
            }
        
        }
    }
}