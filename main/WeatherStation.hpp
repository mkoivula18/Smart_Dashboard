#include <stdio.h>

class WeatherStation
{
public:
    WeatherStation()
    {
        printf("I am WeatherStation. Nice to meet you.\n");
    }

    bool connect_to_wifi(const char *ssid, const char *password);
private:

};
