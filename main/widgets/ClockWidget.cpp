#include "ClockWidget.hpp"

void ClockWidget::create(lv_obj_t *parent, const char *time_text,
                         const char *weekday_text, const char *date_text)
{
    if (container_ != nullptr) {
        set_datetime(time_text, weekday_text, date_text);
        return;
    }

    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, 480, 120);
    set_partial_refresh_background();
    lv_obj_set_style_pad_all(container_, 0, 0);

    value_label_ = lv_label_create(container_);
    lv_label_set_text(value_label_, time_text);
    lv_obj_set_style_text_font(value_label_, &lv_font_montserrat_48, 0);
    lv_obj_align(value_label_, LV_ALIGN_LEFT_MID, 12, 0);

    date_container_ = lv_obj_create(container_);
    // Keep the supporting date information visually tied to the time rather
    // than spreading it across the available clock-widget width.
    lv_obj_set_size(date_container_, 160, 64);
    lv_obj_align(date_container_, LV_ALIGN_LEFT_MID, 125, 0);
    lv_obj_set_style_bg_opa(date_container_, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(date_container_, 0, 0);
    lv_obj_set_style_pad_all(date_container_, 0, 0);

    weekday_label_ = lv_label_create(date_container_);
    lv_label_set_text(weekday_label_, weekday_text);
    lv_obj_set_style_text_font(weekday_label_, &lv_font_montserrat_24, 0);
    lv_obj_align(weekday_label_, LV_ALIGN_TOP_MID, 0, 0);

    date_label_ = lv_label_create(date_container_);
    lv_label_set_text(date_label_, date_text);
    lv_obj_set_style_text_font(date_label_, &lv_font_montserrat_24, 0);
    lv_obj_align(date_label_, LV_ALIGN_TOP_MID, 0, 32);
}

void ClockWidget::set_datetime(const char *time_text, const char *weekday_text,
                               const char *date_text)
{
    if (value_label_ == nullptr || weekday_label_ == nullptr || date_label_ == nullptr) {
        return;
    }

    lv_label_set_text(value_label_, time_text);
    lv_label_set_text(weekday_label_, weekday_text);
    lv_label_set_text(date_label_, date_text);
}
