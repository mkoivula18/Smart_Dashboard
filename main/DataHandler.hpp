#pragma once

#include <stdio.h>
#include <string>
#include <chrono>
#include <ctime>

#include "WeatherStation.hpp"

struct DataAvailable
{
    bool clock      = false;
    bool weather    = false;
    bool wifi       = false;

    bool any_data_available() const
    {
        return clock || weather || wifi;
    }
};

class DataHandler
{
public:
    DataHandler();

    void update_current_temperature();
    void update_current_time();

    // Network Time Protocol, fetch clock sync from internet
    void sync_time_with_ntp();

    void set_current_temperature(const std::string& temperature);
    const std::string& get_current_temperature() const;

    void set_current_date(const std::string& date);
    const std::string& get_current_date() const;

    void set_current_time(const std::string& time);
    const std::string& get_current_time() const;

    void clear_data_available()
    {
        data_available = {};
    }

    bool new_data_available()
    {
        return data_available.any_data_available();
    }

    void set_weather_data_available(bool value)
    {
        printf("New weather data available\n");
        data_available.weather = value;
    }

    void set_clock_data_available(bool value)
    {
        printf("New clock data available\n");
        data_available.clock = value;
    }

    void set_wifi_data_available(bool value)
    {
        printf("New wifi data available\n");
        data_available.wifi = value;
    }

    DataAvailable *get_data_available()
    {
        return &data_available;
    }

private:
    WeatherStation weather_station;

    std::string current_temperature{};
    std::string current_date{};
    std::string current_time{};
    DataAvailable data_available{};

};
