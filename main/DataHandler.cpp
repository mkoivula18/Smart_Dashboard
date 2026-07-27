#include "DataHandler.hpp"
#include "esp_netif_sntp.h"

DataHandler::DataHandler()
{
    printf("Datahandler here!\n");
}

void DataHandler::sync_time_with_ntp()
{
    setenv("TZ", "EET-2EEST,M3.5.0/3,M10.5.0/4", 1);
    tzset();

    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);

    printf("Waiting for NTP sync...\n");
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
        printf("Time sync failed or timed out.\n");
        return;
    }

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    char buf[64];
    strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &timeinfo);
    printf("Current time: %s\n", buf);
}

void DataHandler::update_current_temperature()
{
    std::string new_temperature = weather_station.get_temperature_xml();

    if (new_temperature.empty())
    {
        return;
    }

    if (new_temperature != current_temperature)
    {
        set_current_temperature(new_temperature);
        set_weather_data_available(true);
    }
}

void DataHandler::update_current_time()
{
    auto time = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(time);

    std::tm time_info{};

    localtime_r(&now_c, &time_info);

    std::put_time(&time_info, "%H:%M");

    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M", &time_info);

    std::string new_time;
    new_time.assign(buffer);

    if (current_time != new_time)
    {
        set_current_time(new_time);
        set_clock_data_available(true);
    }
}

void DataHandler::set_current_temperature(const std::string& temperature)
{
    current_temperature = temperature;
}

const std::string& DataHandler::get_current_temperature() const
{
    return current_temperature;
}

void DataHandler::set_current_date(const std::string& date)
{
    current_date = date;
}

const std::string& DataHandler::get_current_date() const
{
    return current_date;
}

void DataHandler::set_current_time(const std::string& time)
{
    current_time = time;
}

const std::string& DataHandler::get_current_time() const
{
    return current_time;
}
