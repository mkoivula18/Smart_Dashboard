#pragma once

#include "WeatherData.hpp"
#include "WeatherUnitView.hpp"

class ForecastWeatherUnit
{
public:
    void create_forecast(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                         const WeatherDayData &data);
    void update(const WeatherDayData &data);
    void set_weekday_short(const char *weekday_short);
    lv_obj_t *root() const
    {
        return view_.root();
    }

private:
    WeatherDayData data_{};
    WeatherUnitView view_{};
};
