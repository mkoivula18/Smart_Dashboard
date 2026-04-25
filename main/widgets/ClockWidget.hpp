#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"
#include "Widget.hpp"

class ClockWidget : public Widget
{
public:
    ClockWidget(lv_obj_t *parent, const char *default_time)
    {
        create(parent, default_time);
    }

    void create(lv_obj_t *parent, const char *time_text);
    void set_time(const char *time_text);

private:
    lv_obj_t *value_label_ = nullptr;
};
