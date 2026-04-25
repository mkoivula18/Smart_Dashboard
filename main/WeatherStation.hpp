#pragma once

#include <stdio.h>
#include "InternetUser.hpp"

class WeatherStation : InternetUser
{
public:
    WeatherStation()
    {
        ESP_LOGI(TAG, "I am WeatherStation. Nice to meet you.\n");
    }

    std::string get_temperature_xml();

private:
    const char *TAG = "WeatherStation";
    std::string FMI_URL = "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&place=Tampere&parameters=t2m&";

};
