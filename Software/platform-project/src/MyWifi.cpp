#include <WiFi.h>
#include <MyWifi.h>


const char* ssid = "YYDS";
const char* password = "@102666666";

void wifi_init()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}