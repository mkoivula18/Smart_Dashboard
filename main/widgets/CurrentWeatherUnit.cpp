#include "CurrentWeatherUnit.hpp"

#include <stdio.h>

void CurrentWeatherUnit::create_current_day(lv_obj_t *parent, lv_coord_t width,
                                            lv_coord_t height, const CurrentDayData &data)
{
    if (frame_.root() != nullptr) {
        update(data);
        return;
    }

    data_ = data;
    frame_.create(parent, width, height, data_.forecast);
    frame_.set_weekday_position(LV_ALIGN_TOP_LEFT, 8, 6);
    frame_.set_icon_position(LV_ALIGN_TOP_LEFT, 8, 28);
    frame_.set_temperature_range_positions(LV_ALIGN_BOTTOM_LEFT, 8, -28,
                                           LV_ALIGN_BOTTOM_LEFT, 8, -8);

    current_caption_label_ = lv_label_create(frame_.root());
    lv_label_set_text(current_caption_label_, "Nyt");
    lv_obj_set_style_text_font(current_caption_label_, &lv_font_montserrat_14, 0);
    lv_obj_align(current_caption_label_, LV_ALIGN_TOP_LEFT, 68, 6);

    current_temperature_label_ = lv_label_create(frame_.root());
    lv_obj_set_style_text_font(current_temperature_label_, &lv_font_montserrat_48, 0);
    lv_obj_align(current_temperature_label_, LV_ALIGN_TOP_LEFT, 68, 26);

    render_current_temperature();
}

void CurrentWeatherUnit::update(const CurrentDayData &data)
{
    data_ = data;
    frame_.update(data_.forecast);
    render_current_temperature();
}

void CurrentWeatherUnit::set_current_temperature(int temperature)
{
    data_.current_temperature = temperature;
    render_current_temperature();
}

void CurrentWeatherUnit::set_weekday_short(const char *weekday_short)
{
    data_.forecast.weekday_short = weekday_short;
    frame_.set_weekday_short(weekday_short);
}

void CurrentWeatherUnit::render_current_temperature()
{
    if (current_temperature_label_ == nullptr) {
        return;
    }

    if (data_.current_temperature.has_value()) {
        char text[32];
        snprintf(text, sizeof(text), "%+d °C", *data_.current_temperature);
        lv_label_set_text(current_temperature_label_, text);
        lv_obj_remove_flag(current_caption_label_, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(current_temperature_label_, LV_OBJ_FLAG_HIDDEN);
    }
    else {
        lv_obj_add_flag(current_caption_label_, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(current_temperature_label_, LV_OBJ_FLAG_HIDDEN);
    }
}
