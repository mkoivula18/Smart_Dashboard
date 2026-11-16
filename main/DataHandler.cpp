#include "DataHandler.hpp"
#include "esp_netif_sntp.h"

namespace
{
constexpr const char *VIIKONPAIVAT[] =
{
    "Sunnuntai", "Maanantai", "Tiistai", "Keskiviikko",
    "Torstai", "Perjantai", "Lauantai",
};
}

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
    if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK)
    {
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
    const std::optional<int> new_temperature = weather_station.get_current_temperature();

    if (!new_temperature.has_value())
    {
        return;
    }

    if (new_temperature != current_temperature)
    {
        set_current_temperature(*new_temperature);
        set_weather_data_available(true);
    }
}

void DataHandler::update_current_time()
{
    auto time = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(time);

    std::tm time_info{};

    localtime_r(&now_c, &time_info);

    char time_buffer[10];
    char date_buffer[16];
    strftime(time_buffer, sizeof(time_buffer), "%H:%M", &time_info);
    strftime(date_buffer, sizeof(date_buffer), "%d.%m.%Y", &time_info);

    const std::string new_time(time_buffer);
    const std::string new_date(date_buffer);
    const std::string new_weekday(VIIKONPAIVAT[time_info.tm_wday]);
    const int new_weekday_index = time_info.tm_wday;

    if (current_time != new_time || current_date != new_date ||
        current_weekday != new_weekday || current_weekday_index != new_weekday_index)
    {
        set_current_time(new_time);
        set_current_date(new_date);
        current_weekday = new_weekday;
        current_weekday_index = new_weekday_index;
        set_clock_data_available(true);
    }
}

void DataHandler::set_current_temperature(int temperature)
{
    current_temperature = temperature;
}

const std::optional<int>& DataHandler::get_current_temperature() const
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

const std::string& DataHandler::get_current_weekday() const
{
    return current_weekday;
}

int DataHandler::get_current_weekday_index() const
{
    return current_weekday_index;
}

void DataHandler::set_current_time(const std::string& time)
{
    current_time = time;
}

const std::string& DataHandler::get_current_time() const
{
    return current_time;
}
