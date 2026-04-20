#include <stdio.h>

class WeatherStation
{
public:
    WeatherStation()
    {
        printf("I am WeatherStation. Nice to meet you.\n");
    }

private:
    const char *FMI_URL = "https://opendata.fmi.fi/wfs?service=WFS&version=2.0.0&request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&place=Tampere&parameters=t2m&";

};
