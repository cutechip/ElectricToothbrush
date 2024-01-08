#include "Arduino.h"
#include "WiFiMulti.h"

#include "LittleFS.h"


#include "lvgl.h"

#include <input.h>
#include <ui.h>
#include <motor.h>
#include <sys_power.h>
#include <PlayAudio.h>
#include <ota.h>
#include <MyWifi.h>
#include<led.h>
#include <mpu6050.h>
#include<netdata.h>
#include<config.h>
#include<charge_dec.h>


void setup() 
{
    Serial.begin(115200);
    Serial.printf("\r\nV%d...\r\n", FIRMWARE_VERSION);
    sys_power_init();
    if(!LittleFS.begin()){
        Serial.println("LittleFS Mount Failed");
        return;
    }
    wifi_init();
    update_net_data();
    input_init();
    ui_init();
    motor_init();
    audio_init();
    led_init();
    mpu6050_init();
    charge_dec_init();
}

void loop()
{
    lv_task_handler();
    ui_process();
    input_process();
    motor_process();
    audio_task();
    led_task();
    mpu6050_task();
    charge_dec_task();
}

