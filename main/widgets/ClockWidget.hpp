#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"
#include "Widget.hpp"

class ClockWidget : public Widget
{
public:
    ClockWidget(lv_obj_t *parent, const char *default_time,
                const char *default_weekday, const char *default_date)
    {
        create(parent, default_time, default_weekday, default_date);
    }

    void create(lv_obj_t *parent, const char *time_text,
                const char *weekday_text, const char *date_text);
    void set_datetime(const char *time_text, const char *weekday_text,
                      const char *date_text);

private:
    lv_obj_t *value_label_ = nullptr;
    lv_obj_t *weekday_label_ = nullptr;
    lv_obj_t *date_label_ = nullptr;
    lv_obj_t *date_container_ = nullptr;
};
