#include "Protocol/SPI.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../lv_conf.h"
#include <chrono>
#include <thread>
#include "../lvgl/examples/lv_examples.h"
#include "../lvgl/demos/lv_demos.h"
#include "../lv_drv_conf.h"
#include "../lvgl/src/display/lv_display.h"
#include "../lvgl/src/drivers/display/ili9341/lv_ili9341.h"
#include "../lvgl/src/stdlib/lv_mem.h"

#define HOR 240
#define VER 320

static SPIDevice teensy("/dev/spidev0.1",1000000,0,8);
static SPIDevice display("/dev/spidev0.0",1000000,0,8);

static uint32_t get_millisec(){
  auto clock = std::chrono::high_resolution_clock::now();
  auto duration = clock.time_since_epoch();
  auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return (uint32_t) millisec;  
}

static void cmdCallBack(lv_display_t *disp, const uint8_t *cmd, size_t cmd_size, const uint8_t *param, size_t param_size){
        write(display.getFD(), cmd,1);
}

static void lcd_send_color(lv_display_t * disp, const uint8_t *cmd, size_t cmd_size, uint8_t * param, size_t param_size){
        if(cmd && cmd_size > 0){
            write(display.getFD(),cmd,cmd_size);
        }
        
        if (param && param_size > 0){
            write(display.getFD(),param,param_size);
        }
        lv_display_flush_ready(disp);
}


int main(int argvc, char ** argv){

    lv_init();
    lv_tick_set_cb(get_millisec);
    lv_display_t *ili9341disp = lv_ili9341_create(HOR, VER,LV_LCD_FLAG_NONE , cmdCallBack,lcd_send_color);
    lv_display_set_rotation(ili9341disp, LV_DISPLAY_ROTATION_90);
    lv_demo_widgets();
    uint8_t buf1 = NULL;
    uint8_t buf2 = NULL;
    uint32_t buf_size = HOR * LCD_BUF_LINES * lv_color_format_get_size(lv_display_get_color_format(ili9341disp));
    buf1= lv_malloc(buf_size);
    
    if(buf1 == NULL){
        LV_LOG_ERROR("display draw buffer malloc failed");
        return;
    }
    
    lv_display_set_buffers(ili9341disp, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    ui_init(ili9341disp);
    
    while(1){
        uint32_t timer = lv_timer_handler();
        if(timer == LV_NO_TIMER_READY){
            timer = LV_DEF_REFR_PERIOD;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        lv_display_flush_ready(ili9341disp);
        /* std:: vector<uint8_t> buffer(64);
        ssize_t teensyData = read(teensy.getFD(),buffer.data(),sizeof(buffer));
        if(teensyData < 0 ){
            std::cout << "Failed read :(" << std::endl;
        }
        else{
        //ssize_t displayData = write(display.getFD(),&teensyData,sizeof(teensyData));
        if (displayData < 0 ){
            std::cout << "Failed write :(" << std::endl;
            }
        
        }*/
    }
}
