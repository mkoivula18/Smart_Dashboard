#pragma once

#include <optional>

#include "WeatherUnitFrame.hpp"

struct CurrentDayData
{
    ForecastDayData forecast;
    std::optional<int> current_temperature;
};

class CurrentWeatherUnit
{
public:
    void create_current_day(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                            const CurrentDayData &data);
    void update(const CurrentDayData &data);
    void set_current_temperature(int temperature);
    void set_weekday_short(const char *weekday_short);
    lv_obj_t *root() const { return frame_.root(); }

private:
    void render_current_temperature();

    CurrentDayData data_{};
    WeatherUnitFrame frame_{};
    lv_obj_t *current_caption_label_ = nullptr;
    lv_obj_t *current_temperature_label_ = nullptr;
};
