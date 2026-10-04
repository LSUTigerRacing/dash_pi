#include <gpiod.hpp> 

//Copied from adafruit ili9341 library
#define ILI9341_PWCTR1 0xC0 ///< Power Control 1
#define ILI9341_PWCTR2 0xC1 ///< Power Control 2
#define ILI9341_VMCTR1 0xC5 ///< VCOM Control 1
#define ILI9341_VMCTR2 0xC7 ///< VCOM Control 2
#define ILI9341_MADCTL 0x36   ///< Memory Access Control
#define ILI9341_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
#define ILI9341_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set
#define ILI9341_FRMCTR1                                                        \
    0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_DFUNCTR 0xB6 ///< Display Function Control
#define ILI9341_GAMMASET 0x26 ///< Gamma Set
#define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
#define ILI9341_SLPOUT 0x11 ///< Sleep Out
#define ILI9341_DISPON 0x29   ///< Display ON

class ScreenGPIO{
    private:
        gpiod::chip chipName;
        gpiod::line_request rstPin;
        gpiod::line_request dataPin;

        static uint8_t rst_offset;
        static uint8_t data_offset;

    public:
        ScreenGPIO(uint8_t rst_offset, uint8_t data_offset);
        ~ScreenGPIO();
        void Reset_screen();
        int Send_CMD(char cmd);
        int Send_Data(const uint8_t* data,size_t len);

};
