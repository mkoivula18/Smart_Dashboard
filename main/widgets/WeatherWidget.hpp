#pragma once

#include <array>
#include <cstddef>

#include "CommonDefs.hpp"
#include "CurrentWeatherUnit.hpp"
#include "ForecastWeatherUnit.hpp"
#include "Widget.hpp"

class WeatherWidget : public Widget
{
public:
    explicit WeatherWidget(lv_obj_t *parent)
    {
        create(parent);
    }

    void create(lv_obj_t *parent);
    void set_current_temperature(int temperature);
    bool set_current_weekday(int weekday_index);

private:
    // Match the clock and Wi-Fi outer margins. The weather-widget border is
    // inset from the dashboard edges, not only its child content.
    static constexpr lv_coord_t WIDGET_WIDTH =
        SCREEN_SIZE_WIDTH - (2 * DASHBOARD_LAYOUT_MARGIN);
    static constexpr lv_coord_t WIDGET_HEIGHT = 140;
    // Units use the full WeatherWidget width; the widget itself already has
    // the dashboard-wide outer margin.
    static constexpr lv_coord_t OUTER_PADDING = 0;
    static constexpr lv_coord_t UNIT_GAP = DASHBOARD_LAYOUT_GAP;
    static constexpr lv_coord_t CURRENT_UNIT_WIDTH = 250;
    // This compile-time count keeps forecast ownership in std::array and
    // avoids heap allocation on the ESP32.
    static constexpr size_t FORECAST_DAY_COUNT = 4;
    static constexpr lv_coord_t MIN_FORECAST_UNIT_WIDTH = 90;
    static constexpr lv_coord_t FORECAST_ROW_WIDTH =
        WIDGET_WIDTH - (2 * OUTER_PADDING) - CURRENT_UNIT_WIDTH - UNIT_GAP;

    static_assert(FORECAST_DAY_COUNT > 0, "At least one forecast day is required");
    static_assert(FORECAST_ROW_WIDTH >=
                      FORECAST_DAY_COUNT * MIN_FORECAST_UNIT_WIDTH +
                          (FORECAST_DAY_COUNT - 1) * UNIT_GAP,
                  "Forecast-day count leaves units too narrow for their content");
    static_assert((FORECAST_ROW_WIDTH - (FORECAST_DAY_COUNT - 1) * UNIT_GAP) %
                          FORECAST_DAY_COUNT == 0,
                  "Forecast-row width must divide evenly between forecast units");

    CurrentWeatherUnit current_day_unit_{};
    std::array<ForecastWeatherUnit, FORECAST_DAY_COUNT> forecast_units_{};
    int current_weekday_index_ = -1;
};
