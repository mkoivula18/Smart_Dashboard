
#include "Display.hpp"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"

Display::Display(unsigned char *buffer) : framebuffer(buffer)
{
    int ret = epd.Init();
    printf("Epd Init %s\n", (ret ? "failed" : "successful!"));
    if (ret != 0) {
        printf("Display disabled: e-paper initialization failed\n");
        return;
    }
    
    // int partial_init = epd.EPD_7IN5_V2_Init_Part();
    // printf("Epd Partial Init %s\n", (partial_init ? "failed" : "successful!"));

    lv_init();
    initialize_lvgl_display();

    header_container = lv_obj_create(main_screen);
    lv_obj_set_size(header_container, SCREEN_SIZE_WIDTH, 120);
    lv_obj_align(header_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(header_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(header_container, 0, 0);
    lv_obj_set_style_pad_all(header_container, 0, 0);

    clock_widget = new ClockWidget(header_container, "XX:XX", "", "");
    clock_widget->set_position(LV_ALIGN_TOP_LEFT, 0, 0);
    clock_widget->add_borders_to_container();

    weather_widget = new WeatherWidget(main_screen, "9", "Ulkolampotila");
    weather_widget->set_position(LV_ALIGN_TOP_MID, 0, 128);
    weather_widget->add_borders_to_container();

    wifi_widget = new WifiWidget(header_container, "Unknown");
    wifi_widget->set_position(LV_ALIGN_TOP_RIGHT, 0, 0);
    wifi_widget->add_borders_to_container();

    // Draw a known-good full image before switching LVGL to area-local buffers.
    display_ready = true;
    refresh_full();

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
    
    lvgl_display = lv_display_create(EPD_WIDTH, EPD_HEIGHT);
    lv_display_set_color_format(lvgl_display, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(lvgl_display, flush_callback);

    // This panel's partial-window controller is unreliable when a window starts
    // at a non-zero X coordinate.  Expanding invalidations here makes LVGL
    // render a complete horizontal band, so the flush buffer really contains
    // pixels for x = 0..799 (rather than expanding the area later in the EPD
    // driver, where those pixels would not exist).
    // lv_display_add_event_cb(lvgl_display, expand_partial_refresh_area,
    //                         LV_EVENT_INVALIDATE_AREA, nullptr);

    // Keep the screen-sized buffer for the first implementation. LVGL still
    // renders only invalidated areas, while the larger buffer avoids splitting
    // a normal widget update into several row-sized flushes.
    lv_display_set_buffers(lvgl_display, framebuffer, NULL,
                           FRAME_BUFFER_SIZE_IN_BYTES + LVGL_I1_PALETTE_BYTES,
                           LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_user_data(lvgl_display, this);

    main_screen = lv_obj_create(lv_screen_active());
    lv_obj_set_size(main_screen, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);
    lv_obj_center(main_screen);
    lv_obj_set_style_bg_opa(main_screen, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(main_screen, lv_color_black(), 0);
    lv_obj_set_style_border_width(main_screen, 0, 0);
    lv_obj_set_style_pad_all(main_screen, 0, 0);

    return lvgl_display;
}

void Display::refresh_full()
{
    if (!display_ready || lvgl_display == nullptr) {
        return;
    }

    // A full update must use the panel's full-refresh initialization. It also
    // resets any partial-mode controller state that could affect contrast.
    // Do not call Epd::Init() here: it also initializes the SPI bus.
    const int full_init = epd.InitFullUpdate();
    if (full_init != 0) {
        printf("Epd full-refresh init failed\n");
        return;
    }
    lv_display_set_render_mode(lvgl_display, LV_DISPLAY_RENDER_MODE_FULL);
    lv_obj_invalidate(lv_screen_active());
    lv_refr_now(lvgl_display);
    lv_display_set_render_mode(lvgl_display, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

bool Display::is_ready() const
{
    return display_ready;
}

void Display::process_lvgl()
{
    if (display_ready) {
        lv_timer_handler();
    }
}

void Display::update_wifi(const char *status_message)
{
    if (!display_ready || wifi_widget == nullptr) {
        return;
    }

    lv_display_enable_invalidation(lvgl_display, false);
    wifi_widget->set_status(status_message);
    lv_display_enable_invalidation(lvgl_display, true);
    refresh_widget(wifi_widget);
}

void Display::update_weather(const char *value)
{
    if (!display_ready || weather_widget == nullptr) {
        return;
    }

    lv_display_enable_invalidation(lvgl_display, false);
    weather_widget->set_value(value);
    lv_display_enable_invalidation(lvgl_display, true);
    refresh_widget(weather_widget);
}

void Display::update_clock(const char *time, const char *weekday, const char *date)
{
    if (!display_ready || clock_widget == nullptr) {
        return;
    }

    clock_widget->set_datetime(time, weekday, date);
    refresh_widget(clock_widget);
}

void Display::refresh_widget(Widget *widget)
{
    if (!display_ready || widget == nullptr) {
        return;
    }

    lv_obj_t *container = widget->get_container();
    if (container == nullptr) {
        return;
    }

    lv_obj_update_layout(container);
    lv_obj_invalidate(container);
}

uint32_t Display::get_esp_time_cb()
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

void Display::expand_partial_refresh_area(lv_event_t *event)
{
    lv_area_t *area = lv_event_get_invalidated_area(event);
    if (area == nullptr) {
        return;
    }

    // Keep the dirty rows, but always redraw the whole horizontal band. This
    // also gives opaque widget containers a chance to overwrite old glyphs.
    area->x1 = PARTIAL_REFRESH_X1;
    area->x2 = PARTIAL_REFRESH_X2;
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
    Display *displayObject = static_cast<Display *>(lv_display_get_user_data(display));
    const uint8_t *pixels = px_map + LVGL_I1_PALETTE_BYTES;

    if (lv_display_get_render_mode(display) == LV_DISPLAY_RENDER_MODE_FULL) {
        // Full mode always supplies the complete framebuffer.
        displayObject->epd.DisplayFrame(pixels);
        lv_display_flush_ready(display);
        return;
    }

    // Invalid partial regions are logged and skipped. Runtime updates never
    // fall back to full refresh because that path is reserved for boot.
    if (!displayObject->epd.IsValidPartialArea(*area)) {
        printf("Skipping invalid LVGL partial area\n");
    }
    else {
        // A logical LVGL refresh can contain multiple flushes. Upload every
        // region first and make the panel visible only after the last one.
        const bool began = displayObject->epd.BeginPartialUpdate();
        if (!began) {
            printf("Could not begin LVGL partial update\n");
        }
        else if (!displayObject->epd.WriteRegion(*area, pixels)) {
            printf("Could not write LVGL partial area\n");
        }
    }

    if (lv_display_flush_is_last(display)) {
        // EndPartialUpdate() is a no-op if no partial transaction began.
        // Calling it unconditionally also closes a transaction whose write
        // failed after BeginPartialUpdate().
        displayObject->epd.EndPartialUpdate();
    }

    lv_display_flush_ready(display);
}
