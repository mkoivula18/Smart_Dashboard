#pragma once

#include <stdio.h>
#include <string>
#include <cstddef>

#include "epd7in5_V2.hpp"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"

#include "WeatherWidget.hpp"
#include "WifiWidget.hpp"
#include "ClockWidget.hpp"

#include "CommonDefs.hpp"

class Display
{
    public:
        static constexpr size_t LVGL_I1_PALETTE_BYTES =
            LV_COLOR_INDEXED_PALETTE_SIZE(LV_COLOR_FORMAT_I1) * sizeof(lv_color32_t);

        Display(unsigned char *buffer);
        ~Display();

        static void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
        static void expand_partial_refresh_area(lv_event_t *event);
        static uint32_t get_esp_time_cb(void);


        void set_sanity_screen_markings();
        void set_test_screen_figures();

        bool is_ready() const;
        void process_lvgl();
        void update_wifi(const char* status_message);
        void update_weather(const char* value);
        void update_clock(const char* time);
        void refresh_widget(Widget *widget);

        void refresh_full();

        lv_display_t *initialize_lvgl_display();

    private:
        Epd epd;
        uint8_t *framebuffer;
        lv_display_t *lvgl_display = nullptr;
        uint32_t partial_refresh_count = 0;
        bool full_refresh_due = false;
        bool partial_cycle_has_region = false;
        bool partial_cycle_failed = false;
        bool display_ready = false;

        static constexpr uint32_t FULL_REFRESH_AFTER_PARTIAL_UPDATES = 20;
        static constexpr lv_coord_t PARTIAL_REFRESH_X1 = 0;
        static constexpr lv_coord_t PARTIAL_REFRESH_X2 = EPD_WIDTH - 1;

        lv_obj_t *temperature_number;

        lv_obj_t *main_screen           = nullptr;
        WeatherWidget *weather_widget   = nullptr;
        WifiWidget *wifi_widget         = nullptr;
        ClockWidget *clock_widget       = nullptr;
};
