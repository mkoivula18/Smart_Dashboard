
#include "Display.hpp"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"

Display::Display(unsigned char *buffer) : framebuffer(buffer)
{
    int ret = epd.Init();
    printf("Epd Init %s\n", (ret ? "failed" : "successful!"));

    lv_init();
    initialize_lvgl_display();

    clock_widget = new ClockWidget(main_screen, "XX:XX");
    clock_widget->set_position(LV_ALIGN_OUT_TOP_MID, 0, 0);
    // clock_widget->add_borders_to_container();

    weather_widget = new WeatherWidget(main_screen, "9", "Ulkolampotila");
    // weather_widget->add_borders_to_container();

    wifi_widget = new WifiWidget(main_screen, "Unknown status");
    wifi_widget->set_position(LV_ALIGN_TOP_RIGHT, 0, 0);
    // wifi_widget->add_borders_to_container();

    lv_obj_set_flex_flow(main_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(main_screen, 8, 0);

}

Display::~Display()
{
    delete weather_widget;
    delete wifi_widget;
    delete clock_widget;
}

lv_display_t *Display::initialize_lvgl_display()
{
    lv_tick_set_cb(get_esp_time_cb);
    
    lv_display_t *display = lv_display_create(EPD_WIDTH, EPD_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(display, flush_callback);
    lv_display_set_buffers(display, framebuffer, NULL, FRAME_BUFFER_SIZE_IN_BYTES + 8, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_user_data(display, this);

    main_screen = lv_obj_create(lv_screen_active());
    lv_obj_set_size(main_screen, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);
    lv_obj_center(main_screen);
    lv_obj_set_style_bg_opa(main_screen, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(main_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(main_screen, 0, 0);
    lv_obj_set_style_pad_all(main_screen, 0, 0);

    return display;
}

uint32_t Display::get_esp_time_cb()
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

void Display::set_test_screen_figures()
{
    lv_obj_t *box = lv_obj_create(main_screen);
    lv_obj_set_size(box, 260, 120);
    lv_obj_align(box, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(box, lv_color_black(), 0);
    lv_obj_set_style_border_width(box, 4, 0);
}

void Display::set_sanity_screen_markings()
{
    lv_obj_t *label = lv_label_create(main_screen);
    lv_label_set_text(label, "CENTER");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *top_left = lv_label_create(main_screen);
    lv_label_set_text(top_left, "TL");
    lv_obj_align(top_left, LV_ALIGN_TOP_LEFT, 4, 4);

    lv_obj_t *top_right = lv_label_create(main_screen);
    lv_label_set_text(top_right, "TR");
    lv_obj_align(top_right, LV_ALIGN_TOP_RIGHT, -4, 4);

    lv_obj_t *bot_left = lv_label_create(main_screen);
    lv_label_set_text(bot_left, "BL");
    lv_obj_align(bot_left, LV_ALIGN_BOTTOM_LEFT, 4, -4);

    lv_obj_t *bot_right = lv_label_create(main_screen);
    lv_label_set_text(bot_right, "BR");
    lv_obj_align(bot_right, LV_ALIGN_BOTTOM_RIGHT, -4, -4);
}

void Display::flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map)
{
    printf("Updating screen..\n");
    // epd.DisplayFrameBuffer(framebuffer, (size_t)FRAME_BUFFER_SIZE_IN_BITS);
    // First 8 bytes are reserved for other data than actual pixels, move pointer 64-bits to the right

    Display *displayObject = (Display*)lv_display_get_user_data(display);

    // displayObject->epd.DisplayFrame(px_map + 8);

    displayObject->epd.DisplayFramePartial(px_map + 8, area->x1, area->y1, area->x2, area->y2, px_map);
    lv_display_flush_ready(display);

    // printf("px_map: 0x%p\n", (void*)px_map);
    printf("area: (%ld, %ld), (%ld, %ld)", area->x1, area->y1, area->x2, area->y2);

}
