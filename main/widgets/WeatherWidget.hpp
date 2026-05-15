#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"
#include "Widget.hpp"

class WeatherWidget : public Widget
{
public:
    WeatherWidget(lv_obj_t *parent, const char *default_value, const char *default_caption)
    {
        create(parent, default_value, default_caption);
    }

    void create(lv_obj_t *parent, const char *value, const char *caption);
    void set_value(const char *value);
    void set_caption(const char *caption);

private:
    lv_obj_t *temp_subcontainer_    = nullptr;
    lv_obj_t *value_label_          = nullptr;
    lv_obj_t *unit_label_           = nullptr;
    lv_obj_t *caption_label_        = nullptr;
};
