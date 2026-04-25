#include "ClockWidget.hpp"

void ClockWidget::create(lv_obj_t *parent, const char *time_text)
{
    if (container_ != nullptr) {
        set_time(time_text);
        return;
    }

    container_ = lv_obj_create(parent);

    // Add the visual contents of the clock widget here.
    // Example: create labels, set fonts, set size, and apply styles.

    lv_obj_set_size(container_, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT / 4);

    value_label_ = lv_label_create(container_);
    lv_label_set_text(value_label_, time_text);
    lv_obj_set_style_text_font(value_label_, &lv_font_montserrat_48, 0);
    lv_obj_center(value_label_);
}

void ClockWidget::set_time(const char *time_text)
{
    if (value_label_ == nullptr) {
        return;
    }

    lv_label_set_text(value_label_, time_text);
}
