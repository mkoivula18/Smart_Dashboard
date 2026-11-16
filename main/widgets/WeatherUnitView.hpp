#pragma once

#include "WeatherData.hpp"
#include "lvgl.h"

// LVGL view shared by current and forecast weather units. Its parent owns data.
class WeatherUnitView
{
public:
    void create(lv_obj_t *parent, lv_coord_t width, lv_coord_t height);
    void render(const WeatherDayData &data);

    void set_weekday_position(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset);
    void set_icon_position(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset);
    void set_temperature_range_positions(lv_align_t high_align,
                                         lv_coord_t high_x_offset, lv_coord_t high_y_offset,
                                         lv_align_t low_align,
                                         lv_coord_t low_x_offset,
                                         lv_coord_t low_y_offset);

    lv_obj_t *root() const
    {
        return container_;
    }

private:
    void render_icon(WeatherIconKind icon_kind);
    static void set_temperature_label(lv_obj_t *label, const char *prefix, int temperature);

    lv_obj_t *container_ = nullptr;
    lv_obj_t *weekday_label_ = nullptr;
    lv_obj_t *icon_slot_ = nullptr;
    lv_obj_t *low_label_ = nullptr;
    lv_obj_t *high_label_ = nullptr;
};
