#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN 9
#define MAX_LED 2


int val = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel( MAX_LED, PIN, NEO_RGB + NEO_KHZ800 );


void led_init()
{
    strip.setBrightness(250);
    strip.begin();
    strip.show();
}

void led_task()
{
    static uint32_t timeout = 0;
    static uint8_t i, hue;
    uint32_t color = strip.ColorHSV(1000, 192, 255);
    if (millis() - timeout > 50)
    {
        timeout = millis();
        strip.setPixelColor(i, strip.Color(hue, 255-hue, hue)); //设置每一个LED颜色
        strip.show();
        if (i > 1) i = 0;
        if (hue++ >= 256) hue = 0;
    }
}



