#include <Arduino.h>
#include <sys_power.h>


#define LCD_BG      13
#define SYS_POWER   14
#define AUDIO_EN    10

void sys_power_init()
{
    pinMode(SYS_POWER, OUTPUT);
    pinMode(LCD_BG, OUTPUT);
    pinMode(AUDIO_EN, OUTPUT);
    digitalWrite(LCD_BG, 0);
    digitalWrite(AUDIO_EN, 1);
    digitalWrite(SYS_POWER, 1);
}


/**
 * @brief 打开电源
 * 
 */
void sys_power_enable()
{
    digitalWrite(SYS_POWER, 1);
}


/**
 * @brief 系统电源关闭
 * 
 */
void sys_power_disable()
{
    digitalWrite(SYS_POWER, 0);
}



void set_audio_enable()
{
    digitalWrite(AUDIO_EN, 0);
}


/**
 * @brief 系统电源关闭
 * 
 */
void set_audio_disable()
{
    digitalWrite(AUDIO_EN, 1);
}
