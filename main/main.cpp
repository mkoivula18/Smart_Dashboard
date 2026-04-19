#include <stdio.h>
#include "lvgl.h"
#include "esp_chip_info.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Display.hpp"

// Initialize buffer with zeros
unsigned char framebuffer[FRAME_BUFFER_SIZE_IN_BYTES*2] {};

void print_esp32_info();

extern "C" void app_main(void)
{
    print_esp32_info();

    Display display(framebuffer);
    // display.set_sanity_screen_markings();
    // display.set_test_screen_figures();
    char temperature[3] = "14";
    lv_obj_t *big_ass_number = display.set_big_number_widget(temperature, "Temperature");

    lv_obj_align(big_ass_number, LV_ALIGN_DEFAULT, 40, 40);

    while(true)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void print_esp32_info()
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
}
