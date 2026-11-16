#pragma once

#include <optional>
#include <stdio.h>
#include "InternetUser.hpp"

class WeatherStation
{
public:
    WeatherStation()
    {
        ESP_LOGI(TAG, "I am WeatherStation. Nice to meet you.\n");
    }

    std::optional<int> get_current_temperature();

private:
    const char *TAG = "WeatherStation";
    InternetUser internet_user_;
    std::string FMI_URL = "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&place=Tampere&parameters=t2m&";

};
