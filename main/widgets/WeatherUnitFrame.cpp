#include "WeatherUnitFrame.hpp"

#include <stdio.h>

namespace {
constexpr lv_coord_t ICON_SIZE = 56;
}

void WeatherUnitFrame::create(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                              const ForecastDayData &data)
{
    if (container_ != nullptr) {
        update(data);
        return;
    }

    data_ = data;
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, width, height);
    lv_obj_set_style_bg_color(container_, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(container_, lv_color_black(), 0);
    lv_obj_set_style_border_width(container_, 1, 0);
    lv_obj_set_style_pad_all(container_, 0, 0);

    weekday_label_ = lv_label_create(container_);
    lv_obj_set_style_text_font(weekday_label_, &lv_font_montserrat_14, 0);

    icon_placeholder_ = lv_obj_create(container_);
    lv_obj_set_size(icon_placeholder_, ICON_SIZE, ICON_SIZE);
    lv_obj_set_style_bg_opa(icon_placeholder_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(icon_placeholder_, lv_color_black(), 0);
    lv_obj_set_style_border_width(icon_placeholder_, 1, 0);
    lv_obj_set_style_pad_all(icon_placeholder_, 0, 0);

    low_label_ = lv_label_create(container_);
    high_label_ = lv_label_create(container_);
    lv_obj_set_style_text_font(low_label_, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_font(high_label_, &lv_font_montserrat_14, 0);

    render_data();
}

void WeatherUnitFrame::update(const ForecastDayData &data)
{
    data_ = data;
    render_data();
}

void WeatherUnitFrame::set_weekday_short(const char *weekday_short)
{
    data_.weekday_short = weekday_short;
    render_data();
}

void WeatherUnitFrame::set_weekday_position(lv_align_t align, lv_coord_t x_offset,
                                             lv_coord_t y_offset)
{
    lv_obj_align(weekday_label_, align, x_offset, y_offset);
}

void WeatherUnitFrame::set_icon_position(lv_align_t align, lv_coord_t x_offset,
                                          lv_coord_t y_offset)
{
    lv_obj_align(icon_placeholder_, align, x_offset, y_offset);
}

void WeatherUnitFrame::set_temperature_range_positions(lv_align_t high_align,
                                                        lv_coord_t high_x_offset,
                                                        lv_coord_t high_y_offset,
                                                        lv_align_t low_align,
                                                        lv_coord_t low_x_offset,
                                                        lv_coord_t low_y_offset)
{
    lv_obj_align(high_label_, high_align, high_x_offset, high_y_offset);
    lv_obj_align(low_label_, low_align, low_x_offset, low_y_offset);
}

void WeatherUnitFrame::render_data()
{
    if (container_ == nullptr) {
        return;
    }

    lv_label_set_text(weekday_label_, data_.weekday_short.c_str());
    set_temperature_label(low_label_, "Alin", data_.low_temperature);
    set_temperature_label(high_label_, "Ylin", data_.high_temperature);

    switch (data_.icon_kind) {
    case WeatherIconKind::Placeholder:
        lv_obj_remove_flag(icon_placeholder_, LV_OBJ_FLAG_HIDDEN);
        break;
    }
}

void WeatherUnitFrame::set_temperature_label(lv_obj_t *label, const char *prefix,
                                              int temperature)
{
    char text[32];
    snprintf(text, sizeof(text), "%s %+d °C", prefix, temperature);
    lv_label_set_text(label, text);
}
