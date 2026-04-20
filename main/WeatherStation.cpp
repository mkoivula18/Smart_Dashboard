#include "WeatherStation.hpp"
#include <stdio.h>
#include <string.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_http_client.h"
#include "esp_crt_bundle.h"

static const char *TAG = "WeatherStation";

static esp_err_t http_event_handler(esp_http_client_event_t *event);
