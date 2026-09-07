#include "ui.hpp"

#include "lvgl.h"

namespace
{
lv_obj_t *table;

enum TableRow : uint32_t {
    ROW_MC_TEMP = 0,
    ROW_MOTOR_TEMP,
    ROW_MC_INPUT_V,
    ROW_DC_CURRENT,
    ROW_AC_CURRENT,
    ROW_MC_THROTTLE,
    ROW_MC_DUTY_CYCLE,
    ROW_FAULT_CODE,
    ROW_VBAT,
    ROW_INT_TEMP,
    ROW_LOG,
    ROW_CELL_LOW,
    ROW_CELL_HIGH,
    ROW_PACK_VOLT,
    ROW_PACK_CURRENT,
    ROW_COUNT
};
//values for data
void set_value(uint32_t row, uint16_t value)
{
    char buffer[16];
    lv_snprintf(buffer, sizeof(buffer), "%u", value);
    lv_table_set_cell_value(table, row, 1, buffer);
}
//values for data but it has decimals
void set_decimal_value(uint32_t row, uint16_t value)
{
    char buffer[16];
    lv_snprintf(buffer, sizeof(buffer), "%u.%u", value / 10, value % 10);
    lv_table_set_cell_value(table, row, 1, buffer);
}
}

void ui_init()
{   //screen root
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
    lv_screen_load(screen);

    table = lv_table_create(screen);
    lv_obj_set_size(table, 320, 240);
    lv_obj_set_pos(table, 0, 0);
    lv_obj_clear_flag(table, LV_OBJ_FLAG_SCROLLABLE);
    //making table
    lv_table_set_col_width(table, 0, 200);
    lv_table_set_col_width(table, 1, 120);
    //style
    lv_obj_set_style_bg_color(table, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(table, 0, 0);
    lv_obj_set_style_pad_all(table, 0, 0);
    lv_obj_set_style_pad_row(table, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_column(table, 8, LV_PART_ITEMS);
    lv_obj_set_style_text_font(table, &lv_font_montserrat_18, LV_PART_ITEMS);
    lv_obj_set_style_text_color(table, lv_color_hex(0xEEEEEE), LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(table, LV_OPA_TRANSP, LV_PART_ITEMS);
    //placing name on the left side of the table
    lv_table_set_cell_value(table, ROW_MC_TEMP, 0, "MC Temp");
    lv_table_set_cell_value(table, ROW_MOTOR_TEMP, 0, "Motor Temp");
    lv_table_set_cell_value(table, ROW_MC_INPUT_V, 0, "MC Input V");
    lv_table_set_cell_value(table, ROW_DC_CURRENT, 0, "DC Current");
    lv_table_set_cell_value(table, ROW_AC_CURRENT, 0, "AC Current");
    lv_table_set_cell_value(table, ROW_MC_THROTTLE, 0, "MC Throttle");
    lv_table_set_cell_value(table, ROW_MC_DUTY_CYCLE, 0, "MC Duty Cycle");
    lv_table_set_cell_value(table, ROW_FAULT_CODE, 0, "Fault Code");
    lv_table_set_cell_value(table, ROW_VBAT, 0, "V BAT");
    lv_table_set_cell_value(table, ROW_INT_TEMP, 0, "Int Temp");
    lv_table_set_cell_value(table, ROW_LOG, 0, "LOG");
    lv_table_set_cell_value(table, ROW_CELL_LOW, 0, "Cell Low");
    lv_table_set_cell_value(table, ROW_CELL_HIGH, 0, "Cell High");
    lv_table_set_cell_value(table, ROW_PACK_VOLT, 0, "Pack Volt");
    lv_table_set_cell_value(table, ROW_PACK_CURRENT, 0, "Pack Current");

    //set all value to zero until actual data aarrive
    for(uint32_t row = 0; row < ROW_COUNT; row++) {
        lv_table_set_cell_value(table, row, 1, "0");
    }
}

void ui_update(const UARTData &data)
{
    set_decimal_value(ROW_MC_TEMP, data.motor_temperature);
    set_decimal_value(ROW_MOTOR_TEMP, data.motor_temperature);
    set_value(ROW_MC_INPUT_V, data.motor_voltage);
    set_decimal_value(ROW_DC_CURRENT, data.motor_current);
    set_decimal_value(ROW_AC_CURRENT, data.motor_current);
    set_value(ROW_MC_THROTTLE, data.motor_status);
    set_value(ROW_MC_DUTY_CYCLE, data.motor_status);
    set_value(ROW_FAULT_CODE, data.fault_code);

    set_decimal_value(ROW_VBAT, data.motor_voltage);
    set_value(ROW_INT_TEMP, data.coolant_temperature);
    set_value(ROW_LOG, data.bytes_received);

    set_value(ROW_CELL_LOW, 0);
    set_value(ROW_CELL_HIGH, 0);
    set_decimal_value(ROW_PACK_VOLT, data.motor_voltage);
    set_decimal_value(ROW_PACK_CURRENT, data.motor_current);
}
