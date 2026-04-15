#include <stdio.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "epd7in5_V2.hpp"

// #include "EPD_7in5h.h"

extern "C" void app_main(void)
{
    printf("Hello world!\n");

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

    Epd epd;
    int ret = epd.Init();

    printf("epd Init() returned %d!\n", ret);

    epd.Clear();

    EpdIf::DelayMs(2000);

    static unsigned char black_image[800 * 480 / 8];
    memset(black_image, 0x00, sizeof(black_image));

    epd.DisplayFrame(black_image);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
