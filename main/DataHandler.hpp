#pragma once

#include <stdio.h>
#include <string>
#include <chrono>
#include <ctime>

#include "WeatherStation.hpp"


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

    bool new_data_available()
    {
        return data_available;
    }

    void set_new_data_available(bool set)
    {
        data_available = set;
    }

private:
    WeatherStation weather_station;

    std::string current_temperature{};
    std::string current_date{};
    std::string current_time{};
    bool data_available;
};
