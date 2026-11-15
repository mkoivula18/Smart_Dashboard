#pragma once

#include "WeatherUnitFrame.hpp"

class ForecastWeatherUnit
{
public:
    void create_forecast(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                         const ForecastDayData &data);
    void update(const ForecastDayData &data);
    void set_weekday_short(const char *weekday_short);
    lv_obj_t *root() const { return frame_.root(); }

private:
    ForecastDayData data_{};
    WeatherUnitFrame frame_{};
};
