#include "WifiWidget.hpp"

void WifiWidget::create(lv_obj_t *parent, const char *status_text)
{
    if (container_ != nullptr) {
        set_status(status_text);
        return;
    }

    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, 180, 48);
    set_partial_refresh_background();
    lv_obj_set_style_pad_all(container_, 0, 0);

    icon_label_ = lv_label_create(container_);
    lv_label_set_text(icon_label_, LV_SYMBOL_WIFI);
    lv_obj_set_style_text_font(icon_label_, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(icon_label_, lv_color_black(), 0);
    lv_obj_align(icon_label_, LV_ALIGN_LEFT_MID, 6, 0);

    status_label_ = lv_label_create(container_);
    lv_label_set_text(status_label_, status_text);
    lv_obj_set_style_text_font(status_label_, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(status_label_, lv_color_black(), 0);
    lv_obj_align_to(status_label_, icon_label_, LV_ALIGN_OUT_RIGHT_MID, 6, 0);
}

void WifiWidget::set_status(const char *status_text)
{
    if (status_label_ == nullptr) {
        return;
    }

    lv_label_set_text(status_label_, status_text);
}

void WifiWidget::set_connected(bool connected)
{
    if (icon_label_ == nullptr) {
        return;
    }

    lv_obj_set_style_text_color(
        icon_label_,
        connected ? lv_color_black() : lv_palette_main(LV_PALETTE_GREY),
        0);
}
