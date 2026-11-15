#include <stdio.h>
#include "lvgl.h"
#include "esp_chip_info.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Display.hpp"
#include "WeatherStation.hpp"
#include "WifiHelper.hpp"
#include "DataHandler.hpp"

#include "secrets.hpp"
#include <optional>
#include <string>

// Comment out this line to return to the normal Wi-Fi, NTP, and API updates.
// #define DISPLAY_REFRESH_TEST

namespace {
constexpr uint32_t DASHBOARD_UPDATE_INTERVAL_SECONDS = 40;

#ifdef DISPLAY_REFRESH_TEST
void run_display_refresh_test(Display &display)
{
    static int refresh_step = 0;
    static int temperature = -99;

    char clock_text[9];
    snprintf(clock_text, sizeof(clock_text), "%02d:%02d",
             (refresh_step / 60) % 24, refresh_step % 60);

    display.update_clock(clock_text, "Testi", "15.11.2026");
    display.update_weather_weekdays(0);
    display.update_weather(temperature);

    refresh_step = (refresh_step + 1) % (24 * 60);
    temperature = (temperature == 99) ? -99 : temperature + 1;
}
#endif
}

// Initialize buffer with zeros
// LVGL stores its I1 palette immediately before the pixel data.
unsigned char framebuffer[FRAME_BUFFER_SIZE_IN_BYTES * 2 + Display::LVGL_I1_PALETTE_BYTES] {};

void print_esp32_info();

extern "C" void app_main(void)
{
    // esp_log_level_set("*", ESP_LOG_WARN);
    // Certificate validation succeeds on every HTTPS request; keep it out of
    // normal serial output while preserving warnings and errors.
    esp_log_level_set("esp-x509-crt-bundle", ESP_LOG_WARN);
    print_esp32_info();

    Display display(framebuffer);

#ifdef DISPLAY_REFRESH_TEST
    while (true)
    {
        run_display_refresh_test(display);
        display.process_lvgl();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif

    WifiHelper wifi_helper;

    bool wifi_connected = wifi_helper.connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    std::string wifi_status_text = (wifi_connected) ? "Connected" : "Not connected";
    display.update_wifi(wifi_status_text.c_str());

    DataHandler data_handler;
    data_handler.sync_time_with_ntp();
    DataAvailable *data_available = data_handler.get_data_available();

    // display.set_sanity_screen_markings();
    while(true)
    {
        // No displayed value changes more often than once per minute in
        // normal operation. The faster test loop above remains opt-in.
        data_handler.update_current_temperature();
        data_handler.update_current_time();

        if (data_handler.new_data_available())
        {
            if (data_available->clock)
            {
                display.update_clock(data_handler.get_current_time().c_str(),
                                     data_handler.get_current_weekday().c_str(),
                                     data_handler.get_current_date().c_str());
                display.update_weather_weekdays(data_handler.get_current_weekday_index());
            }
            if (data_available->weather)
            {
                const std::optional<int> &temperature = data_handler.get_current_temperature();
                if (temperature.has_value())
                {
                    display.update_weather(*temperature);
                }
            }
            if (data_available->wifi)
            {
                std::string test_string = "Test";
                display.update_wifi(test_string.c_str());
            }

            data_handler.clear_data_available();
        }

        data_handler.clear_data_available();

        display.process_lvgl();
        vTaskDelay(pdMS_TO_TICKS(DASHBOARD_UPDATE_INTERVAL_SECONDS * 1000));
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
