#pragma once

#include <optional>
#include <string>

enum class WeatherIconKind
{
    // The API adapter will translate raw provider codes into these UI-neutral states.
    Unknown,
    Clear,
    PartlyCloudy,
    Cloudy,
    Fog,
    Drizzle,
    Rain,
    Sleet,
    Snow,
    Thunderstorm,
};

// Shared forecast information for one calendar day.
struct WeatherDayData
{
    std::string weekday_short;
    int low_temperature;
    int high_temperature;
    WeatherIconKind icon_kind;
};

// The current day has forecast information plus a measured temperature.
struct CurrentWeatherData
{
    WeatherDayData day;
    std::optional<int> current_temperature;
};
