//Modified by Anthony Meiers, definitions from ScreenGPIO.hpp are redefined below, causing compilation errors.
//Also changed additional lines to fix mistakes in the code that were causing additional compilation errors.
#include "ScreenGPIO.hpp"
#include <chrono>
#include <thread>
#include "SPI.hpp"

//copied from adafruit ili9341 library
static uint8_t initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};

//class ScreenGPIO{ //Anthony Meiers - commented out lines are redefinitions from ScreenGPIO.hpp.
//    private:
//        gpiod::chip chipName;
//        gpiod::line_request rstPin;
//        gpiod::line_request dataPin;

//        uint8_t rst_offset;
//        uint8_t data_offset;

//        SPIDevice display;

//    public:
        ScreenGPIO::ScreenGPIO(uint8_t rst_offset, uint8_t data_offset):rst_offset(rst_offset), data_offset(data_offset),chipName("/dev/gpiochip0"),
        rstPin(chipName.prepare_request().set_consumer("screen").add_line_settings(rst_offset,gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT)).do_request()),
        dataPin(chipName.prepare_request().set_consumer("screen").add_line_settings(data_offset,gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT)).do_request()),
        display("/dev/spidev0.1",1000000,0,8,true,false) // Anthony Meiers - changed "spidev0.0" to "spidev0.1" to fix a compilation error.
        {
          Reset_Screen();
          Send_Data(initcmd,111);
        }

        ScreenGPIO::~ScreenGPIO(){
        //  display.~SPIDevice(); //Anthony Meiers - Unnecessary destructor call, could cause errors as C++ automatically destroys objects this would destroy.
        }

        //Resetting the screen just requires turning the pin off briefly and turning it back on
        void ScreenGPIO::Reset_Screen(){
          rstPin.set_value(rst_offset,gpiod::line::value::INACTIVE);
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          rstPin.set_value(rst_offset,gpiod::line::value::ACTIVE);
          return;
        }

        int ScreenGPIO::Send_CMD(char cmd){
          dataPin.set_value(data_offset,gpiod::line::value::INACTIVE);//Data pin low means sending commands
          return display.SPI_Write(reinterpret_cast<uint8_t*> (cmd),1);  
        }

        int ScreenGPIO::Send_Data(const uint8_t* data, size_t len){ //Anthony Meiers - declaration missing "ScreenGPIO::", causing errors when compiling.
          dataPin.set_value(data_offset,gpiod::line::value::ACTIVE); //Data pin high means sending data
          return display.SPI_Write(data,len);
        }
};