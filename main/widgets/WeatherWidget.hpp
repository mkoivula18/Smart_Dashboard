#pragma once

#include "lvgl.h"

class WeatherWidget
{
public:

    WeatherWidget(lv_obj_t *parent, const char *default_value, const char *default_caption)
    {
        create(parent, default_value, default_caption);
        set_position(LV_ALIGN_TOP_LEFT, 40, 40);
    }

    void create(lv_obj_t *parent, const char *value, const char *caption);
    void set_value(const char *value);
    void set_caption(const char *caption);
    void set_position(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

    lv_obj_t *root() const
    {
        return container_;
    }

    bool is_created() const
    {
        return container_ != nullptr;
    }

private:
    lv_obj_t *container_ = nullptr;
    lv_obj_t *value_label_ = nullptr;
    lv_obj_t *unit_label_ = nullptr;
    lv_obj_t *caption_label_ = nullptr;
};
