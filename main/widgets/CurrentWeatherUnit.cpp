#include "CurrentWeatherUnit.hpp"

#include <stdio.h>

void CurrentWeatherUnit::create_current_day(lv_obj_t *parent, lv_coord_t width,
                                            lv_coord_t height, const CurrentWeatherData &data)
{
    if (view_.root() != nullptr)
{
        update(data);
        return;
    }

    // This unit keeps the current day's data and gives only the common part to its view.
    data_ = data;
    // The view creates the top-level LVGL container and its shared child objects.
    view_.create(parent, width, height);
    view_.set_weekday_position(LV_ALIGN_TOP_LEFT, 8, 6);
    view_.set_icon_position(LV_ALIGN_TOP_LEFT, 8, 28);
    view_.set_temperature_range_positions(LV_ALIGN_BOTTOM_LEFT, 8, -28,
                                           LV_ALIGN_BOTTOM_LEFT, 8, -8);

    // These labels belong only to the current-day unit, not to the shared view.
    current_caption_label_ = lv_label_create(view_.root());
    lv_label_set_text(current_caption_label_, "Nyt");
    lv_obj_set_style_text_font(current_caption_label_, &lv_font_montserrat_14, 0);
    lv_obj_align(current_caption_label_, LV_ALIGN_TOP_LEFT, 68, 6);

    current_temperature_label_ = lv_label_create(view_.root());
    lv_obj_set_style_text_font(current_temperature_label_, &lv_font_montserrat_48, 0);
    lv_obj_align(current_temperature_label_, LV_ALIGN_TOP_LEFT, 68, 26);

    // The shared view renders weekday, icon, and forecast range from day data.
    view_.render(data_.day);
    render_current_temperature();
}

void CurrentWeatherUnit::update(const CurrentWeatherData &data)
{
    // Replacing this copy keeps the unit as the single owner of its data.
    data_ = data;
    view_.render(data_.day);
    render_current_temperature();
}

void CurrentWeatherUnit::set_current_temperature(int temperature)
{
    // The measured temperature is separate from the day's forecast range.
    data_.current_temperature = temperature;
    render_current_temperature();
}

void CurrentWeatherUnit::set_weekday_short(const char *weekday_short)
{
    // Updating local data first prevents the view from becoming a second data owner.
    data_.day.weekday_short = weekday_short;
    view_.render(data_.day);
}

void CurrentWeatherUnit::render_current_temperature()
{
    if (current_caption_label_ == nullptr || current_temperature_label_ == nullptr)
    {
        return;
    }

    if (data_.current_temperature.has_value())
    {
        char text[32];
        snprintf(text, sizeof(text), "%+d °C", *data_.current_temperature);
        lv_label_set_text(current_caption_label_, "Nyt");
        lv_label_set_text(current_temperature_label_, text);
    }
    else
    {
        lv_label_set_text(current_caption_label_, "");
        lv_label_set_text(current_temperature_label_, "");
    }
}
