#include "ForecastWeatherUnit.hpp"

void ForecastWeatherUnit::create_forecast(lv_obj_t *parent, lv_coord_t width,
                                          lv_coord_t height, const WeatherDayData &data)
{
    if (view_.root() != nullptr)
    {
        update(data);
        return;
    }

    // This forecast unit owns the data for exactly one future day.
    data_ = data;
    view_.create(parent, width, height);
    view_.set_weekday_position(LV_ALIGN_TOP_LEFT, 8, 6);
    view_.set_icon_position(LV_ALIGN_TOP_MID, 0, 28);
    view_.set_temperature_range_positions(LV_ALIGN_BOTTOM_MID, 0, -28,
                                          LV_ALIGN_BOTTOM_MID, 0, -8);
    view_.render(data_);
}

void ForecastWeatherUnit::update(const WeatherDayData &data)
{
    // Store new data before asking the view to render it.
    data_ = data;
    view_.render(data_);
}

void ForecastWeatherUnit::set_weekday_short(const char *weekday_short)
{
    // The view renders data but does not keep its own copy.
    data_.weekday_short = weekday_short;
    view_.render(data_);
}
