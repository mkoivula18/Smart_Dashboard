#include "ForecastWeatherUnit.hpp"

void ForecastWeatherUnit::create_forecast(lv_obj_t *parent, lv_coord_t width,
                                          lv_coord_t height, const ForecastDayData &data)
{
    if (frame_.root() != nullptr) {
        update(data);
        return;
    }

    data_ = data;
    frame_.create(parent, width, height, data_);
    frame_.set_weekday_position(LV_ALIGN_TOP_LEFT, 8, 6);
    frame_.set_icon_position(LV_ALIGN_TOP_MID, 0, 28);
    frame_.set_temperature_range_positions(LV_ALIGN_BOTTOM_MID, 0, -28,
                                           LV_ALIGN_BOTTOM_MID, 0, -8);
}

void ForecastWeatherUnit::update(const ForecastDayData &data)
{
    data_ = data;
    frame_.update(data_);
}

void ForecastWeatherUnit::set_weekday_short(const char *weekday_short)
{
    data_.weekday_short = weekday_short;
    frame_.set_weekday_short(weekday_short);
}
