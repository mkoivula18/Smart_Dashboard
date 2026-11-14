#include "InternetUser.hpp"


esp_err_t InternetUser::http_event_handler(esp_http_client_event_t *event)
{
    auto *response = (std::string*)event->user_data;

    if (event->event_id == HTTP_EVENT_ON_DATA)
    {
        response->append((char*)event->data, event->data_len);
    }

    return ESP_OK;
}

std::string InternetUser::fetch_xml_from_url(const char *url)
{
    std::string response;

    esp_http_client_config_t config{};
    config.url = url;
    config.method = HTTP_METHOD_GET;
    config.event_handler = http_event_handler;
    config.user_data = &response;
    config.timeout_ms = 15000;
    config.crt_bundle_attach = esp_crt_bundle_attach;

    esp_http_client_handle_t client = esp_http_client_init(&config);

    if (client == nullptr) {
        ESP_LOGE(TAG, "Failed to create HTTP client");
        return "";
    }

    esp_err_t err = esp_http_client_perform(client);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return "";
    }

    int status = esp_http_client_get_status_code(client);
    ESP_LOGD(TAG, "HTTP status: %d", status);
    ESP_LOGD(TAG, "Received %d bytes", response.size());

    esp_http_client_cleanup(client);

    if (status != 200) {
        return "";
    }

    return response;
}
