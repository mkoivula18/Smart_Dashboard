#pragma once

#include "WeatherData.hpp"
#include "WeatherUnitView.hpp"

class CurrentWeatherUnit
{
public:
    void create_current_day(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                            const CurrentWeatherData &data);
    void update(const CurrentWeatherData &data);
    void set_current_temperature(int temperature);
    void set_weekday_short(const char *weekday_short);
    lv_obj_t *root() const
    {
        return view_.root();
    }

private:
    void render_current_temperature();

    CurrentWeatherData data_{};
    WeatherUnitView view_{};
    lv_obj_t *current_caption_label_ = nullptr;
    lv_obj_t *current_temperature_label_ = nullptr;
};
