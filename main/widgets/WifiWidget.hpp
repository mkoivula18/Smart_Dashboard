#pragma once

#include "lvgl.h"

class WifiWidget
{
public:
    WifiWidget() = default;

    void create(lv_obj_t *parent, const char *status_text);
    void set_status(const char *status_text);
    void set_connected(bool connected);
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
    lv_obj_t *icon_label_ = nullptr;
    lv_obj_t *status_label_ = nullptr;
};
