#include "Protocol/SPI.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../lv_conf.h"
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include "../lvgl/examples/lv_examples.h"
#include "../lvgl/demos/lv_demos.h"
#include "../lvgl/src/display/lv_display.h"
#include "../lvgl/src/drivers/display/ili9341/lv_ili9341.h"
#include "../lvgl/src/stdlib/lv_mem.h"
#include "ScreenGPIO.hpp"

#define HOR 240
#define VER 320
#define LCD_BUF_LINES 180

static ScreenGPIO display (16,18); //put actual pin numbers for reset and data pins

/**
 * @brief Callback for lvgl to send how many milliseconds has occurred
 * 
 * @return uint32_t returns how many milliseconds the program has been running for
 */
static uint32_t get_millisec(){
  auto clock = std::chrono::high_resolution_clock::now();
  auto duration = clock.time_since_epoch();
  auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return (uint32_t) millisec;  
}

/**
 * @brief Callback used by lvgl to send commands to the screen
 * 
 * @param disp Pointer to display
 * @param cmd  pointer to command
 * @param cmd_size size of the command
 * @param param pointer to paramters to command
 * @param param_size size of the parameter
 */
static void cmdCallBack(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size){
    display.Send_CMD(*cmd);

    if(param_size > 0){
        display.Send_Data(param,param_size);    
    }
}

/**
 *  @brief Callback used by lvgl to send color data to the display
 * 
 *  @param disp Pointer to display
 *  @param cmd Cmd array
 *  @param cmd_size size of cmd array 
 *  @param param array of parameter of color data
 *  @param param_size size of param array
 * 
 */
static void color_cb(lv_display_t * disp, const uint8_t *cmd, size_t cmd_size, uint8_t * param, size_t param_size){
    for (size_t i = 0; i < cmd_size; ++i) {
        display.Send_CMD(cmd[i]);
    }
    if (param_size > 0) {
        display.Send_Data(param, param_size);  
    }
}

/**
 *  @brief Custom flush call back that is currently not needed but might be needed later
 * 
 *  @param disp pointer to the display object
 *  @param area area of the screen
 *  @param px_map pixel map of the display
 * 
 */
void my_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map){
   // 1. Tell the display which rectangle of pixels we're about to send
    display.Send_CMD(0x2A); // Column Address Set (CASET)
    uint8_t col_data[4] = {
        static_cast <uint8_t>(area->x1 >> 8), static_cast <uint8_t>(area->x1 & 0xFF),
        static_cast< uint8_t>(area->x2 >> 8), static_cast <uint8_t>(area->x2 & 0xFF)
    };
    display.Send_Data(col_data, 4);

    display.Send_CMD(0x2B); // Row Address Set (RASET)
    uint8_t row_data[4] = {
        static_cast <uint8_t>(area->y1 >> 8), static_cast <uint8_t>(area->y1 & 0xFF),
        static_cast <uint8_t>(area->y2 >> 8), static_cast <uint8_t>(area->y2 & 0xFF)
    };
    display.Send_Data(row_data, 4);

    // 2. Tell it we're about to write pixel data into that window
    display.Send_CMD(0x2C); // Memory Write (RAMWR)

    // 3. Send the pixel buffer directly — already uint8_t*, no cast/sizeof needed
    uint32_t width = lv_area_get_width(area);
    uint32_t height = lv_area_get_height(area);
    uint32_t len = width * height * lv_color_format_get_size(lv_display_get_color_format(disp));
    display.Send_Data(px_map, len);

    lv_display_flush_ready(disp);
}

lv_display_t *ili9341disp = lv_ili9341_create(HOR, VER,LV_LCD_FLAG_NONE , cmdCallBack,color_cb);

int main(int argvc, char ** argv){
    
    lv_init();
    lv_tick_set_cb(get_millisec);
    lv_display_set_rotation(ili9341disp, LV_DISPLAY_ROTATION_90); //Makes the screen display horizontal
    uint8_t *buf1 = NULL;
    uint8_t *buf2 = NULL;
    uint32_t buf_size = HOR * LCD_BUF_LINES * lv_color_format_get_size(lv_display_get_color_format(ili9341disp));
    buf1 = (uint8_t*)lv_malloc(buf_size);
    buf2 = (uint8_t*)lv_malloc(buf_size); //for double buffering
      if(buf1 == NULL){
        LV_LOG_ERROR("display draw buffer malloc failed");
        return -1;
    }
    
    lv_display_set_buffers(ili9341disp, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    while(1){
        uint32_t timer = lv_timer_handler();
        if(timer == LV_NO_TIMER_READY){
            timer = LV_DEF_REFR_PERIOD;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        lv_display_flush_ready(ili9341disp);
    }
}
