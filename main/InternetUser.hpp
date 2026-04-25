#pragma once

#include <stdio.h>
#include <string>

#include "esp_http_client.h"
#include "esp_crt_bundle.h"
#include "esp_log.h"


class InternetUser
{
    public:
    InternetUser()
    {
        ESP_LOGI(TAG, "New InternetUser created!\n");
    }

    static esp_err_t http_event_handler(esp_http_client_event_t *event);
    std::string fetch_xml_from_url(const char *url);

    private:
        const char *TAG = "InternetUser";
};
