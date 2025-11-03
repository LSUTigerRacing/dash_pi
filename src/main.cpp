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
#include "../lv_drv_conf.h"
#include "../lvgl/src/display/lv_display.h"
#include "../lvgl/src/drivers/display/ili9341/lv_ili9341.h"
#include "../lvgl/src/stdlib/lv_mem.h"

#define HOR 240
#define VER 320
#define LCD_BUF_LINES 180


//Gets the time for the timer in LVGL to tell how long to wait to call the call back
static uint32_t get_millisec(){
  auto clock = std::chrono::high_resolution_clock::now();
  auto duration = clock.time_since_epoch();
  auto millisec = std::chrono::duration_cast<chrono::milliseconds>(duration).count();
  return (uint32_t) millisec;  
}

//Sends commands with its parameter to the display
static void cmdCallBack(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size){
    vector<uint8_t> cmdBuf;
    cmdBuf.insert(cmdBuf.end(),cmd,cmd + cmd_size);
    cmdBuf.insert(cmdBuf.end(), cmd, cmd + cmd_size);
    write(display.getFD(),cmdBuf.data(),cmd_size + param_size);
}

static void color_cb(lv_display_t * disp, const uint8_t *cmd, size_t cmd_size, uint8_t * param, size_t param_size){
        if(cmd && cmd_size > 0){
            write(display.getFD(),cmd,cmd_size);
        }
        
        if (param && param_size > 0){
            write(display.getFD(),param,param_size);
        }
        lv_display_flush_ready(disp);
}
Draws content to the screen after the Pi has fully rendered them in
void my_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map){
    uint16_t * buf16 = (uint16_t*)px_map;
    int32_t x,y;
    for(y = area->y1; y <= area->y2; y++){
        for(x = area->x2;x <= area->x2; x++){
            write(display.getFD(),buf16,x);
            write(display.getFD(),buf16,y);
            buf16++;
            
        }
    }
    lv_display_flush_ready(disp);
}

static SPIDevice display ("/dev/spidev0.0",1000000,0,8,true,false);
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
        return;
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
