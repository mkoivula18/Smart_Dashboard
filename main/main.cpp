#include <stdio.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "epd7in5_V2.hpp"
#include "esp_timer.h"

#include "lvgl.h"

Epd epd;
// Initialize buffer with zeros 
unsigned char framebuffer[FRAME_BUFFER_SIZE_IN_BYTES*2] {};

void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map);
uint32_t mikko_lv_tick_get_cb(void);


uint32_t mikko_lv_tick_get_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

void flush_callback(lv_display_t *display, const lv_area_t *area, unsigned char *px_map)
{
    printf("Inside callback function!\n");
    epd.DisplayFrameBuffer(framebuffer, (size_t)FRAME_BUFFER_SIZE_IN_BITS);
    lv_display_flush_ready(display);
}

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

    int ret = epd.Init();

    printf("epd Init() returned %d!\n", ret);


    static unsigned char black_image[800 * 480 / 8];
    static unsigned char white_image[800 * 480 / 8];

    memset(black_image, 0xFF, sizeof(black_image));
    unsigned char *imagePtr = black_image;
    imagePtr = imagePtr + sizeof(black_image) / 2;
    memset(imagePtr, 0x00, sizeof(black_image) / 2);
    memset(white_image, 0x00, sizeof(white_image));

    // epd.DisplayFrame(black_image);
    // EpdIf::DelayMs(2000);

    lv_init();
    lv_tick_set_cb(mikko_lv_tick_get_cb);
    
    lv_display_t *display = lv_display_create(EPD_WIDTH, EPD_HEIGHT);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);
    lv_display_set_flush_cb(display, flush_callback);
    lv_display_set_buffers(display, framebuffer, NULL, sizeof(framebuffer), LV_DISPLAY_RENDER_MODE_FULL);
    
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_obj_center(label);
        

    // epd.DisplayFrame(white_image);
    // EpdIf::DelayMs(2000);

    // epd.DisplayFrameBuffer(black_image, sizeof(black_image));
    // EpdIf::DelayMs(2000);
    
    // epd.DisplayFrameBuffer(white_image, sizeof(white_image));
    // EpdIf::DelayMs(2000);

    int olen = 0;
    while (true) {
        // if (olen)
        // {
        // }
        // else
        // {
        //     lv_label_set_text(label, "Elossa");
        // }
        // olen = ~olen;
        
        lv_label_set_text(label, "Olen elossa!!");
        printf("before lv_timer_handler\n");
        lv_timer_handler();
        printf("after lv_timer_handler\n");
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
