#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"

class Widget
{
public:
    void set_position(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        if (container_ == nullptr)
        {
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

    lv_obj_t *get_container() const
    {
        return container_;
    }

    void add_borders_to_container()
    {
        if (container_ == nullptr)
        {
            return;
        }

        // Keep the opaque background configured by
        // set_partial_refresh_background(). A transparent widget root would
        // leave pixels behind when shorter content replaces older text.
        lv_obj_set_style_border_color(container_, lv_color_black(), 0);
        lv_obj_set_style_border_width(container_, 1, 0);
    }

protected:
    void set_partial_refresh_background()
    {
        if (container_ == nullptr)
        {
            return;
        }

        // A partial refresh must write white pixels over content that has been
        // removed (for example when "XX:XX" becomes a narrower time string).
        lv_obj_set_style_bg_color(container_, lv_color_white(), 0);
        lv_obj_set_style_bg_opa(container_, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(container_, 0, 0);
    }

    lv_obj_t *container_ = nullptr;
};
