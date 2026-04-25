#pragma once

#include <stdio.h>
#include <string>

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
        Display(unsigned char *buffer) : framebuffer(buffer)
        {
            int ret = epd.Init();
            printf("Epd Init %s\n", (ret ? "failed" : "successful!"));

            lv_init();
            initialize_lvgl_display();

            weather_widget = new WeatherWidget(main_screen, "9", "Ulkolampotila");
            weather_widget->set_position(LV_ALIGN_LEFT_MID, 0, 0);
            // weather_widget->add_borders_to_container();

            wifi_widget = new WifiWidget(main_screen, "Unknown status");
            wifi_widget->set_position(LV_ALIGN_TOP_RIGHT, 0, 0);
            // wifi_widget->add_borders_to_container();

            clock_widget = new ClockWidget(main_screen, "XX:XX");
            clock_widget->set_position(LV_ALIGN_OUT_TOP_MID, 0, 0);
            // clock_widget->add_borders_to_container();
        }

        ~Display()
        {
            delete weather_widget;
            delete wifi_widget;
        }

        static void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
        static uint32_t get_esp_time_cb(void);


        void set_sanity_screen_markings();
        void set_test_screen_figures();

        void set_wifi_status(const char* status_message)
        {
            wifi_widget->set_status(status_message);
        }

        void set_temperature_number(const char* value)
        {
            weather_widget->set_value(value);
        }

        void update_clock(const char* time)
        {
            clock_widget->set_time(time);
        }

        lv_display_t *initialize_lvgl_display();

    private:
        Epd epd;
        uint8_t *framebuffer;

        lv_obj_t *temperature_number;

        lv_obj_t *main_screen           = nullptr;
        WeatherWidget *weather_widget   = nullptr;
        WifiWidget *wifi_widget         = nullptr;
        ClockWidget *clock_widget       = nullptr;
};
