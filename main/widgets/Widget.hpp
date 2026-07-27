#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"

class Widget
{
public:
    void set_position(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        if (container_ == nullptr) {
            return;
        }

        lv_obj_align(container_, align, x_ofs, y_ofs);
    }

    lv_obj_t *root() const
    {
        return container_;
    }

    bool is_created() const
    {
        return container_ != nullptr;
    }

    lv_obj_t *get_container()
    {
        return container_;
    }

    void add_borders_to_container()
    {
        lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_color(container_, lv_color_black(), 0);
        lv_obj_set_style_border_width(container_, 4, 0);
    }

protected:
    lv_obj_t *container_ = nullptr;
};
