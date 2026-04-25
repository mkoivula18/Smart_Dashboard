#pragma once

#include "CommonDefs.hpp"
#include "lvgl.h"
#include "Widget.hpp"

class WifiWidget : public Widget
{
public:
    WifiWidget(lv_obj_t *screen, const char *default_status)
    {
        create(screen, default_status);
    }

    void create(lv_obj_t *parent, const char *status_text);
    void set_status(const char *status_text);
    void set_connected(bool connected);

private:
    lv_obj_t *icon_label_ = nullptr;
    lv_obj_t *status_label_ = nullptr;
};
