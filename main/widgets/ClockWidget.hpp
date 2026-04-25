#pragma once

#include "lvgl.h"

class ClockWidget
{
public:
    ClockWidget() = default;

    ClockWidget(lv_obj_t *parent, const char *default_time)
    {
        create(parent, default_time);
    }

    void create(lv_obj_t *parent, const char *time_text);
    void set_time(const char *time_text);
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
    lv_obj_t *time_label_ = nullptr;
};
