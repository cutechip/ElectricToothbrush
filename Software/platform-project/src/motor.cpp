#include <Arduino.h>
#include <motor.h>

#define MOTO_IN1  19
#define MOTO_IN2  22


hw_timer_t *timer = NULL;

motor_mode_t mode_motor;
static uint32_t motor_tick = 0;
uint16_t motor_mode_timeout = 0;

uint8_t duty_cycle = 0;

uint8_t time_cnt = 0;
uint8_t flag= 0;


// 这里负责写电机振动时的波形逻辑，自己调到合适的振动效果。可配合MPU6050去设置不同的振动频率
void IRAM_ATTR TimerEvent()
{
    time_cnt++;
    if (time_cnt == 1)
    {
        digitalWrite(MOTO_IN1, 1);
        digitalWrite(MOTO_IN2, 0);
    }
    else if(time_cnt == motor_mode_timeout)
    {
        digitalWrite(MOTO_IN1, 0);
        digitalWrite(MOTO_IN2, 1);
    }
    else if(time_cnt ==  motor_mode_timeout + 2)
    {
        digitalWrite(MOTO_IN1, 0);
        digitalWrite(MOTO_IN2, 0);
    }
    else if (time_cnt == 7)
    {
        time_cnt = 0;
    }
}
 

void motor_init()
{
    
    pinMode(MOTO_IN1, OUTPUT);
    pinMode(MOTO_IN2, OUTPUT);
    digitalWrite(MOTO_IN1, 0);
    digitalWrite(MOTO_IN2, 0);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &TimerEvent, true);
    timerAlarmWrite(timer, 500, true);
    timerAlarmDisable(timer); 
}


/**
 * @brief S设置电机模式
 * 
 * @param mode 
 */
void set_motor_mode(motor_mode_t mode)
{
    mode_motor = mode;
    motor_tick = millis();
    switch (mode)
    {
    case MOTOR_MODE_IDEL:
        digitalWrite(MOTO_IN1, 0);
        digitalWrite(MOTO_IN2, 0);
        timerAlarmDisable(timer); 
        break;
    case MOTOR_SMART_MODE:
        motor_mode_timeout = 2;
        timerAlarmEnable(timer); 
        break;
    case MOTOR_CARE_MODE:
        motor_mode_timeout = 3;
        timerAlarmEnable(timer); 
        break;
    default:
        break;
    }
    
}


/**
 * @brief 电机处理任务
 * 
 */
void motor_process()
{
    // static uint8_t motor_flag = 0;
    // if (mode_motor == MOTOR_MODE_IDEL) 
    // {
    //     digitalWrite(MOTO_IN1, 0);
    //     digitalWrite(MOTO_IN2, 0);
    //     return ;
    // }
    // if (millis() - motor_tick > motor_mode_timeout)
    // {
    //     motor_tick = millis();
    //     digitalWrite(MOTO_IN1, motor_flag);
    //     digitalWrite(MOTO_IN2, !motor_flag);
    //     motor_flag = !motor_flag;
    // }

}