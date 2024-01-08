#include<Arduino.h>
#include<charge_dec.h>

uint8_t charge_flag = 0;


/**
 * @brief 初始化
 * 
 */
void charge_dec_init()
{
    pinMode(37, INPUT_PULLUP);
}

uint8_t get_charge_flag()
{
    return charge_flag;
}

/**
 * @brief 充电显示任务检测
 * 
 */
void charge_dec_task()
{
    static uint32_t time = 0;
    static uint8_t hight_cnt, low_cnt = 0;
    if (millis() - time > 10)
    {
        time = millis();
        if (digitalRead(37))
        {
            hight_cnt++;
            low_cnt = 0;
            if (hight_cnt >= 10)
            {
                hight_cnt = 10;
                charge_flag = 0;
            }
        } else {
            low_cnt++;
            hight_cnt = 0;
            if (low_cnt >= 10)
            {
                low_cnt = 10;
                charge_flag = 1;
            }
        }
        


        // Serial.printf("digitalRead:%d", digitalRead(37));
    }
    
}