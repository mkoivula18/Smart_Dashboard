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
    (void)time_text;
}

void ClockWidget::set_time(const char *time_text)
{
    if (time_label_ == nullptr) {
        return;
    }

    lv_label_set_text(time_label_, time_text);
}

void ClockWidget::set_position(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    if (container_ == nullptr) {
        return;
    }

    lv_obj_align(container_, align, x_ofs, y_ofs);
}
