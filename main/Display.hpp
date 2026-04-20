#include <stdio.h>
#include "epd7in5_V2.hpp"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"

#include <string>

class Display
{
    public:
        Display(unsigned char *buffer) : framebuffer(buffer)
        {
            int ret = epd.Init();
            printf("Epd Init %s\n", (ret ? "failed" : "successful!"));

            lv_init();
            printf("lv_init done!\n");
            
            initialize_lvgl_display();
            printf("Display is created!\n");
        }

        static void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
        static uint32_t get_esp_time_cb(void);


        void set_sanity_screen_markings();
        void set_test_screen_figures();
        lv_obj_t *set_big_number_widget(char *value, char *text);
        lv_obj_t *create_wifi_widget(std::string &value);

        lv_display_t *initialize_lvgl_display();

    private:
        Epd epd;
        uint8_t *framebuffer;
};
