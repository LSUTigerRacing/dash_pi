/**
 * @file lvglScreen.cpp
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "../lv_conf.h"
#include "lvglScreen.hpp"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *main_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t lvglscreen;

    static bool style_inited = false;

    if (!style_inited)
    {
        lv_style_init(&lvglscreen);
        lv_style_set_bg_color(&lvglscreen, lv_color_hex(0x141313));

        style_inited = true;
    }

    lv_obj_t *lv_obj_0 = lv_obj_create(NULL);
    // lv_obj_set_name_static(lv_obj_0, "main_#");

    lv_obj_add_style(lv_obj_0, &lvglscreen, 0);
    lv_obj_t *lv_label_0 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_0, "RPM -");
    lv_obj_set_x(lv_label_0, 100);
    lv_obj_set_y(lv_label_0, 50);
    lv_obj_set_style_text_color(lv_label_0, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_1 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_1, "MPH -");
    lv_obj_set_x(lv_label_1, 200);
    lv_obj_set_y(lv_label_1, 50);
    lv_obj_set_style_text_color(lv_label_1, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_2 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_2, "Oil Pressure -");
    lv_obj_set_x(lv_label_2, 30);
    lv_obj_set_y(lv_label_2, 120);
    lv_obj_set_style_text_font(lv_label_2, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lv_label_2, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_3 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_3, "Oil temp -");
    lv_obj_set_x(lv_label_3, 115);
    lv_obj_set_y(lv_label_3, 120);
    lv_obj_set_style_text_font(lv_label_3, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lv_label_3, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_4 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_4, "Voltage -");
    lv_obj_set_x(lv_label_4, 200);
    lv_obj_set_y(lv_label_4, 120);
    lv_obj_set_style_text_font(lv_label_4, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lv_label_4, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_5 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_5, "Fuel Pressure -");
    lv_obj_set_x(lv_label_5, 30);
    lv_obj_set_y(lv_label_5, 165);
    lv_obj_set_style_text_font(lv_label_5, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lv_label_5, lv_color_hex(0xffffff), 0);

    lv_obj_t *lv_label_6 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_6, "Coolant -");
    lv_obj_set_x(lv_label_6, 200);
    lv_obj_set_y(lv_label_6, 165);
    lv_obj_set_style_text_font(lv_label_6, &lv_font_montserrat_12, 0);
    lv_obj_set_style_text_color(lv_label_6, lv_color_hex(0xffffff), 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
