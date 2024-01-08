#ifndef _MOTOR_H
#define _MOTOR_H


typedef enum 
{
    MOTOR_MODE_IDEL,
    MOTOR_SMART_MODE,
    MOTOR_CARE_MODE,
}motor_mode_t;

void motor_init();
void set_motor_mode(motor_mode_t mode);
void motor_process();




#endif