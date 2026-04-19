
#include "Display.hpp"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"

lv_display_t *Display::initialize_lvgl_display()
{
    lv_tick_set_cb(get_esp_time_cb);
    
    lv_display_t *display = lv_display_create(EPD_WIDTH, EPD_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(display, flush_callback);
    lv_display_set_buffers(display, framebuffer, NULL, FRAME_BUFFER_SIZE_IN_BYTES + 8, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_user_data(display, this);
    return display;
}

uint32_t Display::get_esp_time_cb()
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

lv_obj_t *Display::set_big_number_widget(char *value, char *text)
{
    uint32_t width = 340;
    uint32_t height = 80;
    lv_obj_t *metric = lv_obj_create(lv_screen_active());
    lv_obj_set_size(metric, width, height);
    lv_obj_align(metric, LV_ALIGN_CENTER, 0, -40);

    lv_obj_set_style_bg_opa(metric, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(metric, 0, 0);
    lv_obj_set_style_pad_all(metric, 0, 0);

    lv_obj_t *number = lv_label_create(metric);
    lv_label_set_text(number, value);
    lv_obj_set_style_text_font(number, &lv_font_montserrat_48, 0);
    lv_obj_align(number, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *unit = lv_label_create(metric);
    lv_label_set_text(unit, "°C");
    lv_obj_set_style_text_font(unit, &lv_font_montserrat_24, 0);
    lv_obj_align_to(unit, number, LV_ALIGN_OUT_RIGHT_TOP, 8, 10);

    lv_obj_t *caption = lv_label_create(metric);
    lv_label_set_text(caption, text);
    lv_obj_align(caption, LV_ALIGN_BOTTOM_LEFT, 4, 0);

    uint32_t box_padding = 10;
    lv_obj_t *box = lv_obj_create(metric);
    lv_obj_set_size(box, width + box_padding, height + box_padding);
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(box, lv_color_black(), 0);
    lv_obj_set_style_border_width(box, 4, 0);

    return metric;
}

void Display::set_test_screen_figures()
{
    lv_obj_t *box = lv_obj_create(lv_screen_active());
    lv_obj_set_size(box, 260, 120);
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(box, lv_color_black(), 0);
    lv_obj_set_style_border_width(box, 4, 0);
}

void Display::set_sanity_screen_markings()
{
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "CENTER");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *top_left = lv_label_create(lv_screen_active());
    lv_label_set_text(top_left, "TL");
    lv_obj_align(top_left, LV_ALIGN_TOP_LEFT, 4, 4);

    lv_obj_t *top_right = lv_label_create(lv_screen_active());
    lv_label_set_text(top_right, "TR");
    lv_obj_align(top_right, LV_ALIGN_TOP_RIGHT, -4, 4);

    lv_obj_t *bot_left = lv_label_create(lv_screen_active());
    lv_label_set_text(bot_left, "BL");
    lv_obj_align(bot_left, LV_ALIGN_BOTTOM_LEFT, 4, -4);

    lv_obj_t *bot_right = lv_label_create(lv_screen_active());
    lv_label_set_text(bot_right, "BR");
    lv_obj_align(bot_right, LV_ALIGN_BOTTOM_RIGHT, -4, -4);
}

void Display::flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map)
{
    printf("Inside callback function!\n");
    // epd.DisplayFrameBuffer(framebuffer, (size_t)FRAME_BUFFER_SIZE_IN_BITS);
    // First 8 bytes are reserved for other data than actual pixels, move pointer 64-bits to the right

    Display *displayObject = (Display*)lv_display_get_user_data(display);

    displayObject->epd.DisplayFrame(px_map + 8);
    lv_display_flush_ready(display);
}
