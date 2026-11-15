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
    lv_obj_set_size(container_, SCREEN_SIZE_WIDTH, 160);
    set_partial_refresh_background();
    lv_obj_set_style_pad_all(container_, 0, 0);

    temp_subcontainer_ = lv_obj_create(container_);
    lv_obj_set_size(temp_subcontainer_, 320, 120);
    lv_obj_center(temp_subcontainer_);
    lv_obj_set_style_bg_opa(temp_subcontainer_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_color(temp_subcontainer_, lv_color_black(), 0);
    lv_obj_set_style_border_width(temp_subcontainer_, 0, 0);
    lv_obj_set_style_radius(temp_subcontainer_, 12, 0);
    lv_obj_set_style_pad_all(temp_subcontainer_, 12, 0);

    value_label_ = lv_label_create(temp_subcontainer_);
    lv_label_set_text(value_label_, value);
    lv_obj_set_style_text_font(value_label_, &lv_font_montserrat_48, 0);
    lv_obj_align(value_label_, LV_ALIGN_TOP_MID, -18, 10);

    unit_label_ = lv_label_create(temp_subcontainer_);
    lv_label_set_text(unit_label_, "°C");
    lv_obj_set_style_text_font(unit_label_, &lv_font_montserrat_24, 0);
    lv_obj_align_to(unit_label_, value_label_, LV_ALIGN_OUT_RIGHT_TOP, 6, 10);

    caption_label_ = lv_label_create(temp_subcontainer_);
    lv_label_set_text(caption_label_, caption);
    lv_obj_set_style_text_font(caption_label_, &lv_font_montserrat_14, 0);
    lv_obj_align(caption_label_, LV_ALIGN_BOTTOM_MID, 0, -8);
}

void WeatherWidget::set_value(const char *value)
{
    if (value_label_ == nullptr || unit_label_ == nullptr) {
        return;
    }

    lv_label_set_text(value_label_, value);
    // A label's width changes with the number of digits. Re-apply the
    // relative alignment so the unit keeps a consistent gap after 1, 12,
    // and negative temperatures alike.
    lv_obj_align_to(unit_label_, value_label_, LV_ALIGN_OUT_RIGHT_TOP, 6, 10);
}

void WeatherWidget::set_caption(const char *caption)
{
    if (caption_label_ == nullptr) {
        return;
    }

    lv_label_set_text(caption_label_, caption);
}
