#include<Arduino.h>
#include <input.h>
#include "OneButton.h"


OneButton button(35, false);

uint8_t key_buf[10];

uint8_t head_index = 0;
uint8_t tail_index = 0;
uint8_t buf_index = 0;

void put_key(uint8_t val)
{
    if (++buf_index >= 10) buf_index = 0;
    key_buf[head_index] = val;
    if (++head_index >= 10) head_index = 0;
}

void click() {
    put_key(1);
} 


/**
 * @brief 获取输入设备
 * 
 * @return uint8_t 
 */
uint8_t get_key_value()
{
    uint8_t res = 0;
    if (buf_index == 0) return 0;
    buf_index--;
    res = key_buf[tail_index];
    tail_index++;
    if (tail_index >= 10) tail_index = 0;
    return res;
}

void longPressStart()
{
    put_key(2);
}

/**
 * @brief 输入设备初始化
 * 
 */
void input_init()
{
     button.attachClick(click);
     button.attachLongPressStart(longPressStart);
}

void input_process()
{
    button.tick();
}