#include <stdio.h>

class WifiHelper
{
public:
    WifiHelper()
    {
        printf("WifiHelper here hello hazagaan?\n");
    }
    bool connect_to_wifi(const char *ssid, const char *password);    

private:

};
