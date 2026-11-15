#include "WeatherWidget.hpp"

namespace {
constexpr WeatherIconKind PLACEHOLDER_ICON = WeatherIconKind::Placeholder;
constexpr const char *WEEKDAY_SHORT[] = {"su", "ma", "ti", "ke", "to", "pe", "la"};
constexpr int WEEKDAY_COUNT = sizeof(WEEKDAY_SHORT) / sizeof(WEEKDAY_SHORT[0]);

constexpr int PLACEHOLDER_LOW_TEMPERATURE = -6;
constexpr int PLACEHOLDER_HIGH_TEMPERATURE = 2;
}

void WeatherWidget::create(lv_obj_t *parent)
{
    if (container_ != nullptr) {
        return;
    }

    container_ = lv_obj_create(parent);
    lv_obj_set_size(container_, WIDGET_WIDTH, WIDGET_HEIGHT);
    set_partial_refresh_background();
    lv_obj_set_style_pad_all(container_, 0, 0);
    add_borders_to_container();

    current_day_unit_.create_current_day(
        container_, CURRENT_UNIT_WIDTH, WIDGET_HEIGHT,
        {{"XX", PLACEHOLDER_LOW_TEMPERATURE,
          PLACEHOLDER_HIGH_TEMPERATURE, PLACEHOLDER_ICON}, 1});
    lv_obj_set_pos(current_day_unit_.root(), OUTER_PADDING, 0);

    // LVGL resolves object coordinates lazily. Force that calculation before
    // deriving the forecast row from the current unit's actual geometry.
    lv_obj_update_layout(container_);
    const lv_coord_t forecast_start_x = lv_obj_get_x(current_day_unit_.root()) +
                                        lv_obj_get_width(current_day_unit_.root()) + UNIT_GAP;
    const lv_coord_t forecast_area_width = lv_obj_get_width(container_) -
                                           forecast_start_x - OUTER_PADDING;

    const lv_coord_t forecast_unit_width =
        (forecast_area_width - (FORECAST_DAY_COUNT - 1) * UNIT_GAP) /
        FORECAST_DAY_COUNT;

    for (size_t index = 0; index < FORECAST_DAY_COUNT; ++index) {
        forecast_units_[index].create_forecast(
            container_, forecast_unit_width, WIDGET_HEIGHT,
            {"XX", PLACEHOLDER_LOW_TEMPERATURE,
             PLACEHOLDER_HIGH_TEMPERATURE, PLACEHOLDER_ICON});
        lv_obj_set_pos(forecast_units_[index].root(),
                       forecast_start_x + index * (forecast_unit_width + UNIT_GAP), 0);
    }
}

void WeatherWidget::set_current_temperature(int temperature)
{
    current_day_unit_.set_current_temperature(temperature);
}

bool WeatherWidget::set_current_weekday(int weekday_index)
{
    if (weekday_index < 0 || weekday_index >= WEEKDAY_COUNT ||
        weekday_index == current_weekday_index_) {
        return false;
    }

    current_weekday_index_ = weekday_index;
    current_day_unit_.set_weekday_short(WEEKDAY_SHORT[weekday_index]);
    for (size_t index = 0; index < FORECAST_DAY_COUNT; ++index) {
        const int days_after_current_day = static_cast<int>(index) + 1;
        const int forecast_weekday_index =
            (weekday_index + days_after_current_day) % WEEKDAY_COUNT;

        forecast_units_[index].set_weekday_short(
            WEEKDAY_SHORT[forecast_weekday_index]);
    }
    return true;
}
