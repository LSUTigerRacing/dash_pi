
#include "ui.hpp"
#include "lvgl.h"

#include <cstdint>

namespace
{
    // Colors
    constexpr uint32_t BLACK      = 0x000000;
    constexpr uint32_t DARK_GREY  = 0x242424;
    constexpr uint32_t MID_GREY   = 0x555555;
    constexpr uint32_t GREEN      = 0x00FF00;
    constexpr uint32_t WHITE      = 0xEEEEEE;
    constexpr uint32_t YELLOW     = 0xFFFF00;
    constexpr uint32_t RED        = 0xFF3030;
    constexpr uint32_t GREY       = 0x888888;

    // Gauges
    lv_obj_t *rpm_arc = nullptr;
    lv_obj_t *current_arc = nullptr;

    lv_obj_t *rpm_value = nullptr;
    lv_obj_t *current_value = nullptr;

    // Telemetry cards
    lv_obj_t *motor_temp_value = nullptr;
    lv_obj_t *voltage_value = nullptr;
    lv_obj_t *coolant_value = nullptr;
    lv_obj_t *brake_value = nullptr;

    // Status labels
    lv_obj_t *rtd_value = nullptr;
    lv_obj_t *fault_value = nullptr;
    lv_obj_t *status_value = nullptr;

    constexpr int32_t RPM_MAX = 10000;

    // Motor current is displayed in tenths of an amp,
    // following the decimal convention in the old UI.
    constexpr int32_t CURRENT_MAX = 3000;

    int32_t clamp_value(int32_t value,
                        int32_t minimum,
                        int32_t maximum)
    {
        if (value < minimum) return minimum;
        if (value > maximum) return maximum;
        return value;
    }

    lv_color_t color(uint32_t hex)
    {
        return lv_color_hex(hex);
    }

    lv_obj_t *make_label(
        lv_obj_t *parent,
        const char *text,
        int32_t x,
        int32_t y,
        int32_t width,
        int32_t height,
        const lv_font_t *font,
        uint32_t text_color)
    {
        lv_obj_t *label = lv_label_create(parent);

        lv_label_set_text(label, text);
        lv_obj_set_pos(label, x, y);
        lv_obj_set_size(label, width, height);

        lv_obj_set_style_text_font(label, font, 0);
        lv_obj_set_style_text_color(label, color(text_color), 0);
        lv_obj_set_style_text_align(
            label, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_set_style_bg_opa(label, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(label, 0, 0);
        lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

        return label;
    }

    void set_integer(lv_obj_t *label, uint32_t value)
    {
        if (label == nullptr) return;

        lv_label_set_text_fmt(
            label, "%u", static_cast<unsigned>(value));
    }

    // Format a value stored in tenths:
    // 505 becomes "50.5".
    void set_tenths(lv_obj_t *label, uint32_t value)
    {
        if (label == nullptr) return;

        char buffer[20];

        lv_snprintf(
            buffer,
            sizeof(buffer),
            "%u.%u",
            static_cast<unsigned>(value / 10),
            static_cast<unsigned>(value % 10));

        lv_label_set_text(label, buffer);
    }

    // Create a telemetry card with a title and live value.
    lv_obj_t *make_card(
        lv_obj_t *parent,
        const char *title,
        int32_t x,
        int32_t y,
        int32_t width,
        int32_t height)
    {
        lv_obj_t *card = lv_obj_create(parent);

        lv_obj_set_pos(card, x, y);
        lv_obj_set_size(card, width, height);

        lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_style_radius(card, 5, 0);
        lv_obj_set_style_bg_color(
            card, color(DARK_GREY), 0);
        lv_obj_set_style_bg_opa(
            card, LV_OPA_COVER, 0);

        lv_obj_set_style_border_color(
            card, color(MID_GREY), 0);
        lv_obj_set_style_border_width(card, 1, 0);
        lv_obj_set_style_pad_all(card, 1, 0);

        make_label(
            card, title,
            1, 1, width - 4, 14,
            &lv_font_montserrat_12, WHITE);

        lv_obj_t *value = make_label(
            card, "--",
            1, 16, width - 4, height - 18,
            &lv_font_montserrat_16, GREEN);

        return value;
    }

    // Create a circular gauge.
    lv_obj_t *make_gauge(
        lv_obj_t *parent,
        const char *title,
        int32_t x,
        int32_t y,
        int32_t maximum,
        lv_obj_t **value_label)
    {
        lv_obj_t *arc = lv_arc_create(parent);

        lv_obj_set_size(arc, 96, 96);
        lv_obj_set_pos(arc, x, y);

        lv_arc_set_rotation(arc, 135);
        lv_arc_set_bg_angles(arc, 0, 270);
        lv_arc_set_range(arc, 0, maximum);
        lv_arc_set_value(arc, 0);

        lv_obj_set_style_arc_color(
            arc, color(MID_GREY), LV_PART_MAIN);
        lv_obj_set_style_arc_width(
            arc, 7, LV_PART_MAIN);

        lv_obj_set_style_arc_color(
            arc, color(GREEN), LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(
            arc, 7, LV_PART_INDICATOR);

        // Hide the draggable knob.
        lv_obj_set_style_opa(
            arc, LV_OPA_TRANSP, LV_PART_KNOB);

        lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

        *value_label = make_label(
            parent, "0",
            x + 8, y + 31, 80, 30,
            &lv_font_montserrat_12, GREEN);

        make_label(
            parent, title,
            x - 2, y + 77, 100, 18,
            &lv_font_montserrat_12, WHITE);

        return arc;
    }

    void set_gauge_value(
        lv_obj_t *arc,
        int32_t value,
        int32_t maximum)
    {
        if (arc == nullptr) return;

        value = clamp_value(value, 0, maximum);
        lv_arc_set_value(arc, value);
    }
}

void ui_init()
{
    // Create and load the dashboard screen.
    lv_obj_t *screen = lv_obj_create(nullptr);

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(
        screen, color(BLACK), 0);
    lv_obj_set_style_bg_opa(
        screen, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_style_border_width(screen, 0, 0);

    lv_screen_load(screen);

    // Header
    make_label(
        screen, "MOTOR DASHBOARD",
        4, 0, 180, 18,
        &lv_font_montserrat_12, WHITE);

    make_label(
        screen, "LIVE",
        270, 0, 44, 18,
        &lv_font_montserrat_12, GREEN);

    // Left gauge: Motor RPM
    rpm_arc = make_gauge(
        screen, "Motor RPM",
        2, 20, RPM_MAX, &rpm_value);

    // Center telemetry cards
    motor_temp_value = make_card(
        screen, "MOTOR C",
        106, 25, 54, 43);

    voltage_value = make_card(
        screen, "MC VOLTS",
        163, 25, 54, 43);

    coolant_value = make_card(
        screen, "COOLANT",
        106, 72, 54, 43);

    brake_value = make_card(
        screen, "BRAKE",
        163, 72, 54, 43);

    // Right gauge: AC current
    current_arc = make_gauge(
        screen, "AC Current",
        220, 20, CURRENT_MAX, &current_value);

    // RTD indicator. No RTD field is currently provided
    // in UARTData, so it starts as unavailable.
    make_label(
        screen, "RTD",
        4, 130, 42, 24,
        &lv_font_montserrat_16, GREY);

    rtd_value = make_label(
        screen, "N/A",
        46, 130, 48, 24,
        &lv_font_montserrat_12, GREY);

    // Fault indicator
    make_label(
        screen, "FAULT",
        103, 130, 52, 24,
        &lv_font_montserrat_12, WHITE);

    fault_value = make_label(
        screen, "NONE",
        155, 130, 70, 24,
        &lv_font_montserrat_12, GREEN);

    // Additional motor status
    make_label(
        screen, "MOTOR STATUS",
        4, 160, 105, 17,
        &lv_font_montserrat_12, WHITE);

    status_value = make_label(
        screen, "WAITING FOR UART",
        105, 160, 210, 17,
        &lv_font_montserrat_12, YELLOW);

    // Bottom status bar
    lv_obj_t *status_bar = lv_obj_create(screen);

    lv_obj_set_pos(status_bar, 3, 195);
    lv_obj_set_size(status_bar, 314, 40);

    lv_obj_clear_flag(
        status_bar, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_radius(status_bar, 4, 0);
    lv_obj_set_style_bg_color(
        status_bar, color(DARK_GREY), 0);
    lv_obj_set_style_bg_opa(
        status_bar, LV_OPA_COVER, 0);

    lv_obj_set_style_border_color(
        status_bar, color(MID_GREY), 0);
    lv_obj_set_style_border_width(status_bar, 1, 0);
    lv_obj_set_style_pad_all(status_bar, 2, 0);

    make_label(
        status_bar, "CAN2: N/A",
        0, 8, 100, 18,
        &lv_font_montserrat_12, YELLOW);

    make_label(
        status_bar, "LOG: READY",
        104, 8, 100, 18,
        &lv_font_montserrat_12, GREEN);

    make_label(
        status_bar, "GPS: N/A",
        210, 8, 95, 18,
        &lv_font_montserrat_12, YELLOW);
}

void ui_update(const UARTData &data)
{
    // Motor RPM gauge
    set_gauge_value(
        rpm_arc, data.motor_rpm, RPM_MAX);

    set_integer(rpm_value, data.motor_rpm);

    // Motor current gauge.
    // Assumes motor_current is measured in tenths of an amp.
    set_gauge_value(
        current_arc, data.motor_current, CURRENT_MAX);

    set_tenths(current_value, data.motor_current);

    // Telemetry cards.
    // Temperatures are formatted in tenths, matching
    // the convention used by the previous UI.
    set_tenths(
        motor_temp_value, data.motor_temperature);

    // Motor voltage is displayed as a whole number.
    set_integer(
        voltage_value, data.motor_voltage);

    set_tenths(
        coolant_value, data.coolant_temperature);

    set_tenths(
        brake_value, data.brake_temperature);

    // RTD is not available in the current UARTData struct.
    // Keep the indicator explicitly unavailable.

    // Fault information
    if (data.has_active_fault)
    {
        if (fault_value != nullptr)
        {
            lv_label_set_text_fmt(
                fault_value, "%u / %u",
                static_cast<unsigned>(data.fault_code),
                static_cast<unsigned>(data.fault_severity));

            lv_obj_set_style_text_color(
                fault_value, color(RED), 0);
        }
    }
    else
    {
        if (fault_value != nullptr)
        {
            lv_label_set_text(fault_value, "NONE");

            lv_obj_set_style_text_color(
                fault_value, color(GREEN), 0);
        }
    }

    // Motor status byte.
    if (status_value != nullptr)
    {
        lv_label_set_text_fmt(
            status_value, "STATUS: %u",
            static_cast<unsigned>(data.motor_status));
    }
}
