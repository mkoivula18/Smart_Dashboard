#include "WeatherWidget.hpp"
#include <stdio.h>

void WeatherWidget::create(lv_obj_t *parent, const char *value, const char *caption)
{
    if (container_ != nullptr) {
        set_value(value);
        set_caption(caption);
        return;
    }
    printf("creating weather widget, parent = %p\n", (void*)parent);
    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, 340, 80);

    lv_obj_set_style_bg_opa(container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(container_, 0, 0);
    lv_obj_set_style_pad_all(container_, 0, 0);

    value_label_ = lv_label_create(container_);
    lv_label_set_text(value_label_, value);
    lv_obj_set_style_text_font(value_label_, &lv_font_montserrat_48, 0);
    lv_obj_align(value_label_, LV_ALIGN_LEFT_MID, 0, 0);

    unit_label_ = lv_label_create(container_);
    lv_label_set_text(unit_label_, "°C");
    lv_obj_set_style_text_font(unit_label_, &lv_font_montserrat_24, 0);
    lv_obj_align_to(unit_label_, value_label_, LV_ALIGN_OUT_RIGHT_TOP, 8, 10);

    caption_label_ = lv_label_create(container_);
    lv_label_set_text(caption_label_, caption);
    lv_obj_align(caption_label_, LV_ALIGN_BOTTOM_LEFT, 4, 0);
}

void WeatherWidget::set_value(const char *value)
{
    if (value_label_ == nullptr) {
        return;
    }

    lv_label_set_text(value_label_, value);
}

void WeatherWidget::set_caption(const char *caption)
{
    if (caption_label_ == nullptr) {
        return;
    }

    lv_label_set_text(caption_label_, caption);
}

void WeatherWidget::set_position(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    if (container_ == nullptr) {
        return;
    }

    lv_obj_align(container_, align, x_ofs, y_ofs);
}
