#pragma once

#include <string>

#include "lvgl.h"

enum class WeatherIconKind
{
    Placeholder,
};

struct ForecastDayData
{
    std::string weekday_short;
    int low_temperature;
    int high_temperature;
    WeatherIconKind icon_kind;
};

class WeatherUnitFrame
{
public:
    void create(lv_obj_t *parent, lv_coord_t width, lv_coord_t height,
                const ForecastDayData &data);
    void update(const ForecastDayData &data);
    void set_weekday_short(const char *weekday_short);

    void set_weekday_position(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset);
    void set_icon_position(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset);
    void set_temperature_range_positions(lv_align_t high_align,
                                         lv_coord_t high_x_offset, lv_coord_t high_y_offset,
                                         lv_align_t low_align,
                                         lv_coord_t low_x_offset, lv_coord_t low_y_offset);

    lv_obj_t *root() const { return container_; }

private:
    void render_data();
    static void set_temperature_label(lv_obj_t *label, const char *prefix, int temperature);

    ForecastDayData data_{};
    lv_obj_t *container_ = nullptr;
    lv_obj_t *weekday_label_ = nullptr;
    lv_obj_t *icon_placeholder_ = nullptr;
    lv_obj_t *low_label_ = nullptr;
    lv_obj_t *high_label_ = nullptr;
};
