#include <stdio.h>
#include "lvgl.h"
#include "esp_chip_info.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Display.hpp"
#include "WeatherStation.hpp"
#include "WifiHelper.hpp"
#include "DataHandler.hpp"

#include "secrets.hpp"
#include <string>


// Initialize buffer with zeros
unsigned char framebuffer[FRAME_BUFFER_SIZE_IN_BYTES*2] {};

void print_esp32_info();

extern "C" void app_main(void)
{
    print_esp32_info();

    Display display(framebuffer);
    WifiHelper wifi_helper;

    bool wifi_connected = wifi_helper.connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    std::string wifi_status_text = (wifi_connected) ? "Connected" : "Not connected";
    display.set_wifi_status(wifi_status_text.c_str());

    DataHandler data_handler;
    data_handler.sync_time_with_ntp();

    // display.set_sanity_screen_markings();
    data_handler.update_current_time();

    while(true)
    {
        data_handler.update_current_temperature();
        // data_handler.update_current_time();

        if (data_handler.new_data_available())
        {
            display.update_clock(data_handler.get_current_time().c_str());
            display.set_temperature_number(data_handler.get_current_temperature().c_str());
            data_handler.set_new_data_available(false);
        }

        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(1000));
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
