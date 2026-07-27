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
        Display(unsigned char *buffer);
        ~Display();

        static void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
        static uint32_t get_esp_time_cb(void);


        void set_sanity_screen_markings();
        void set_test_screen_figures();

        void update_wifi(const char* status_message)
        {
            wifi_widget->set_status(status_message);
        }

        void update_weather(const char* value)
        {
            weather_widget->set_value(value);
            refresh_widget(weather_widget);
        }

        void update_clock(const char* time)
        {
            clock_widget->set_time(time);
            refresh_widget(clock_widget);
        }

        void refresh_widget(Widget *widget)
        {
            lv_obj_t *container = widget->get_container();
            lv_area_t area;
            lv_obj_get_coords(container, &area);
            printf("Object coordinates: (%ld, %ld), (%ld, %ld)\n", area.x1, area.y1, area.x2, area.y2);

            // refresh_area(area);

            lv_obj_invalidate(container);
        }

        // void refresh_area(lv_area_t& area)
        // {
        //     printf("Area refreshing implementation TODO..\n");
        // }

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
